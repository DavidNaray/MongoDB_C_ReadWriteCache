#ifndef ReadTile_H
#define ReadTile_H   // these form a guard

#include "Schema/TileSchema.h"
#include <bson/bson.h>

void bson_to_tile(const bson_t* doc, Tile* t);


void bson_to_textures(const bson_t* doc, Textures* tex);


void bson_to_UnitRef(const bson_t* doc, UnitRef* ur);
void bson_to_UnitQueue(const bson_t* doc, UnitQueue* uq);

#endif