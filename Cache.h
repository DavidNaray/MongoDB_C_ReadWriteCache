#ifndef Cache_H
#define Cache_H   // these form a guard

#include <pthread.h>
#include <mongoc/mongoc.h>

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
extern mongoc_client_t *mongoClient;

Cache* cache_create(void);
void cache_free(Cache* c);

//user dealings
void cache_insert_user(Cache* c, User* user);
User* cache_get_user(Cache* c, const char* username);
void cache_delete_user(Cache* c, const char* username);

//Tile dealings
void cache_insert_tile(Cache* c, Tile* t);
Tile* cache_get_tile(Cache* c, const int x,const int y);
void cache_delete_tile(Cache* c, const int x,const int y);

#endif