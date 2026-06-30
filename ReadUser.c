#include "ReadUser.h"
#include <mongoc/mongoc.h>
#include <string.h>

//reading trained units
void bson_to_unit(const bson_t* doc, Unit* u) {
    bson_iter_t iter;
    bson_iter_init(&iter, doc);

    while (bson_iter_next(&iter)) {
        const char* key = bson_iter_key(&iter);

        if (strcmp(key, "health") == 0)
            u->health = bson_iter_int32(&iter);

        if (strcmp(key, "tile") == 0) {
            bson_iter_t sub;
            bson_iter_recurse(&iter, &sub);
            while (bson_iter_next(&sub)) {
                const char* sk = bson_iter_key(&sub);
                if (strcmp(sk, "x") == 0) u->tile[0] = bson_iter_int32(&sub);
                if (strcmp(sk, "y") == 0) u->tile[1] = bson_iter_int32(&sub);
            }
        }

        if (strcmp(key, "position") == 0) {
            bson_iter_t sub;
            bson_iter_recurse(&iter, &sub);
            while (bson_iter_next(&sub)) {
                const char* sk = bson_iter_key(&sub);
                if (strcmp(sk, "x") == 0) u->position[0] = bson_iter_int32(&sub);
                if (strcmp(sk, "y") == 0) u->position[1] = bson_iter_int32(&sub);
            }
        }
    }
}

void bson_to_unit_block(const bson_t* doc, UnitBlock** ub_out) {
    bson_iter_t iter;
    bson_iter_init(&iter, doc);

    int count = 0;

    // First pass: read count
    while (bson_iter_next(&iter)) {
        if (strcmp(bson_iter_key(&iter), "count") == 0) {
            count = bson_iter_int32(&iter);
            break;
        }
    }

    // Allocate UnitBlock
    UnitBlock* ub = malloc(sizeof(UnitBlock) + count * sizeof(Unit));
    ub->count = count;

    // Second pass: read individuals
    bson_iter_init(&iter, doc);
    while (bson_iter_next(&iter)) {
        if (strcmp(bson_iter_key(&iter), "individuals") == 0) {
            bson_iter_t arr;
            bson_iter_recurse(&iter, &arr);

            int idx = 0;
            while (bson_iter_next(&arr)) {
                const uint8_t* buf;
                uint32_t len;
                bson_iter_document(&arr, &len, &buf);

                bson_t child;
                bson_init_static(&child, buf, len);

                bson_to_unit(&child, &ub->individuals[idx]);
                idx++;
            }
        }
    }

    *ub_out = ub;
}

void bson_to_regimen(const bson_t* doc, Regimen* r){
    bson_iter_t iter;
    bson_iter_init(&iter, doc);

    // init pointers
    for (int i = 0; i < UNIT_MAX; i++) {
        r->units[i] = NULL;
    }

    while (bson_iter_next(&iter)) {
        const char* key = bson_iter_key(&iter);

        if (strcmp(key, "id") == 0) {
            r->id = bson_iter_int32(&iter);
        }

        if (strcmp(key, "units") == 0) {
            bson_iter_t arr;
            bson_iter_recurse(&iter, &arr);

            while (bson_iter_next(&arr)) {
                uint32_t len;
                const uint8_t* buf;

                bson_iter_document(&arr, &len, &buf);

                bson_t child;
                bson_init_static(&child, buf, len);

                int idx = atoi(bson_iter_key(&arr));

                bson_to_unit_block(&child, &r->units[idx]);
            }
        }
    }
}

void bson_to_regimen_queue(const bson_t* doc, RegimenQueue* q){
    bson_iter_t iter;
    bson_iter_init(&iter, doc);

    q->regimens = NULL;
    q->count = 0;
    q->capacity = 0;

    while (bson_iter_next(&iter)) {
        const char* key = bson_iter_key(&iter);

        if (strcmp(key, "count") == 0) {
            q->count = bson_iter_int32(&iter);
        }

        if (strcmp(key, "capacity") == 0) {
            q->capacity = bson_iter_int32(&iter);
        }

        if (strcmp(key, "regimens") == 0) {

            // allocate pointer array
            q->regimens = malloc(sizeof(Regimen*) * q->capacity);

            for (int i = 0; i < q->capacity; i++) {
                q->regimens[i] = NULL;
            }

            bson_iter_t arr;
            bson_iter_recurse(&iter, &arr);

            while (bson_iter_next(&arr)) {
                uint32_t len;
                const uint8_t* buf;

                bson_iter_document(&arr, &len, &buf);

                bson_t child;
                bson_init_static(&child, buf, len);

                int idx = atoi(bson_iter_key(&arr));

                Regimen* r = malloc(sizeof(Regimen));
                bson_to_regimen(&child, r);

                q->regimens[idx] = r;
            }
        }
    }
}


