#include "mheads.h"
#include "lheads.h"
#include "oadmin.h"

NEOERR* paper_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    return STATUS_OK;
}

NEOERR* paper_preview_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    MCS_NOT_NULLA(cgi->hdf);

    hdf_set_copy(cgi->hdf, PRE_OUTPUT".s", PRE_QUERY".s");

    mstr_script_escape(cgi->hdf, PRE_OUTPUT".s");
    
    return STATUS_OK;
}
