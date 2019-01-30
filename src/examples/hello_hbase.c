/**
 * hello_hbase.c
 *   A sample using libhbase
 *
 * Build sample (see: build-hello_hbase.sh):
 *
 * gcc -o hello_hbase hello_hbase.c \
 *   -I../../target/libhbase-1.0-SNAPSHOT/include \
 *   -L../../target/libhbase-1.0-SNAPSHOT/lib/native -lhbase \
 *   -L/usr/local/java/jre/lib/amd64/server -ljvm \
 *   -lpthread \
 *   -lrt
 */
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#include <hbase/hbase.h>


/*
 * Sample code to illustrate usage of libhbase APIs
 */

#ifdef __cplusplus
extern  "C" {
#endif

// "<zk_host>:<zk_port>,<zk_host>:<zk_port>,..."
#define ZK_QUORUM    "localhost:2181"

int main (int argc, char *argv[])
{
    int rc = 0;

    hb_connection_t connection = NULL;
    hb_client_t client = NULL;
    hb_put_t put = NULL;

    printf("hello hbase start...\n");

    rc = hb_connection_create(ZK_QUORUM, 0, &connection);
    if (rc == 0) {
        printf("hb_connection_create success: %s\n", ZK_QUORUM);
    } else {
        printf("hb_connection_create error: %s\n", ZK_QUORUM);
        goto cleanup;
    }

    hb_client_create(connection, &client);
    if (rc == 0) {
        printf("hb_client_create error.\n");
    } else {
        goto cleanup;
    }

cleanup:
    if (client) {
        // hb_client_destroy(client, client_disconnection_callback, NULL);
        // wait_client_disconnection();
    }

    if (connection) {
        // hb_connection_destroy(connection);
    }

    return rc;
}

#ifdef __cplusplus
}
#endif
