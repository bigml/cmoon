#include "mheads.h"
#include "lheads.h"
#include "ospd.h"

NEOERR* spd_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mevent_t *evt = hash_lookup(evth, "aic");

    if (!cgi || !cgi->hdf || !evt) return nerr_raise(NERR_ASSERT, "paramter null");

    return STATUS_OK;
}

NEOERR* spd_data_add(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    /*
     * member
     */
    mevent_t *evt = hash_lookup(evth, "member");

    if (!cgi || !cgi->hdf || !evt) return nerr_raise(NERR_ASSERT, "paramter null");

    hdf_copy(evt->hdfsnd, NULL, hdf_get_obj(cgi->hdf, PRE_QUERY));

    hdf_set_value(evt->hdfsnd, "mstatu", "10");
    if (hdf_get_int_value(evt->hdfsnd, "dad", 0) == 1)
        hdf_set_value(evt->hdfsnd, "_addcar", "1");
    
    MEVENT_TRIGGER(evt, NULL, REQ_CMD_MEMBER_ADD, FLAGS_SYNC);

    /*
     * plan
     */
    evt = hash_lookup(evth, "plan");
    if (!evt) return nerr_raise(NERR_ASSERT, "plan backend error");
    
    hdf_copy(evt->hdfsnd, NULL, hdf_get_obj(cgi->hdf, PRE_QUERY));
    hdf_set_value(evt->hdfsnd, "pstatu", "10");

    char *s = hdf_get_value(evt->hdfsnd, "stime", NULL);
    if (s && !reg_search("^[0-2][0-9](:[0-9][0-9])+$", s))
        hdf_remove_tree(evt->hdfsnd, "stime");

    s = hdf_get_value(evt->hdfsnd, "etime", NULL);
    if (s && !reg_search("^[0-2][0-9](:[0-9][0-9])+$", s))
        hdf_remove_tree(evt->hdfsnd, "etime");

    MEVENT_TRIGGER(evt, NULL, REQ_CMD_PLAN_ADD, FLAGS_NONE);

    return STATUS_OK;
}
