#include "ReadTile.h"
#include <mongoc/mongoc.h>
#include <string.h>

void bson_to_textures(const bson_t* doc, Textures* tex){
    bson_iter_t iter;
    bson_iter_init(&iter, doc);
    while (bson_iter_next(&iter)) {
        const char* key = bson_iter_key(&iter);

        if(strcmp(key, "heightmapUrl")==0){
            const char* str = bson_iter_utf8(&iter, NULL);
            strncpy(tex->heightmapUrl, str, sizeof(tex->heightmapUrl));
            tex->heightmapUrl[sizeof(tex->heightmapUrl) - 1] = '\0';
            continue;
        }

        if(strcmp(key, "texturemapUrl")==0){
            const char* str = bson_iter_utf8(&iter, NULL);
            strncpy(tex->texturemapUrl, str, sizeof(tex->texturemapUrl));
            tex->texturemapUrl[sizeof(tex->texturemapUrl) - 1] = '\0';
            continue;
        }

        if(strcmp(key, "WalkMapURL")==0){
            const char* str = bson_iter_utf8(&iter, NULL);
            strncpy(tex->WalkMapURL, str, sizeof(tex->WalkMapURL));
            tex->WalkMapURL[sizeof(tex->WalkMapURL) - 1] = '\0';
            continue;
        }
    }
}


void bson_to_UnitRef(const bson_t* doc, UnitRef* ur){
    bson_iter_t iter;
    bson_iter_init(&iter, doc);

    while (bson_iter_next(&iter)) {
        const char* key = bson_iter_key(&iter);
        
        if (strcmp(key, "regimenId") == 0) {ur->regimenId = bson_iter_int32(&iter);continue;}

        if (strcmp(key, "unitIndex") == 0) {ur->unitIndex = bson_iter_int32(&iter);continue;}

        if (strcmp(key, "active") == 0) {ur->active = bson_iter_bool(&iter);continue;}

        if (strcmp(key, "username") == 0) {
            const char* str = bson_iter_utf8(&iter, NULL);
            strncpy(ur->username, str, sizeof(ur->username));
            ur->username[sizeof(ur->username) - 1] = '\0';
            continue;
        }
    }

}

void bson_to_UnitQueue(const bson_t* doc, UnitQueue* uq){
    bson_iter_t iter;
    bson_iter_init(&iter, doc);

    uq->references = NULL;
    uq->count = 0;
    uq->capacity = 0;

    while (bson_iter_next(&iter)) {
        const char* key = bson_iter_key(&iter);

        if (strcmp(key, "count") == 0) {uq->count = bson_iter_int32(&iter);continue;}

        if (strcmp(key, "capacity") == 0) {uq->capacity = bson_iter_int32(&iter);continue;}

        if (strcmp(key, "references") == 0) {

            uq->references = malloc(sizeof(UnitRef*) * uq->capacity);

            for (int i = 0; i < uq->capacity; i++) {uq->references[i] = NULL;}

            bson_iter_t arr;
            bson_iter_recurse(&iter, &arr);

            while (bson_iter_next(&arr)) {
                uint32_t len;
                const uint8_t* buf;

                bson_iter_document(&arr, &len, &buf);

                bson_t child;
                bson_init_static(&child, buf, len);

                int idx = atoi(bson_iter_key(&arr));

                UnitRef* ur = malloc(sizeof(UnitRef));
                bson_to_UnitRef(&child,ur);

                uq->references[idx] = ur;
            }
        }
    }
}


//bring it together
void bson_to_tile(const bson_t* doc, Tile* t){
    bson_iter_t iter;
    bson_iter_init(&iter, doc);

    while (bson_iter_next(&iter)) {
        const char* key = bson_iter_key(&iter);

        if (strcmp(key, "x") == 0){t->x = bson_iter_int32(&iter);continue;}

        if (strcmp(key, "y") == 0){t->y = bson_iter_int32(&iter);continue;}

        if (strcmp(key, "lastUpdate") == 0) {
            const char* str = bson_iter_utf8(&iter, NULL);
            strncpy(t->lastUpdate, str, sizeof(t->lastUpdate));
            t->lastUpdate[sizeof(t->lastUpdate) - 1] = '\0';
            continue;
        }

        // if (strcmp(key, "count") == 0) {t->count = bson_iter_int32(&iter);continue;}


        //everything else
        uint32_t len;
        const uint8_t* buf;

        bson_iter_document(&iter, &len, &buf);

        bson_t child;
        bson_init_static(&child, buf, len);

        if (strcmp(key, "textures") == 0) {bson_to_textures(&child,&t->textures);continue;}

        if (strcmp(key, "units") == 0) {bson_to_UnitQueue(&child,&t->unitReferences);continue;}
    }
}

