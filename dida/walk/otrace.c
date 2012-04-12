#include "mheads.h"
#include "lheads.h"
#include "otrace.h"

NEOERR* trace_event(HDF *node, HASH *evth, session_t *ses, int type)
{
    mevent_t *evt = hash_lookup(evth, "trace");
    
    MCS_NOT_NULLB(evt, node);

    hdf_copy(evt->hdfsnd, NULL, node);

    /*
     * system information
     */
    hdf_set_int_value(evt->hdfsnd, "type", type);
    hdf_set_value(evt->hdfsnd, "sender", ses->mname);
    hdf_set_value(evt->hdfsnd, "provid", hdf_get_value(ses->province, "id", NULL));
    hdf_set_value(evt->hdfsnd, "cityid", hdf_get_value(ses->city, "id", NULL));
    hdf_set_int_value(evt->hdfsnd, "browser", ses->browser);
    mcs_set_float_value(evt->hdfsnd, "bversion", ses->bversion);

    MEVENT_TRIGGER(evt, NULL, REQ_CMD_TRACE_ADD, FLAGS_NONE);

    return STATUS_OK;
}

NEOERR* trace_data_add(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mevent_t *evt = hash_lookup(evth, "trace");

    MCS_NOT_NULLB(cgi->hdf, evt);

    hdf_copy(evt->hdfsnd, NULL, hdf_get_obj(cgi->hdf, PRE_QUERY));

    MEVENT_TRIGGER(evt, NULL, REQ_CMD_TRACE_ADD, FLAGS_NONE);

    return STATUS_OK;
}
