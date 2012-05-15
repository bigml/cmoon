#include "mheads.h"
#include "lheads.h"
#include "oadmin.h"

NEOERR* admin_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mdb_conn *db = hash_lookup(dbh, "main");
    char *mname;
    NEOERR *err;

    MEMBER_CHECK_ADMIN();

    SET_ADMIN_ACTION();

    return STATUS_OK;
}

NEOERR* admin_paper_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mdb_conn *db = hash_lookup(dbh, "main");
    int count, offset;
    char *mname;
    NEOERR *err;

    MEMBER_CHECK_ADMIN();

    SET_ADMIN_ACTION();

    mdb_pagediv(cgi->hdf, PRE_QUERY, &count, &offset, PRE_OUTPUT, cgi->hdf);

    MDB_PAGEDIV_SET(cgi->hdf, PRE_OUTPUT, db, "paper", "statu=%d", NULL, PAPER_ST_OK);
    
    MDB_QUERY_RAW(db, "paper", _COL_PAPER, "statu=%d ORDER BY id DESC "
                  " LIMIT %d OFFSET %d", NULL, PAPER_ST_OK, count, offset);
    err = mdb_set_rows(cgi->hdf, db, _COL_PAPER, PRE_OUTPUT".papers",
                       NULL, MDB_FLAG_EMPTY_OK);
    if (err != STATUS_OK) return nerr_pass(err);
    
    return STATUS_OK;
}

NEOERR* admin_paper_new_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mdb_conn *db = hash_lookup(dbh, "main");
    char *mname;
    NEOERR *err;

    MEMBER_CHECK_ADMIN();

    SET_ADMIN_ACTION();

    return STATUS_OK;
}
