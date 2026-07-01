#ifndef Cache_H
#define Cache_H   // these form a guard

#include <pthread.h>
#include "khash.h"
#include "Schema/UserBreakdown.h"
#include "Schema/TileSchema.h"


KHASH_MAP_INIT_STR(UserCache, User*);
KHASH_MAP_INIT_STR(TileCache, Tile*);

typedef struct {
    khash_t(UserCache)* Users;
    khash_t(TileCache)* Tiles;

    pthread_mutex_t lock;
} Cache;

extern Cache* GlobalCache;

Cache* cache_create(void);
void cache_free(Cache* c);

//user dealings
void cache_insert_user(Cache* c, User*);
User* cache_get_user(Cache* c, const char* username);
void cache_delete_user(Cache* c, const char* username);

//Tile dealings
void cache_insert_tile(Cache* c, const int x,const int y);
Tile* cache_get_tile(Cache* c, const int x,const int y);
void cache_delete_tile(Cache* c, const int x,const int y);

#endif