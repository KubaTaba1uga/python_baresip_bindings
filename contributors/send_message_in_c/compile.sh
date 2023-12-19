#!/bin/bash

RE_PATH=$1

BARESIP_PATH=$2

gcc client_server_app.c -g \
    -Wl,--copy-dt-needed-entries \
    -I$RE_PATH/include -I$BARESIP_PATH/include \
    -lbaresip -lre -lpthread \
    -L$RE_PATH/build -L$BARESIP_PATH/build  \
    -Wl,-rpath=$RE_PATH/build \
    -o client_server
