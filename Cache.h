#ifndef Cache_H
#define Cache_H   // these form a guard

#include <pthread.h>
#include "khash.h"
#include "UserBreakdown.h"


KHASH_MAP_INIT_STR(UserCache, User*);

typedef struct {
    khash_t(UserCache)* Users;

    pthread_mutex_t lock;
} Cache;

extern Cache* GlobalCache;

Cache* cache_create(void);
void cache_free(Cache* c);

//user dealings
void cache_insert_user(Cache* c, User*);
User* cache_get_user(Cache* c, const char* username);
void cache_delete_user(Cache* c, const char* username);

#endif