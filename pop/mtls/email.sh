#!/bin/bash

useage()
{
   echo "useage: $0 GOTIME [0 immediatly 1 minute 2 O clock 3 midnight]"
   echo "example: $0 0"
   exit 1
}

if [ $# -lt 1 ]; then
	useage
fi

CUR_DATE=`date +"%F%T"`
GOTIME=$1
TMPPREFIX=/tmp/email
TMPFILE=${TMPPREFIX}.${GOTIME}.${CUR_DATE}
TMPFILEMAIL=${TMPFILE}.content
FS="|"


# 根据参数 select 对应的数据到 临时文件
psql -U lcser lcs_aux -t -F${FS} -c "SELECT max(opts), max(subject), str_concat(sendto || ', '), str_concat(cc || ', '), str_concat(bcc || ', '), checksum, str_concat(id || ',') FROM email WHERE state=0 AND gotime="${GOTIME}" GROUP BY checksum" > ${TMPFILE}

# readline 临时文件， 分行处理
# awk -F: 'script' / -f scriptfile FILE  对FILE逐行进行script操作
# script 由 pattern 和 action 组成
# pattern {action} 如$ awk '/root/' test，或$ awk '$3 < 100' test。
# pattern 为空 或者 满足条件时， 执行action
# action 为空时， pattern true 打印
cat ${TMPFILE} | while read oneline
do
	legal=`echo ${oneline} | awk -F${FS} '{if (NF==7) {print "ok"} else {print "illegal"}}'`
	if [ "${legal}" = "ok" ]; then
		checksum=`echo ${oneline} | awk -F${FS} '{ print $6}'`
		psql -U lcser lcs_aux -t -c "SELECT content FROM email WHERE checksum='${checksum}' LIMIT 1" > ${TMPFILEMAIL}
		
		ids=`echo ${oneline} | awk -F${FS} '{ print $7}'`
		idsl=$((${#ids}-1))
#		ids=$(expr substr ${ids} 0 ${idsl})
		ids=${ids:0:${idsl}}
		cmd=`echo ${oneline} | awk -F${FS} '{ print "/usr/local/bin/email " $1 " -s " $2 " -cc " $4 " -bcc " $5 $3}'`
		echo -n "send mail for ${ids}... "
		${cmd} < ${TMPFILEMAIL}
		if [ $? = 0 ]; then
			echo "done"
			psql -U lcser lcs_aux -c "UPDATE email SET state=1 WHERE id IN (${ids})"
		else
			echo "failure $?."
		fi
	else
		echo "${oneline} illegal"
	fi
done
