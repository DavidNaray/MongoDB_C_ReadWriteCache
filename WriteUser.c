#include "WriteUser.h"
#include <mongoc/mongoc.h>
#include <string.h>


//trained regimens
void unit_to_bson(const Unit* u, bson_t* parent) {
    BSON_APPEND_INT32(parent, "health", u->health);

    bson_t tile;
    BSON_APPEND_DOCUMENT_BEGIN(parent, "tile", &tile);
    BSON_APPEND_INT32(&tile, "x", u->tile[0]);
    BSON_APPEND_INT32(&tile, "y", u->tile[1]);
    bson_append_document_end(parent, &tile);

    bson_t pos;
    BSON_APPEND_DOCUMENT_BEGIN(parent, "position", &pos);
    BSON_APPEND_INT32(&pos, "x", u->position[0]);
    BSON_APPEND_INT32(&pos, "y", u->position[1]);
    bson_append_document_end(parent, &pos);
}

void unit_block_to_bson(const UnitBlock* ub, bson_t* parent) {
    BSON_APPEND_INT32(parent, "count", ub->count);

    bson_t arr;
    BSON_APPEND_ARRAY_BEGIN(parent, "individuals", &arr);

    char key[16];
    uint32_t len;
    for (int i = 0; i < ub->count; i++) {
        bson_t udoc;
        bson_uint32_to_string(i, key,&len, sizeof(key));
        BSON_APPEND_DOCUMENT_BEGIN(&arr, key, &udoc);

        unit_to_bson(&ub->individuals[i], &udoc);

        bson_append_document_end(&arr, &udoc);
    }

    bson_append_array_end(parent, &arr);
}

void regimen_to_bson(const Regimen* r, bson_t* parent) {
    BSON_APPEND_INT32(parent, "id", r->id);

    bson_t arr;
    BSON_APPEND_ARRAY_BEGIN(parent, "units", &arr);

    char key[16];//buffer of 16 characters for keys
    uint32_t len;
    for (int i = 0; i < UNIT_MAX; i++) {
        bson_t ubdoc;
        bson_uint32_to_string(i, key,&len, sizeof(key));
        BSON_APPEND_DOCUMENT_BEGIN(&arr, key, &ubdoc);

        if (r->units[i] != NULL){unit_block_to_bson(r->units[i], &ubdoc);}

        bson_append_document_end(&arr, &ubdoc);
    }

    bson_append_array_end(parent, &arr);
}

void regimen_queue_to_bson(const RegimenQueue* q, bson_t* parent) {
    BSON_APPEND_INT32(parent, "count", q->count);
    BSON_APPEND_INT32(parent, "capacity", q->capacity);

    bson_t arr;
    BSON_APPEND_ARRAY_BEGIN(parent, "regimens", &arr);

    char key[16];
    uint32_t len;
    for (int i = 0; i < q->count; i++) {
        bson_t rdoc;
        bson_uint32_to_string(i, key,&len, sizeof(key));
        BSON_APPEND_DOCUMENT_BEGIN(&arr, key, &rdoc);

        regimen_to_bson(q->regimens[i], &rdoc);

        bson_append_document_end(&arr, &rdoc);
    }

    bson_append_array_end(parent, &arr);
}


//in training regimens
void unit_training_to_bson(bson_t* parent, const UnitTraining* ut){
    BSON_APPEND_INT32(parent, "count", ut->count);
    BSON_APPEND_INT32(parent, "progress", ut->progress);
    BSON_APPEND_INT32(parent, "finish", ut->finish);
}

void regimen_training_to_bson(const RegimenTraining* rt, bson_t* parent){
    // units array
    bson_t unitsArr;
    BSON_APPEND_ARRAY_BEGIN(parent, "units", &unitsArr);

    char key[16];
    uint32_t len;

    for (int i = 0; i < UNIT_MAX; i++){
        bson_t utDoc;
        bson_uint32_to_string(i, key, &len, sizeof(key));
        BSON_APPEND_DOCUMENT_BEGIN(&unitsArr, key, &utDoc);

        unit_training_to_bson(&utDoc, &rt->units[i]);

        bson_append_document_end(&unitsArr, &utDoc);
    }

    bson_append_array_end(parent, &unitsArr);

    // deployTile
    bson_t tileDoc;
    BSON_APPEND_DOCUMENT_BEGIN(parent, "deployTile", &tileDoc);
    BSON_APPEND_INT32(&tileDoc, "x", rt->deployTile[0]);
    BSON_APPEND_INT32(&tileDoc, "y", rt->deployTile[1]);
    bson_append_document_end(parent, &tileDoc);

    // deployPixel
    bson_t pixelDoc;
    BSON_APPEND_DOCUMENT_BEGIN(parent, "deployPixel", &pixelDoc);
    BSON_APPEND_INT32(&pixelDoc, "x", rt->deployPixel[0]);
    BSON_APPEND_INT32(&pixelDoc, "y", rt->deployPixel[1]);
    bson_append_document_end(parent, &pixelDoc);

    BSON_APPEND_BOOL(parent, "active", rt->active);
}

