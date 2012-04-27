#include "mheads.h"
#include "lheads.h"
#include "opaper.h"

NEOERR* paper_of_recentday(HDF *node, HASH *dbh)
{
    mdb_conn *db = hash_lookup(dbh, "paper");
    int limit;
    NEOERR *err;

    MCS_NOT_NULLB(node, db);

    HDF_GET_INT(node, "limit", limit);
    
    MDB_QUERY_RAW(db, "paper", _COL_PAPER, "statu=%d ORDER BY id DESC LIMIT %d",
                  NULL, PAPER_ST_OK, limit);

    return nerr_pass(mdb_set_rows(node, db, _COL_PAPER, PRE_OUTPUT".papers",
                                  NULL, MDB_FLAG_EMPTY_OK));
}

NEOERR* paper_data_add(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mdb_conn *db = hash_lookup(dbh, "paper");
    STRING str; string_init(&str);
    int id;
    NEOERR *err;

    MCS_NOT_NULLB(cgi->hdf, db);

    err = mdb_build_incol(hdf_get_obj(cgi->hdf, PRE_QUERY),
                          hdf_get_obj(g_cfg, "Db.InsertCol.paper"), &str);
    if (err != STATUS_OK) return nerr_pass(err);

    MDB_EXEC(db, NULL, "INSERT INTO paper %s RETURNING id", NULL, str.buf);

    err = mdb_get(db, "i", &id);
    if (err != STATUS_OK) return nerr_pass(err);

    /*
     * produce static html
     */
    hdf_copy(cgi->hdf, PRE_OUTPUT, hdf_get_obj(cgi->hdf, PRE_QUERY));
    err = ltpl_render2filef(cgi, "paper", "%s%d.html", ROOT_PAPER, id);
    TRACE_NOK(err);
    
    /*
     * reproduce sitemap
     * system/exec pager/sitemap command has no effect for www-data user
     * (don't know why), so, do it your self, bigml
     * or, add to your crontab
     */
    //mtc_dbg("%d", mutil_systemf("%ssitemap", PATH_PAGER));
    
    return STATUS_OK;
}
