#include "Cache.h"

Cache *GlobalCache = NULL;
mongoc_client_t *mongoClient = NULL;

Cache* cache_create(void){
    Cache* c = malloc(sizeof(Cache));
    c->Users = kh_init(UserCache);
    c->Tiles = kh_init(TileCache);
    pthread_mutex_init(&c->lock, NULL);
    return c;
};

void cache_free(Cache* c){
    if (!c) return;
    kh_destroy(UserCache, c->Users);
    kh_destroy(TileCache, c->Tiles);
    pthread_mutex_destroy(&c->lock);
    free(c);
};


//user dealings
void cache_insert_user(Cache* c, User* user){
    // pthread_mutex_lock(&c->lock);

    int ret;
    khiter_t k = kh_put(UserCache, c->Users, user->username, &ret);
    kh_val(c->Users, k) = user;
    
    // pthread_mutex_unlock(&c->lock);
};

User* cache_get_user(Cache* c, const char* username){
    // pthread_mutex_lock(&c->lock);

    khiter_t k = kh_get(UserCache, c->Users, username);
    if (k == kh_end(c->Users)){return NULL;}

    // pthread_mutex_unlock(&c->lock);

    return kh_val(c->Users, k);
};

void cache_delete_user(Cache* c, const char* username){
    // pthread_mutex_lock(&c->lock);

    khiter_t k = kh_get(UserCache, c->Users, username);
    
    if (k != kh_end(c->Users)) {kh_del(UserCache, c->Users, k);}

    // pthread_mutex_unlock(&c->lock);
};


//tile dealings
void cache_insert_tile(Cache* c, Tile* t){
    // pthread_mutex_lock(&c->lock);
    // char key[2]={t->x,t->y};
    char key[32];
    snprintf(key, sizeof(key), "%d,%d", t->x, t->y);

    int ret;
    khiter_t k = kh_put(TileCache, c->Tiles, key, &ret);
    kh_val(c->Tiles, k) = t;

    // pthread_mutex_unlock(&c->lock);
}

Tile* cache_get_tile(Cache* c, const int x,const int y){
    // pthread_mutex_lock(&c->lock);
    // char key[2]={x,y};
    char key[32];
    snprintf(key, sizeof(key), "%d,%d", x, y);

    khiter_t k = kh_get(TileCache, c->Tiles, key);
    if (k == kh_end(c->Tiles)){return NULL;}

    // pthread_mutex_unlock(&c->lock);

    return kh_val(c->Tiles, k);
}

void cache_delete_tile(Cache* c, const int x,const int y){
    // pthread_mutex_lock(&c->lock);
    // char key[2]={x,y};
    char key[32];
    snprintf(key, sizeof(key), "%d,%d", x, y);

    khiter_t k = kh_get(TileCache, c->Tiles, key);
    
    if (k != kh_end(c->Tiles)) {kh_del(TileCache, c->Tiles, k);}

    // pthread_mutex_unlock(&c->lock);
}