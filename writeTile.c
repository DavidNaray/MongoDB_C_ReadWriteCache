#include "WriteTile.h"
#include <mongoc/mongoc.h>
#include <string.h>

void textures_to_bson(const Textures* tex, bson_t* parent){
    BSON_APPEND_UTF8(parent, "heightmapUrl", tex->heightmapUrl);
    BSON_APPEND_UTF8(parent, "texturemapUrl", tex->texturemapUrl);
    BSON_APPEND_UTF8(parent, "WalkMapURL", tex->WalkMapURL);
}


void UnitRef_to_bson(bson_t* parent, const UnitRef* ur){
    BSON_APPEND_UTF8(parent, "username", ur->username);
    BSON_APPEND_INT32(parent, "regimenId", ur->regimenId);
    BSON_APPEND_INT32(parent, "unitIndex", ur->unitIndex);
    BSON_APPEND_BOOL(parent, "active", ur->active);
}

void UnitQueue_to_bson(bson_t* parent, const UnitQueue* uq){
    BSON_APPEND_INT32(parent, "count", uq->count);
    BSON_APPEND_INT32(parent, "capacity", uq->capacity);

    bson_t refs;
    BSON_APPEND_DOCUMENT_BEGIN(parent, "references", &refs);

    for (int i = 0; i < uq->capacity; i++) {
        UnitRef* ur = uq->references[i];
        if (ur == NULL) continue; // ie NULL

        bson_t child;
        char key[16];
        uint32_t len;
        // snprintf(idxStr, sizeof(idxStr), "%d", i);
        bson_uint32_to_string(i, key, &len, sizeof(key));

        BSON_APPEND_DOCUMENT_BEGIN(&refs, key, &child);
        UnitRef_to_bson(&child, ur);
        bson_append_document_end(&refs, &child);
    }

    bson_append_document_end(parent, &refs);
}


//bring it together
bson_t* tile_to_bson(const Tile* t){
    bson_t* doc = bson_new();

    BSON_APPEND_INT32(doc, "x", t->x);
    BSON_APPEND_INT32(doc, "y", t->y);

    BSON_APPEND_UTF8(doc, "lastUpdate", t->lastUpdate);

    bson_t texturesDoc;
    BSON_APPEND_DOCUMENT_BEGIN(doc, "textures", &texturesDoc);
    textures_to_bson(&t->textures,&texturesDoc);
    bson_append_document_end(doc, &texturesDoc);

    bson_t unitReferencesDoc;
    BSON_APPEND_DOCUMENT_BEGIN(doc, "unitReferences", &unitReferencesDoc);
    UnitQueue_to_bson(&unitReferencesDoc,&t->unitReferences);
    bson_append_document_end(doc, &unitReferencesDoc);

}