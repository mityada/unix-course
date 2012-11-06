#!/bin/bash

shopt -s nullglob

QPATH=$HOME/.nyaqueue

mkdir -p "$QPATH"
mkdir -p "$QPATH/requests"

tmp=`mktemp -d --tmpdir="$QPATH"`

for a in "$@"
do
    r=`mktemp --tmpdir="$tmp"`
    echo "$a" > "$r"
    mv "$r" "$QPATH/requests"
done

rm -rf $tmp

wgetdaemon() {
    touch "$QPATH/running"
    trap "rm '$QPATH/running'" 0
    trap "" SIGHUP

    while [ 1 ]
    do
        for a in "$QPATH/requests"/*
        do
            url=`cat "$a"`
            rm "$a"
            wget -c "$url"
        done

        sleep 1
    done
}

[[ -f "$QPATH/running" ]] || wgetdaemon 0<&- &> /dev/null &
