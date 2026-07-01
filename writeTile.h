#ifndef WriteTile_H
#define WriteTile_H   // these form a guard

#include "Schema/TileSchema.h"
#include <bson/bson.h>

bson_t* tile_to_bson(const Tile* t);


void textures_to_bson(const Textures* tex, bson_t* parent);


void UnitRef_to_bson(bson_t* parent, const UnitRef* ur);
void UnitQueue_to_bson(bson_t* parent, const UnitQueue* uq);

#endif