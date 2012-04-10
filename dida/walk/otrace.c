#include "mheads.h"
#include "lheads.h"
#include "otrace.h"

NEOERR* trace_data_add(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mevent_t *evt = hash_lookup(evth, "trace");

    MCS_NOT_NULLB(cgi->hdf, evt);

    hdf_copy(evt->hdfsnd, NULL, hdf_get_obj(cgi->hdf, PRE_QUERY));

    MEVENT_TRIGGER(evt, NULL, REQ_CMD_TRACE_ADD, FLAGS_NONE);

    return STATUS_OK;
}
