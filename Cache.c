#include "Cache.h"

Cache* GlobalCache = NULL;

Cache* cache_create(void){
    Cache* c = malloc(sizeof(Cache));
    c->Users = kh_init(UserCache);
    return c;
};

void cache_free(Cache* c){
    if (!c) return;
    kh_destroy(UserCache, c->Users);
    free(c);
};

//user dealings
void cache_insert_user(Cache* c, User* user){
    int ret;
    khiter_t k = kh_put(UserCache, c->Users, user->username, &ret);
    kh_val(c->Users, k) = user;
};

User* cache_get_user(Cache* c, const char* username){
    khiter_t k = kh_get(UserCache, c->Users, username);
    if (k == kh_end(c->Users)){return NULL;}

    return kh_val(c->Users, k);
};

void cache_delete_user(Cache* c, const char* username){
    khiter_t k = kh_get(UserCache, c->Users, username);
    
    if (k != kh_end(c->Users)) {kh_del(UserCache, c->Users, k);}
};