//what regimens are in training
void bson_to_unit_training(const bson_t* doc, UnitTraining* ut){
    bson_iter_t iter;
    bson_iter_init(&iter, doc);

    while (bson_iter_next(&iter)) {
        const char* key = bson_iter_key(&iter);

        if (strcmp(key, "count") == 0) {
            ut->count = bson_iter_int32(&iter);
        }

        if (strcmp(key, "progress") == 0) {
            ut->progress = bson_iter_int32(&iter);
        }

        if (strcmp(key, "finish") == 0) {
            ut->finish = bson_iter_int32(&iter);
        }
    }
}

void bson_to_regimen_training(const bson_t* doc, RegimenTraining* rt){
    bson_iter_t iter;
    bson_iter_init(&iter, doc);

    while (bson_iter_next(&iter)) {
        const char* key = bson_iter_key(&iter);

        // units array
        if (strcmp(key, "units") == 0) {
            bson_iter_t arr;
            bson_iter_recurse(&iter, &arr);

            while (bson_iter_next(&arr)) {
                uint32_t len;
                const uint8_t* buf;

                bson_iter_document(&arr, &len, &buf);

                bson_t child;
                bson_init_static(&child, buf, len);

                int idx = atoi(bson_iter_key(&arr));

                bson_to_unit_training(&child, &rt->units[idx]);
            }
        }

        // deployTile
        if (strcmp(key, "deployTile") == 0) {
            bson_iter_t sub;
            bson_iter_recurse(&iter, &sub);

            while (bson_iter_next(&sub)) {
                const char* sk = bson_iter_key(&sub);

                if (strcmp(sk, "x") == 0) rt->deployTile[0] = bson_iter_int32(&sub);
                if (strcmp(sk, "y") == 0) rt->deployTile[1] = bson_iter_int32(&sub);
            }
        }

        // deployPixel
        if (strcmp(key, "deployPixel") == 0) {
            bson_iter_t sub;
            bson_iter_recurse(&iter, &sub);

            while (bson_iter_next(&sub)) {
                const char* sk = bson_iter_key(&sub);

                if (strcmp(sk, "x") == 0) rt->deployPixel[0] = bson_iter_int32(&sub);
                if (strcmp(sk, "y") == 0) rt->deployPixel[1] = bson_iter_int32(&sub);
            }
        }

        // active
        if (strcmp(key, "active") == 0) {
            rt->active = bson_iter_bool(&iter);
        }
    }
}

void bson_to_regimen_training_list(const bson_t* doc, RegimenTrainingList* list){
    bson_iter_t iter;
    bson_iter_init(&iter, doc);

    while (bson_iter_next(&iter)) {
        if (strcmp(bson_iter_key(&iter), "regimens") == 0) {

            bson_iter_t arr;
            bson_iter_recurse(&iter, &arr);

            while (bson_iter_next(&arr)) {
                uint32_t len;
                const uint8_t* buf;

                bson_iter_document(&arr, &len, &buf);

                bson_t child;
                bson_init_static(&child, buf, len);

                int idx = atoi(bson_iter_key(&arr));

                bson_to_regimen_training(&child, &list->regimens[idx]);
            }
        }
    }
}


//resource
void bson_to_resource(const bson_t* doc, Resource* r){
    bson_iter_t iter;
    bson_iter_init(&iter, doc);

    while (bson_iter_next(&iter)) {
        const char* key = bson_iter_key(&iter);

        if (strcmp(key, "Total") == 0) {r->Total = bson_iter_int32(&iter);}

        if (strcmp(key, "Rate") == 0) {r->Rate = bson_iter_double(&iter);}
    }
}

void bson_to_manpower(const bson_t* doc, ResourceManPower* m){
    bson_iter_t iter;
    bson_iter_init(&iter, doc);

    while (bson_iter_next(&iter)) {
        const char* key = bson_iter_key(&iter);

        if (strcmp(key, "TotalManPower") == 0) {
            m->TotalManPower = bson_iter_int32(&iter);
        }

        if (strcmp(key, "TotalPopulation") == 0) {
            m->TotalPopulation = bson_iter_int32(&iter);
        }

        if (strcmp(key, "Rate") == 0) {
            m->Rate = bson_iter_double(&iter);
        }

        if (strcmp(key, "RecruitableFactor") == 0) {
            m->RecruitableFactor = bson_iter_double(&iter);
        }

        if (strcmp(key, "MaxPopulation") == 0) {
            m->MaxPopulation = bson_iter_int32(&iter);
        }

        if (strcmp(key, "FreePopulation") == 0) {
            m->FreePopulation = bson_iter_int32(&iter);
        }
    }
}

