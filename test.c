#include <mongoc/mongoc.h>

int main() {
    mongoc_init();

    mongoc_client_t* client = mongoc_client_new("mongodb://localhost:27017");

    bson_t cmd;
    bson_t reply;
    bson_error_t error;

    bson_init(&cmd);
    bson_append_int32(&cmd, "ping", 4, 1);

    bson_init(&reply);

    if (!mongoc_client_command_simple(client, "admin", &cmd, NULL, &reply, &error)) {
        printf("Ping failed: %s\n", error.message);
    } else {
        printf("Ping OK\n");
    }

    bson_destroy(&cmd);
    bson_destroy(&reply);

    mongoc_client_destroy(client);
    mongoc_cleanup();

    return 0;
}
