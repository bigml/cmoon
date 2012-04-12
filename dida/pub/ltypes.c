#include "mheads.h"
#include "lheads.h"

static char *m_browsers[4] = {"mozilla", "webkit", "opera", "msie"};
static int   m_browsers_size = 4;

session_t* session_default()
{
    session_t *ses = calloc(1, sizeof(session_t));

    ses->reqtype = CGI_REQ_AJAX;
    
    return ses;
}

NEOERR* session_init(CGI *cgi, HASH *dbh, session_t **ses)
{
    session_t *lses;
    char tok[_POSIX_PATH_MAX];

    HDF *node = hdf_get_child(cgi->hdf, PRE_QUERY);
    while (node) {
        snprintf(tok, sizeof(tok), "%s._type_%s", PRE_QUERY, hdf_obj_name(node));
        char *type = hdf_get_value(cgi->hdf, tok, NULL);
        if (type && !strcmp(type, "object")) {
            mjson_str2hdf(node, NULL);
        }
        node = hdf_obj_next(node);
    }
    
    *ses = NULL;

    lses = calloc(1, sizeof(session_t));
    if (!lses) return nerr_raise(NERR_NOMEM, "calloc memory for session_t failure");

    /*
     * mname
     */
    char *s;
    HDF_FETCH_STR(cgi->hdf, PRE_COOKIE".mname", s);
    if (!s) HDF_FETCH_STR(cgi->hdf, PRE_COOKIE".username", s);
    if (s) lses->mname = strdup(s);

    /*
     * province
     */
    HDF_FETCH_STR(cgi->hdf, PRE_COOKIE".province", s);
    hdf_init(&lses->province);
    if (s) {
        neos_unescape(s, strlen(s), '%');
        hdf_set_value(lses->province, NULL, s);
        mjson_str2hdf(lses->province, NULL);
    }

    /*
     * city
     */
    HDF_FETCH_STR(cgi->hdf, PRE_COOKIE".city", s);
    hdf_init(&lses->city);
    if (s) {
        neos_unescape(s, strlen(s), '%');
        hdf_set_value(lses->city, NULL, s);
        mjson_str2hdf(lses->city, NULL);
    }

    /*
     * browser
     */
    HDF_FETCH_STR(cgi->hdf, PRE_HTTP".UserAgent", s);
    if (s) {
        mstr_repchr(s, ' ', '\0');
        for (int i = 0; i < m_browsers_size; i++) {
            if (!strncasecmp(s, m_browsers[i], strlen(m_browsers[i]))) {
                lses->browser = i;
                break;
            }
        }
        s = strchr(s, '/');
        if (s) lses->bversion = strtof(s+1, NULL);
    }

    /*
     * reqtype
     */
    lses->reqtype = CGI_REQ_HTML;
    char *uri = hdf_get_value(cgi->hdf, PRE_REQ_URI_RW, NULL);
    if (!uri) {
        uri = "terminal";
        lses->reqtype = CGI_REQ_TERMINAL;
    }
    mstr_repchr(uri, '/', '_');
    uri = mstr_strip(uri, '_');
    if (!strncmp(uri, "json_", 5)) {
        uri = uri+5;
        lses->reqtype = CGI_REQ_AJAX;
    } else if (!strncmp(uri, "image_", 6)) {
        uri = uri+6;
        lses->reqtype = CGI_REQ_IMAGE;
    }

    /*
     * dataer, render
     */
    switch (http_req_method(cgi)) {
        case CGI_REQ_POST:
            snprintf(tok, sizeof(tok), "%s_data_mod", uri);
            break;
        case CGI_REQ_PUT:
            snprintf(tok, sizeof(tok), "%s_data_add", uri);
            break;
        case CGI_REQ_DEL:
            snprintf(tok, sizeof(tok), "%s_data_del", uri);
            break;
        default:
        case CGI_REQ_GET:
            snprintf(tok, sizeof(tok), "%s_data_get", uri);
            break;
    }
    lses->dataer = strdup(tok);
    lses->render = strdup(uri);

    /*
     * tm_cache_browser
     */
    node = hdf_get_obj(g_cfg, PRE_CFG_FILECACHE".0");
    while (node != NULL) {
        if (reg_search(hdf_get_value(node, "uri", "NULL"), uri)) {
            lses->tm_cache_browser = hdf_get_int_value(node, "tm_cache", 0);
            break;
        }
        node = hdf_obj_next(node);
    }

    /*
     * DONE
     */
    *ses = lses;
    
    return STATUS_OK;
}

void session_destroy(session_t **ses)
{
    session_t *lses;
    if (ses == NULL) return;
    lses = *ses;

    if (lses == NULL) return;

    SAFE_FREE(lses->mname);
    SAFE_FREE(lses->dataer);
    SAFE_FREE(lses->render);

    hdf_destroy(&lses->province);
    hdf_destroy(&lses->city);

    free(lses);
    lses = NULL;
}