void regimen_training_list_to_bson(const RegimenTrainingList* list, bson_t* parent){
    bson_t arr;
    BSON_APPEND_ARRAY_BEGIN(parent, "regimens", &arr);

    char key[16];
    uint32_t len;

    for (int i = 0; i < MAX_REGIMENS; i++){
        bson_t rtDoc;
        bson_uint32_to_string(i, key, &len, sizeof(key));
        BSON_APPEND_DOCUMENT_BEGIN(&arr, key, &rtDoc);

        regimen_training_to_bson(&list->regimens[i], &rtDoc);

        bson_append_document_end(&arr, &rtDoc);
    }

    bson_append_array_end(parent, &arr);
}


//technologies
void tech_to_bson(bson_t* parent, const char* key, const Tech* t){
    bson_t doc;
    BSON_APPEND_DOCUMENT_BEGIN(parent, key, &doc);

    BSON_APPEND_BOOL(&doc, "unlocked", t->unlocked);
    BSON_APPEND_UTF8(&doc, "Description", t->Description);

    bson_append_document_end(parent, &doc);
}

void technologies_to_bson(const Technologies* t, bson_t* parent){
    tech_to_bson(parent, "Bows", &t->Bows);
    tech_to_bson(parent, "Swords", &t->Swords);
    tech_to_bson(parent, "Shields", &t->Shields);
    tech_to_bson(parent, "Spears", &t->Spears);
    tech_to_bson(parent, "LeatherArmour", &t->LeatherArmour);
    tech_to_bson(parent, "BatteringRam", &t->BatteringRam);
    tech_to_bson(parent, "WagonFort", &t->WagonFort);
    tech_to_bson(parent, "WoodWall", &t->WoodWall);
    tech_to_bson(parent, "StoneWall", &t->StoneWall);
    tech_to_bson(parent, "WoodGate", &t->WoodGate);
    tech_to_bson(parent, "StoneGate", &t->StoneGate);
    tech_to_bson(parent, "WoodenTower", &t->WoodenTower);
    tech_to_bson(parent, "StoneTower", &t->StoneTower);
    tech_to_bson(parent, "WoodenKeep", &t->WoodenKeep);
    tech_to_bson(parent, "StoneKeep", &t->StoneKeep);
    tech_to_bson(parent, "WoodHouse", &t->WoodHouse);
    tech_to_bson(parent, "StoneHouse", &t->StoneHouse);
    tech_to_bson(parent, "Pavement", &t->Pavement);

    tech_to_bson(parent, "CivilianFactory", &t->CivilianFactory);
    tech_to_bson(parent, "MilitaryFactory", &t->MilitaryFactory);
    tech_to_bson(parent, "Farm", &t->Farm);
    tech_to_bson(parent, "Quarry", &t->Quarry);
    tech_to_bson(parent, "LumberMill", &t->LumberMill);
    tech_to_bson(parent, "Barracks", &t->Barracks);
    tech_to_bson(parent, "Market", &t->Market);
    tech_to_bson(parent, "TownHall", &t->TownHall);
    tech_to_bson(parent, "Warehouse", &t->Warehouse);

    tech_to_bson(parent, "ChainArmour", &t->ChainArmour);
    tech_to_bson(parent, "PlateArmour", &t->PlateArmour);
    tech_to_bson(parent, "Crossbows", &t->Crossbows);
    tech_to_bson(parent, "Trebuchet", &t->Trebuchet);
    tech_to_bson(parent, "Catapult", &t->Catapult);
    tech_to_bson(parent, "Ballista", &t->Ballista);

    tech_to_bson(parent, "StandardisedParts", &t->StandardisedParts);
    tech_to_bson(parent, "RobustSupplyChains", &t->RobustSupplyChains);
    tech_to_bson(parent, "WorkerShifts", &t->WorkerShifts);
    tech_to_bson(parent, "FortifiedSettlements", &t->FortifiedSettlements);
    tech_to_bson(parent, "CropRotation", &t->CropRotation);
}


