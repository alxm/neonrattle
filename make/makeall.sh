#!/bin/bash

output=""
make_arg=$1

color_text ()
{
    output="$output\n\033[$1;1m$2\033[0m"
}

record_result ()
{
    if [ $2 -eq 0 ];
    then
        color_text 32 "[ Success ] $1"
    else
        color_text 31 "[ Failure ] $1"
    fi
}

do_make ()
{
    make -k -j -f $2 $make_arg
    record_result "$1" $?
}

do_emmake ()
{
    emmake make -k -j -f $2 $make_arg
    record_result "$1" $?
}

source /opt/emsdk/emsdk_env.sh

do_make "Dev" Makefile
do_emmake "Emscripten" Makefile.emscripten

echo -e $output
