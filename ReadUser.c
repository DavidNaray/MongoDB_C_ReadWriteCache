#include "ReadUser.h"
#include <mongoc/mongoc.h>
#include <string.h>

void bson_to_unit(const bson_t* doc, Unit* u) {
    bson_iter_t iter;
    bson_iter_init(&iter, doc);

    while (bson_iter_next(&iter)) {
        const char* key = bson_iter_key(&iter);

        if (strcmp(key, "health") == 0)
            u->health = bson_iter_int32(&iter);

        if (strcmp(key, "tile") == 0) {
            bson_iter_t sub;
            bson_iter_recurse(&iter, &sub);
            while (bson_iter_next(&sub)) {
                const char* sk = bson_iter_key(&sub);
                if (strcmp(sk, "x") == 0) u->tile[0] = bson_iter_int32(&sub);
                if (strcmp(sk, "y") == 0) u->tile[1] = bson_iter_int32(&sub);
            }
        }

        if (strcmp(key, "position") == 0) {
            bson_iter_t sub;
            bson_iter_recurse(&iter, &sub);
            while (bson_iter_next(&sub)) {
                const char* sk = bson_iter_key(&sub);
                if (strcmp(sk, "x") == 0) u->position[0] = bson_iter_int32(&sub);
                if (strcmp(sk, "y") == 0) u->position[1] = bson_iter_int32(&sub);
            }
        }
    }
}