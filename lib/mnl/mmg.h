#ifndef __MMG_H__
#define __MMG_H__

#include "mheads.h"

__BEGIN_DECLS

typedef struct _mmg_conn {
    mongo_sync_connection *con;
    bson *docq;
    bson *docs;
    mongo_packet *p;
    mongo_sync_cursor *c;
    NEOERR* (*query_callback)(struct _mmg_conn *db, HDF *node, bool lastone);
    int flags;
    int skip;
    int limit;
    int rescount;
    bool incallback;
    void *callbackdata;
} mmg_conn;

/*
 * timeout in microsecond, 1000ms = 1s
 */
NEOERR* mmg_init(char *host, int port, int timeout, mmg_conn **db);
NEOERR* mmg_auth(mmg_conn *db, char *dsn, char *user, char *pass);
NEOERR* mmg_seed_add(mmg_conn *db, char *host, int port);
void mmg_destroy(mmg_conn *db);

enum {
    MMG_FLAG_Z = 0,
    /* allow entry result. don't return error if no result found */
    MMG_FLAG_EMPTY = 1 << 11,
    /* don't add number as key for each result row */
    MMG_FLAG_MIXROWS = 1 << 12,
    /* we need mongo's _id */
    MMG_FLAG_GETOID = 1 << 13,
    /* wrap node by $set on hdf_update */
    MMG_FLAG_UPSET = 1 << 14,
};
/*
 * flags: reused flags with mongo_wire_cmd_query().
 *        so, MMG_FLAG_XX start with 1 << 11
 * querys: query condition json object
 *         it can be just "{'obj_id': 10, 'user_id', 10}"
 *         or more complex like this:
 *         "{'$where': 'this.start_date+this.start_time > %ld'}"
 *         "{'$query': {'user_id': %d}, '$orderby': {'user_integral_id': 1}}"
 *
 * qcbk: the query callback on each result document.
 * ATTENTION: we can query recursive, but the callback won't call recursive.
 *            we just call the first NOTNULL qcbk
 *            because following prepare overwrite the formal's callback will coredump
 *            so, later prepare's callback won't be remember in db
 */
NEOERR* mmg_prepare(mmg_conn *db, int flags, int skip, int limit,
                    NEOERR* (*qcbk)(mmg_conn *db, HDF *node, bool lastone),
                    char *sels, char *querys);
NEOERR* mmg_preparef(mmg_conn *db, int flags, int skip, int limit,
                     NEOERR* (*qcbk)(mmg_conn *db, HDF *node, bool lastone),
                     char *sels, char *qfmt, ...)
                     ATTRIBUTE_PRINTF(7, 8);
void mmg_set_callbackdata(mmg_conn *db, void *data);

/*
 * dsn is the namespace, the database and collection name
 */
NEOERR* mmg_query(mmg_conn *db, char *dsn, char *prefix, HDF *outnode);

NEOERR* mmg_string_insert(mmg_conn *db, char *dsn, char *str);
NEOERR* mmg_string_insertf(mmg_conn *db, char *dsn, char *fmt, ...)
                           ATTRIBUTE_PRINTF(3, 4);
NEOERR* mmg_hdf_insert(mmg_conn *db, char *dsn, HDF *node);
/*
 * limit insert node(securer)
 * all key from lnode's key, value from node
 * with a little config syntax, refer the code
 */
NEOERR* mmg_hdf_insertl(mmg_conn *db, char *dsn, HDF *node, HDF *lnode);

NEOERR* mmg_string_update(mmg_conn *db, char *dsn, int flags, char *up, char *sel);
NEOERR* mmg_string_updatef(mmg_conn *db, char *dsn, int flags, char *up, char *selfmt, ...)
                           ATTRIBUTE_PRINTF(5, 6);
NEOERR* mmg_hdf_update(mmg_conn *db, char *dsn, int flags, HDF *node, char *sel);
NEOERR* mmg_hdf_updatef(mmg_conn *db, char *dsn, int flags, HDF *node, char *selfmt, ...)
                        ATTRIBUTE_PRINTF(5, 6);
/* same as insertl */
NEOERR* mmg_hdf_updatefl(mmg_conn *db, char *dsn, int flags, HDF *node, HDF *lnode,
                         char *selfmt, ...)
                         ATTRIBUTE_PRINTF(6, 7);

NEOERR* mmg_count(mmg_conn *db, char *dbname, char *collname, int *ret, char *querys);
NEOERR* mmg_countf(mmg_conn *db, char *dbname, char *collname, int *ret, char *qfmt, ...)
                   ATTRIBUTE_PRINTF(5, 6);

NEOERR* mmg_delete(mmg_conn *db, char *dsn, int flags, char *sel);
NEOERR* mmg_deletef(mmg_conn *db, char *dsn, int flags, char *selfmt, ...)
                   ATTRIBUTE_PRINTF(4, 5);

NEOERR* mmg_custom(mmg_conn *db, char *dbname,
                   char *prefix, HDF *outnode, char *command);
NEOERR* mmg_customf(mmg_conn *db, char *dbname,
                    char *prefix, HDF *outnode, char *cmdfmt, ...)
                    ATTRIBUTE_PRINTF(5, 6);

/*
 * get one row's string value
 */
char* mmg_get_valuef(mmg_conn *db, char *dsn, char *key, int skip, char *qfmt, ...)
                     ATTRIBUTE_PRINTF(5, 6);

/*
 * get servral row's int value, and add them
 */
int mmg_get_int_valuef(mmg_conn *db, char *dsn, char *key, int skip, int limit,
                       char *qfmt, ...)
                       ATTRIBUTE_PRINTF(6, 7);


#define MMG_SET_NTT(hdf, key, db, dbname, collname, selfmt, ...)        \
    do {                                                                \
        int zinta;                                                      \
        err = mmg_countf(db, dbname, collname, &zinta, selfmt, ##__VA_ARGS__); \
        if (err != STATUS_OK) return nerr_pass(err);                    \
        hdf_set_int_value(hdf, key, zinta);                             \
    } while (0)


__END_DECLS
#endif    /* __MMG_H__ */
