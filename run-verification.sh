#!/bin/bash

EXEC=$1
SET_NAME=$2
PROTOCOLS_NUM=$3
LOG="logs/log$SET_NAME"
ERRLOG="logs/err_log$SET_NAME"
touch $LOG
touch $ERRLOG
for i in `seq 0 ${PROTOCOLS_NUM}`
do
  mkdir outs/$SET_NAME
  echo "START $i" >> $LOG
  $EXEC outs ins $SET_NAME $i >>$LOG 2>>$ERRLOG && echo "DONE" >>$LOG || echo "ERROR" >>$LOG
done