void bson_to_resources(const bson_t* doc, Resources* r){
    bson_iter_t iter;
    bson_iter_init(&iter, doc);

    while (bson_iter_next(&iter)) {
        const char* key = bson_iter_key(&iter);

        // lastUpdated
        if (strcmp(key, "lastUpdated") == 0) {
            const char* str = bson_iter_utf8(&iter, NULL);
            strncpy(r->lastUpdated, str, sizeof(r->lastUpdated));
            r->lastUpdated[sizeof(r->lastUpdated) - 1] = '\0';
            continue;
        }

        uint32_t len;
        const uint8_t* buf;

        bson_iter_document(&iter, &len, &buf);

        bson_t child;
        bson_init_static(&child, buf, len);

        if (strcmp(key, "Gold") == 0){bson_to_resource(&child, &r->Gold);}
        if (strcmp(key, "Stone") == 0){bson_to_resource(&child, &r->Stone);}
        if (strcmp(key, "Wood") == 0){bson_to_resource(&child, &r->Wood);}
        if (strcmp(key, "Political") == 0){bson_to_resource(&child, &r->Political);}
        if (strcmp(key, "Stability") == 0){bson_to_resource(&child, &r->Stability);}
        if (strcmp(key, "WarSupport") == 0){bson_to_resource(&child, &r->WarSupport);}

        if (strcmp(key, "ManPower") == 0){bson_to_manpower(&child, &r->ManPower);}

    }
}


//technologies
void bson_to_tech(const bson_t* doc, Tech* t){
    bson_iter_t iter;
    bson_iter_init(&iter, doc);

    while (bson_iter_next(&iter)) {
        const char* key = bson_iter_key(&iter);

        if (strcmp(key, "unlocked") == 0) {
            t->unlocked = bson_iter_bool(&iter);
        }

        if (strcmp(key, "Description") == 0) {
            const char* str = bson_iter_utf8(&iter, NULL);
            strncpy(t->Description, str, sizeof(t->Description));
            t->Description[sizeof(t->Description) - 1] = '\0';
        }
    }
}

void bson_to_technologies(const bson_t* doc, Technologies* t){
    bson_iter_t iter;
    bson_iter_init(&iter, doc);

    while (bson_iter_next(&iter)) {
        const char* key = bson_iter_key(&iter);

        uint32_t len;
        const uint8_t* buf;

        // All Tech fields are subdocuments
        bson_iter_document(&iter, &len, &buf);

        bson_t child;
        bson_init_static(&child, buf, len);

        if (strcmp(key, "Bows") == 0) bson_to_tech(&child, &t->Bows);
        if (strcmp(key, "Swords") == 0) bson_to_tech(&child, &t->Swords);
        if (strcmp(key, "Shields") == 0) bson_to_tech(&child, &t->Shields);
        if (strcmp(key, "Spears") == 0) bson_to_tech(&child, &t->Spears);
        if (strcmp(key, "LeatherArmour") == 0) bson_to_tech(&child, &t->LeatherArmour);
        if (strcmp(key, "BatteringRam") == 0) bson_to_tech(&child, &t->BatteringRam);
        if (strcmp(key, "WagonFort") == 0) bson_to_tech(&child, &t->WagonFort);
        if (strcmp(key, "WoodWall") == 0) bson_to_tech(&child, &t->WoodWall);
        if (strcmp(key, "StoneWall") == 0) bson_to_tech(&child, &t->StoneWall);
        if (strcmp(key, "WoodGate") == 0) bson_to_tech(&child, &t->WoodGate);
        if (strcmp(key, "StoneGate") == 0) bson_to_tech(&child, &t->StoneGate);
        if (strcmp(key, "WoodenTower") == 0) bson_to_tech(&child, &t->WoodenTower);
        if (strcmp(key, "StoneTower") == 0) bson_to_tech(&child, &t->StoneTower);
        if (strcmp(key, "WoodenKeep") == 0) bson_to_tech(&child, &t->WoodenKeep);
        if (strcmp(key, "StoneKeep") == 0) bson_to_tech(&child, &t->StoneKeep);
        if (strcmp(key, "WoodHouse") == 0) bson_to_tech(&child, &t->WoodHouse);
        if (strcmp(key, "StoneHouse") == 0) bson_to_tech(&child, &t->StoneHouse);
        if (strcmp(key, "Pavement") == 0) bson_to_tech(&child, &t->Pavement);

        if (strcmp(key, "CivilianFactory") == 0) bson_to_tech(&child, &t->CivilianFactory);
        if (strcmp(key, "MilitaryFactory") == 0) bson_to_tech(&child, &t->MilitaryFactory);
        if (strcmp(key, "Farm") == 0) bson_to_tech(&child, &t->Farm);
        if (strcmp(key, "Quarry") == 0) bson_to_tech(&child, &t->Quarry);
        if (strcmp(key, "LumberMill") == 0) bson_to_tech(&child, &t->LumberMill);
        if (strcmp(key, "Barracks") == 0) bson_to_tech(&child, &t->Barracks);
        if (strcmp(key, "Market") == 0) bson_to_tech(&child, &t->Market);
        if (strcmp(key, "TownHall") == 0) bson_to_tech(&child, &t->TownHall);
        if (strcmp(key, "Warehouse") == 0) bson_to_tech(&child, &t->Warehouse);

        if (strcmp(key, "ChainArmour") == 0) bson_to_tech(&child, &t->ChainArmour);
        if (strcmp(key, "PlateArmour") == 0) bson_to_tech(&child, &t->PlateArmour);
        if (strcmp(key, "Crossbows") == 0) bson_to_tech(&child, &t->Crossbows);
        if (strcmp(key, "Trebuchet") == 0) bson_to_tech(&child, &t->Trebuchet);
        if (strcmp(key, "Catapult") == 0) bson_to_tech(&child, &t->Catapult);
        if (strcmp(key, "Ballista") == 0) bson_to_tech(&child, &t->Ballista);

        if (strcmp(key, "StandardisedParts") == 0) bson_to_tech(&child, &t->StandardisedParts);
        if (strcmp(key, "RobustSupplyChains") == 0) bson_to_tech(&child, &t->RobustSupplyChains);
        if (strcmp(key, "WorkerShifts") == 0) bson_to_tech(&child, &t->WorkerShifts);
        if (strcmp(key, "FortifiedSettlements") == 0) bson_to_tech(&child, &t->FortifiedSettlements);
        if (strcmp(key, "CropRotation") == 0) bson_to_tech(&child, &t->CropRotation);
    }
}


