#include "WriteUser.h"
#include <mongoc/mongoc.h>
#include <string.h>

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

