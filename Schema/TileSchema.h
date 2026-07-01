#ifndef TileSchema_H
#define TileSchema_H   // these form a guard

#include <stdbool.h>

typedef struct {
    // int userId;
    char username[256];
    int regimenId;
    int unitIndex; // index inside individuals[] (from unitblock in users)

    //active flag to avoid constant need to resize the array
    bool active;   // determines if slot is still valid or if it can be updated with new unit ref
} UnitRef;

typedef struct {
    UnitRef** references;
    int count;//how many units in tile for unitref that are active
    int capacity;//active+ nonactive

} UnitQueue;


typedef struct {
    char heightmapUrl[64];
    char texturemapUrl[64];
    char WalkMapURL[64];

} Textures;

// typedef struct {

// } AbstractMap;

typedef struct {
    int  x;
    int  y;

    char lastUpdate[64];

    Textures textures;

    // AbstractMap abstractMap;
    
    UnitQueue unitReferences;//array of references for the unit information
} Tile;

#endif