// production info
void bson_to_PDetails(const bson_t* doc, PDetails* p){
    bson_iter_t iter;
    bson_iter_init(&iter, doc);

    while (bson_iter_next(&iter)) {
        const char* key = bson_iter_key(&iter);

        if (strcmp(key, "TotalMil") == 0){p->TotalMil = bson_iter_int32(&iter);}
        if (strcmp(key, "FreeMil") == 0){p->FreeMil = bson_iter_int32(&iter);}
        
        if (strcmp(key, "TotalCiv") == 0){p->TotalCiv = bson_iter_int32(&iter);}
        if (strcmp(key, "FreeCiv") == 0){p->FreeCiv = bson_iter_int32(&iter);}
        
    }
}


//bring it all together
void bson_to_user(const bson_t* doc, User* u){
    bson_iter_t iter;
    bson_iter_init(&iter, doc);

    while (bson_iter_next(&iter)) {
        const char* key = bson_iter_key(&iter);

        // Simple UTF‑8 string fields
        if (strcmp(key, "username") == 0) {
            const char* str = bson_iter_utf8(&iter, NULL);
            strncpy(u->username, str, sizeof(u->username));
            u->username[sizeof(u->username) - 1] = '\0';
            continue;
        }

        if (strcmp(key, "passwordHash") == 0) {
            const char* str = bson_iter_utf8(&iter, NULL);
            strncpy(u->passwordHash, str, sizeof(u->passwordHash));
            u->passwordHash[sizeof(u->passwordHash) - 1] = '\0';
            continue;
        }

        if (strcmp(key, "lastClaimDate") == 0) {
            const char* str = bson_iter_utf8(&iter, NULL);
            strncpy(u->lastClaimDate, str, sizeof(u->lastClaimDate));
            u->lastClaimDate[sizeof(u->lastClaimDate) - 1] = '\0';
            continue;
        }

        // Everything else is a subdocument
        uint32_t len;
        const uint8_t* buf;

        bson_iter_document(&iter, &len, &buf);

        bson_t child;
        bson_init_static(&child, buf, len);

        // Resources
        if (strcmp(key, "Resources") == 0) {
            bson_to_resources(&child, &u->Resources);
            continue;
        }

        // Technologies
        if (strcmp(key, "Technologies") == 0) {
            bson_to_technologies(&child, &u->Technologies);
            continue;
        }

        // Production Details
        if (strcmp(key, "PDetails") == 0) {
            bson_to_PDetails(&child, &u->ProdInfo);
            continue;
        }

        // Regimen Queue
        if (strcmp(key, "RegimenQueue") == 0) {
            bson_to_regimen_queue(&child, &u->regimens);
            continue;
        }

        // Training Regimens
        if (strcmp(key, "TrainingRegimens") == 0) {
            bson_to_regimen_training_list(&child, &u->regimenTrainingList);
            continue;
        }
    }
}

