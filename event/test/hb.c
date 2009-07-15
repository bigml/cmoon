/*
 * 事件中心心跳检测报警程序, 需配合 crontab 定时拉起使用
 * PATH/hb 192.168.1.15 db_community ../daemon/mevent.hdf > /tmp/meventhb.log
 * 每次检查一个后端插件的工作情况
 *
 * 正常情况下 输出 后端插件的 stats 统计信息
 * 处理错误 0x800 时 短信上报 errcode
 * 网络,客户端错误时  短信上报 trigger return
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "mevent.h"
#include "timer.h"
#include "common.h"
#include "smsalarm.h"

int main(int argc, char *argv[])
{
	uint32_t errcode;
	int ret;

	if (argc != 4) {
		printf("Usage: %s HOST EVENT_NAME CONFIG_FILE\n", argv[0]);
		return 1;
	}

	settings.smsalarm = 1;
	settings.logfname = "-";
	log_init();
	if (config_parse_file(argv[3], &g_cfg) != 1) {
		printf("parse config file %s failure", argv[3]);
			return 1;
	}
	
	mevent_t *evt = mevent_init();
	if (evt == NULL) {
		printf("init error\n");
		SMS_ALARM("mevent_init error");
		return 1;
	}
	
	mevent_add_udp_server(evt, argv[1], 26010);
	mevent_chose_plugin(evt, argv[2], REQ_CMD_STATS, FLAGS_SYNC);
	ret = mevent_trigger(evt, &errcode);
	if (ret == REP_OK) {
		data_cell_dump(evt->rcvdata);
	} else if (ret == REP_ERR) {
		if (errcode == ERR_BUSY) {
			printf("process busy %d!\n", errcode);
			SMS_ALARM("process busy %d", errcode);
		} else {
			printf("process error %d!\n", errcode);
			SMS_ALARM("process error %d", errcode);
		}
	} else {
		printf("process failure %d\n", ret);
		SMS_ALARM("process failure %d", ret);
	}

	mevent_free(evt);
	return 0;
}
