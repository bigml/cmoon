#ifndef __MEVENT_APEEXT_H__
#define __MEVENT_APEEXT_H__

#include <stdbool.h>

enum {
	REQ_CMD_USERON  = 1001,
	REQ_CMD_USEROFF = 1002,
	REQ_CMD_MSGSND  = 2001,
	REQ_CMD_MSGBRD  = 2002
} req_cmd_aic;

enum {
	REP_ERR_ALREADYREGIST = 501,
	REP_ERR_NREGIST = 511
} rep_code_aic;

typedef struct _UserEntry {
	bool online;
	char *server;
} UserEntry;

UserEntry* user_new();

#endif	/* __MEVENT_APEEXT_H__ */
