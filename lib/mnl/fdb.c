#include "mheads.h"

int fdb_init_long(fdb_t **fdb, char *ip, char *user, char *pass,
				  char *name, unsigned int port)
{
	fdb_t *ldb;

	ldb = calloc(1, sizeof(fdb_t));
	if (ldb == NULL) {
		return RET_DBOP_INITE;
	}
	ldb->conn = mysql_init(NULL);
	if (ldb->conn == NULL) {
		return RET_DBOP_INITE;
	}
	*fdb = ldb;

	if (!strcasecmp(ip, "null")) ip = NULL;
	if (!strcasecmp(user, "null")) user = NULL;
	if (!strcasecmp(pass, "null")) pass = NULL;
	if (!strcasecmp(name, "null")) name = NULL;

	my_bool reconnect = 1;
	mysql_options(ldb->conn, MYSQL_OPT_RECONNECT, &reconnect);
	mysql_options(ldb->conn, MYSQL_SET_CHARSET_NAME, "utf8");
	mysql_options(ldb->conn, MYSQL_INIT_COMMAND, "SET NAMES 'utf8'");

	MYSQL *lcon = NULL;
	lcon = mysql_real_connect(ldb->conn, ip, user, pass, name, port,
							  NULL, CLIENT_FOUND_ROWS);
	if (lcon == NULL)
		return RET_DBOP_CONNECTE;
	else
		ldb->conn = lcon;
	return RET_DBOP_OK;
}

int fdb_exec(fdb_t *fdb)
{
	int ret;
	
	if (fdb->conn == NULL)
		return RET_DBOP_CONNECTE;
	if (fdb->res != NULL) {
		mysql_free_result(fdb->res);
		fdb->res = NULL;
	}
	ret = mysql_real_query(fdb->conn, fdb->sql, strlen(fdb->sql));
	if (ret != 0)
		return RET_DBOP_ERROR;
	fdb->res = mysql_store_result(fdb->conn);
	return RET_DBOP_OK;
}
int fdb_fetch_row(fdb_t *fdb)
{
	if (fdb->conn == NULL)
		return RET_DBOP_CONNECTE;
	if (fdb->res == NULL) {
		fdb->res = mysql_store_result(fdb->conn);
		if (fdb->res == NULL)
			return RET_DBOP_ERROR;
	}
	fdb->row = mysql_fetch_row(fdb->res);
	if (fdb->row == NULL)
		return RET_DBOP_ERROR;
	return RET_DBOP_OK;
}
int fdb_affect_rows(fdb_t *fdb)
{
	if (fdb->conn == NULL)
		return RET_DBOP_CONNECTE;
	return (int)mysql_affected_rows(fdb->conn);
}
char* fdb_error(fdb_t *fdb)
{
	if (fdb == NULL) return "init error";
	if (fdb->conn == NULL) return "connect error";
	return (char*)mysql_error(fdb->conn);
}

void fdb_free(fdb_t **fdb)
{
	fdb_t *ldb = *fdb;
	
	if (ldb == NULL)
		return;
	if (ldb->res != NULL) {
		mysql_free_result(ldb->res);
		ldb->res = NULL;
	}
	if (ldb->conn != NULL) {
		mysql_close(ldb->conn);
		ldb->conn = NULL;
	}
	free(ldb);
	*fdb = NULL;
}

static void get_errmsg(int ret, char *res)
{
	switch (ret) {
	case RET_DBOP_INITE:
		strcpy(res, "链接数据库错误");
		break;
	case RET_DBOP_HDFNINIT:
		strcpy(res, "输入数据错误");
		break;
	case RET_DBOP_INPUTE:
		strcpy(res, "输入参数错误");
		break;
	case RET_DBOP_DBNINIT:
		strcpy(res, "数据库未初始化");
		break;
	case RET_DBOP_DBINTRANS:
		strcpy(res, "数据库操作未提交");
		break;
	case RET_DBOP_SELECTE:
		strcpy(res, "数据库查询失败");
		break;
	case RET_DBOP_UPDATEE:
		strcpy(res, "数据库更新失败");
		break;
	case RET_DBOP_EXIST:
		strcpy(res, "资源不存在");
		break;
	case RET_DBOP_NEXIST:
		strcpy(res, "资源已存在");
		break;
	case RET_DBOP_MEMALLOCE:
		strcpy(res, "分配内存失败");
	default:
		strcpy(res, "数据库操作错误");
		break;
	}
}

void fdb_opfinish(int ret, HDF *hdf, fdb_t *fdb,
				  char *target, char *url, bool header)
{
	char msg[LEN_SM];
	
	if (ret == RET_DBOP_OK) {
		return;
	}

	get_errmsg(ret, msg);
	mutil_redirect(msg, target, url, header);
	
	if (fdb != NULL) {
		fdb_free(&fdb);
	}
	/* TODO system resource need free*/
	exit(ret);
}

void fdb_opfinish_json(int ret, HDF *hdf, fdb_t *fdb)
{
	char msg[LEN_SM];
	
	if (ret == RET_DBOP_OK) {
		hdf_set_value(hdf, PRE_SUCCESS, "1");
		return;
	}

	hdf_remove_tree(hdf, PRE_SUCCESS);
	get_errmsg(ret, msg);
	hdf_set_value(hdf, PRE_ERRMSG, msg);
	mjson_output_hdf(hdf, 0);
	
	if (fdb != NULL) {
		fdb_free(&fdb);
	}
	/* TODO system resource need free*/
	exit(ret);
}