//resources
void resource_to_bson(bson_t* parent, const char* key, const Resource* r){
    bson_t doc;
    BSON_APPEND_DOCUMENT_BEGIN(parent, key, &doc);

    BSON_APPEND_INT32(&doc, "Total", r->Total);
    BSON_APPEND_DOUBLE(&doc, "Rate", r->Rate);

    bson_append_document_end(parent, &doc);
}

void manpower_to_bson(bson_t* parent, const char* key, const ResourceManPower* m){
    bson_t doc;
    BSON_APPEND_DOCUMENT_BEGIN(parent, key, &doc);

    BSON_APPEND_INT32(&doc, "TotalManPower", m->TotalManPower);
    BSON_APPEND_INT32(&doc, "TotalPopulation", m->TotalPopulation);//int32 for int
    BSON_APPEND_DOUBLE(&doc, "Rate", m->Rate);//double for floating point
    BSON_APPEND_DOUBLE(&doc, "RecruitableFactor", m->RecruitableFactor);
    BSON_APPEND_INT32(&doc, "MaxPopulation", m->MaxPopulation);
    BSON_APPEND_INT32(&doc, "FreePopulation", m->FreePopulation);

    bson_append_document_end(parent, &doc);
}

void resources_to_bson(const Resources* r, bson_t* parent){
    resource_to_bson(parent, "Gold", &r->Gold);
    resource_to_bson(parent, "Stone", &r->Stone);
    resource_to_bson(parent, "Wood", &r->Wood);
    resource_to_bson(parent, "Political", &r->Political);
    resource_to_bson(parent, "Stability", &r->Stability);
    resource_to_bson(parent, "WarSupport", &r->WarSupport);

    manpower_to_bson(parent, "ManPower", &r->ManPower);

    BSON_APPEND_UTF8(parent, "lastUpdated", r->lastUpdated);
}


//production info
void PDetails_to_bson(const PDetails* p, bson_t* parent){
    BSON_APPEND_INT32(parent, "TotalMil", p->TotalMil);
    BSON_APPEND_INT32(parent, "FreeMil", p->FreeMil);

    BSON_APPEND_INT32(parent, "TotalCiv", p->TotalCiv);
    BSON_APPEND_INT32(parent, "FreeCiv", p->FreeCiv);
}


//bring it together
bson_t* user_to_bson(const User* u){
    bson_t* doc = bson_new();

    // Basic identity fields
    BSON_APPEND_UTF8(doc, "username", u->username);
    BSON_APPEND_UTF8(doc, "passwordHash", u->passwordHash);
    BSON_APPEND_UTF8(doc, "lastClaimDate", u->lastClaimDate);

    // Resources
    bson_t resourcesDoc;
    BSON_APPEND_DOCUMENT_BEGIN(doc, "Resources", &resourcesDoc);
    resources_to_bson(&u->Resources, &resourcesDoc);
    bson_append_document_end(doc, &resourcesDoc);

    // Technologies
    bson_t techDoc;
    BSON_APPEND_DOCUMENT_BEGIN(doc, "Technologies", &techDoc);
    technologies_to_bson(&u->Technologies, &techDoc);
    bson_append_document_end(doc, &techDoc);

    // Production details
    bson_t pdetailsDoc;
    BSON_APPEND_DOCUMENT_BEGIN(doc, "PDetails", &pdetailsDoc);
    PDetails_to_bson(&u->ProdInfo, &pdetailsDoc);
    bson_append_document_end(doc, &pdetailsDoc);

    // Combat regimens
    bson_t regimenQueueDoc;
    BSON_APPEND_DOCUMENT_BEGIN(doc, "RegimenQueue", &regimenQueueDoc);
    regimen_queue_to_bson(&u->regimens, &regimenQueueDoc);
    bson_append_document_end(doc, &regimenQueueDoc);

    // Training regimens
    bson_t trainingListDoc;
    BSON_APPEND_DOCUMENT_BEGIN(doc, "TrainingRegimens", &trainingListDoc);
    regimen_training_list_to_bson(&u->regimenTrainingList, &trainingListDoc);
    bson_append_document_end(doc, &trainingListDoc);

    return doc;
}
