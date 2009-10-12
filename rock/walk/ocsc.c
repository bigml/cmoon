#include "mheads.h"
#include "lheads.h"
#include "ocsc.h"
#include "ofile.h"

#define CSC_QUERY_COL " id, fid, uid, img, exp, to_char(intime, 'YYYY-MM-DD') as intime, " \
	" to_char(uptime, 'YYYY-MM-DD') as uptime "

#define CSC_QUERY_RAW(conn, condition, sfmt, ...)						\
	mdb_exec(conn, NULL, "SELECT "CSC_QUERY_COL" FROM tjt WHERE %s;",	\
			 sfmt, condition, ##__VA_ARGS__)

#define CSC_GET_RAW(conn, tjt)										\
	mdb_get(conn, "iiiSSSS", &(tjt->id), &(tjt->fid), &(tjt->uid),		\
			&(tjt->img), &(tjt->exp), &(tjt->intime), &(tjt->uptime))

int csc_get_data(HDF *hdf, HASH *dbh, session_t *ses)
{
	int count, offset, fid, ret;

	mdb_conn *dbsys, *dbcsc;

	dbsys = (mdb_conn*)hash_lookup(dbh, "Sys");
	dbcsc = (mdb_conn*)hash_lookup(dbh, "Csc");

	PRE_DBOP(hdf, dbsys);
	PRE_DBOP(hdf, dbcsc);

	if (ses->file != NULL)
		lutil_fill_layout_by_file(dbsys, ses->file, hdf);
	hdf_set_value(hdf, PRE_OUTPUT".navtitle", "菜色");
	file_get_nav_by_uri(dbsys, "/csc", PRE_OUTPUT, hdf);

	ret = file_check_user_power(hdf, dbsys, ses, ses->file, LMT_APPEND);
	if (ret == RET_RBTOP_OK) {
		hdf_set_value(hdf, PRE_OUTPUT".appendable", "1");
	}

	/* TODO cache this */
	mmisc_get_offset(hdf, &count, &offset);
	fid = ses->file->id;
	CSC_QUERY_RAW(dbcsc, "fid=$1 ORDER BY uptime LIMIT $2 OFFSET $3",
				  "iii", fid, count, offset);
	return mdb_set_rows(hdf, dbcsc, CSC_QUERY_COL, PRE_OUTPUT".items");
}

int csc_add_image(CGI *cgi, mdb_conn *conn, session_t *ses)
{
	unsigned char hash[LEN_MD5];
	int ret;

	FILE *fp = cgi_filehandle(cgi, "imagename");
	
	PRE_DBOP(cgi->hdf, conn);
	
	if (fp == NULL) {
		mtc_err("input file named: imagename not found");
		return RET_RBTOP_INPUTE;
	}

	ret = lutil_image_accept(fp, "csc", hash);
	if (ret != RET_RBTOP_OK) {
		mtc_err("accept image failure %d", ret);
		return ret;
	}
	
	hdf_set_valuef(cgi->hdf, PRE_OUTPUT".imageurl=%s/%s/%s/%s.jpg",
                   IMG_DOMAIN, IMG_PATH, IMG_ORI, hash);
	hdf_set_valuef(cgi->hdf, PRE_OUTPUT".imagename=%s.jpg", hash);
	return RET_RBTOP_OK;
}

int csc_add_item(HDF *hdf, mdb_conn *conn, session_t *ses)
{
	PRE_DBOP(hdf, conn);

    int fid, uid;
    char *img, *exp;
    int ret;

    uid = ses->member->uin;
    fid = ses->file->id;
    img = hdf_get_value(hdf, PRE_QUERY".img", "");
    exp = hdf_get_value(hdf, PRE_QUERY".exp", "");

    ret = MDATA_SET(conn, EVT_PLUGIN_CSC, NULL, FLAGS_NONE,
                    "INSERT INTO tjt (fid, uid, img, exp) "
                    " VALUES (%d, %d, $1, $2)", "ss",
                    fid, uid, img, exp);
    if (ret != MDB_ERR_NONE) {
        mtc_err("add file err %s", mdb_get_errmsg(conn));
        return RET_RBTOP_INSERTE;
    }
    
	return RET_RBTOP_OK;
}
