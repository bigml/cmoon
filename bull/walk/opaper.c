#include "mheads.h"
#include "lheads.h"
#include "opaper.h"

NEOERR* paper_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    return STATUS_OK;
}

NEOERR* paper_class_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mdb_conn *db = hash_lookup(dbh, "main");
    char tok[LEN_HDF_KEY];
    int id, pid;
    NEOERR *err;

    HDF_FETCH_INT(cgi->hdf, PRE_QUERY".id", id);
    HDF_FETCH_INT(cgi->hdf, PRE_QUERY".pid", pid);

    HDF *node = hdf_get_child(cgi->hdf, PRE_QUERY".papers");
    if (node) {
    redo:
        HDF_GET_INT(node, "id", id);
        HDF_GET_INT(node, "pid", pid);
    }

    if (pid == 0) {
        hdf_set_valuef(cgi->hdf, PRE_OUTPUT".papers.%d.title=顶极分类", id);
    } else {
        MDB_QUERY_RAW(db, "paper", "title", "id=%d", NULL, pid);
        
        snprintf(tok, sizeof(tok), "%s.papers.%d", PRE_OUTPUT, id);
        err = mdb_set_row(cgi->hdf, db, "title", tok, MDB_FLAG_EMPTY_OK);
        if (err != STATUS_OK) return nerr_pass(err);
    }
    
    hdf_set_valuef(cgi->hdf, PRE_OUTPUT".papers.%d.id=%d", id, id);
    hdf_set_valuef(cgi->hdf, PRE_OUTPUT".papers.%d.pid=%d", id, pid);

    node = hdf_obj_next(node);
    if (node) goto redo;
    
    hdf_set_attr(cgi->hdf, PRE_OUTPUT".papers", "type", "array");
    
    return STATUS_OK;
}

NEOERR* paper_matchtitle_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mdb_conn *db = hash_lookup(dbh, "main");
    char *title;
    NEOERR *err;

    HDF_GET_STR(cgi->hdf, PRE_QUERY".title", title);

    MDB_QUERY_RAW(db, "paper", "id, title", "title LIKE '%%%s%%'", NULL, title);
    err = mdb_set_rows(cgi->hdf, db, "id, title", PRE_OUTPUT".titles",
                       NULL, MDB_FLAG_EMPTY_OK);
    if (err != STATUS_OK) return nerr_pass(err);

    hdf_set_attr(cgi->hdf, PRE_OUTPUT".titles", "type", "array");

    return STATUS_OK;
}

NEOERR* paper_preview_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    MCS_NOT_NULLA(cgi->hdf);

    hdf_set_copy(cgi->hdf, PRE_OUTPUT".s", PRE_QUERY".s");

    mstr_script_escape(cgi->hdf, PRE_OUTPUT".s");
    
    return STATUS_OK;
}
