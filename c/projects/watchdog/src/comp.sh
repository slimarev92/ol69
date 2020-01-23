#!/bin/bash

DS_PATH='../../../ds/src'

GD='gcc -pedantic -o3 -pedantic-errors -Wall -Wextra -g -o prog' 

rm *.out

$GD watchdog.c $DS_PATH/scheduler.c  $DS_PATH/hpq.c $DS_PATH/task.c \
$DS_PATH/uid.c  $DS_PATH/heap.c  $DS_PATH/dvec.c  -I../../../ds/include -I../include \
-lpthread

mv prog watchdog.out

# $GD wd_api.c $DS_PATH/scheduler.c  $DS_PATH/hpq.c $DS_PATH/task.c \
# $DS_PATH/uid.c  $DS_PATH/heap.c  $DS_PATH/dvec.c  -I../../../ds/include -I../include \
# -lpthread

# mv prog wd_api.out

$GD ../test/wd_test.c $DS_PATH/scheduler.c  $DS_PATH/hpq.c $DS_PATH/task.c \
$DS_PATH/uid.c  $DS_PATH/heap.c  $DS_PATH/dvec.c ../src/wd_api.c  \
-I../../../ds/include -I../include -lpthread

mv prog wd_test.out 





