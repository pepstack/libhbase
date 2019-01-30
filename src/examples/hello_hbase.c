/**
 * hello_hbase.c
 *   A sample using libhbase: 同步方式, 测试 api 是否可用
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

#define HTABLE_NAME  "libhbase_test"

/* "ha08.ztgame.com:2181,ha07.ztgame.com:2181,ha06.ztgame.com:2181" */
#define ZK_QUORUM    "localhost:2181"


static int ensure_hbase_table (hb_connection_t connection, const char *table_name)
{
    int32_t retCode = -1;
    hb_admin_t admin = NULL;

    if ((retCode = hb_admin_create(connection, &admin)) != 0) {
        printf("hb_admin_create error.\n");
        goto cleanup;
    }

    if ((retCode = hb_admin_table_exists(admin, NULL, table_name)) == 0) {
        printf("Table '%s' exists\n", table_name);
    } else if (retCode != ENOENT) {
        printf("Table '%s' not exists\n", table_name);
        goto cleanup;
    }

    retCode = hb_admin_table_enabled(admin, NULL, table_name);
    if (retCode != 0) {
        printf("hb_admin_table_enabled error.\n");
        goto cleanup;
    }

cleanup:

    if (admin) {
        hb_admin_destroy(admin, NULL, NULL);
    }

    return retCode;
}


static void hbase_put_cell (hb_client_t client)
{
    char rowkey[256];
    size_t rowkeylen;

    char family[] = "f1";
    char column[] = "author";
    char value[] = "zhang";

    hb_put_t put = NULL;
    hb_cell_t *cell = NULL;

    rowkeylen = snprintf(rowkey, sizeof(rowkey), "%s", "rowkey_2");

    hb_put_create(rowkey, rowkeylen, &put);

    hb_mutation_set_table(put, HTABLE_NAME, strlen(HTABLE_NAME));
    hb_mutation_set_durability(put, DURABILITY_SYNC_WAL);

    cell = (hb_cell_t *) calloc(1, sizeof(hb_cell_t));

    cell->row = rowkey;
    cell->row_len = rowkeylen;

    cell->family = family;
    cell->family_len = strlen(family);

    cell->qualifier = column;
    cell->qualifier_len = strlen(column);

    cell->value = value;
    cell->value_len = strlen(value);

    cell->ts = 1391111111111L;

    hb_put_add_cell(put, cell);

    hb_mutation_send(client, put, NULL, NULL);

    sleep(1);

    hb_client_flush(client, NULL, NULL);

    sleep(1);

    free(cell);
}


static void hbase_get_cell (hb_client_t client)
{
    char rowkey[256];
    size_t keylen;
   
    hb_get_t get = NULL;

    keylen = snprintf(rowkey, sizeof(rowkey), "%s", "rowkey001");

    hb_get_create(rowkey, keylen, &get);

    hb_get_add_column(get, "f1", 2, NULL, 0);

    hb_get_set_table(get, HTABLE_NAME, strlen(HTABLE_NAME));

    /* up to ten versions of each column */
    hb_get_set_num_versions(get, 10);

    hb_get_send(client, get, NULL, NULL);

    sleep(1);

    hb_get_destroy(get);
}


int main (int argc, char *argv[])
{
    int rc = 0;

    hb_connection_t connection = NULL;
    hb_client_t client = NULL;
    hb_put_t put = NULL;

    printf("hello hbase start...\n");

    if (argc > 1) {
        printf("ZK_QUORUM='%s'\n", argv[1]);

        rc = hb_connection_create(argv[1], 0, &connection);
    } else {
        printf("ZK_QUORUM='%s'\n", ZK_QUORUM);

        rc = hb_connection_create(ZK_QUORUM, 0, &connection);
    }

    if (rc == 0) {
        printf("hb_connection_create success.\n");
    } else {
        printf("hb_connection_create error.\n");
        goto cleanup;
    }

    ensure_hbase_table(connection, HTABLE_NAME);

    hb_client_create(connection, &client);
    if (rc == 0) {
        printf("hb_client_create success.\n");
    } else {
        printf("hb_client_create error.\n");
        goto cleanup;
    }

    hbase_get_cell(client);

    hbase_put_cell(client);

    sleep(1);

cleanup:
    if (client) {
        hb_client_destroy(client, NULL, NULL);
    }

    if (connection) {
        hb_connection_destroy(connection);
    }

    return rc;
}

#ifdef __cplusplus
}
#endif
