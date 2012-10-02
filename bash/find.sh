#!/bin/bash

shopt -s dotglob
shopt -s nocaseglob

path="."

if [[ $# -gt 0 && ${1} != -* ]]; then
    path=$1
    shift
fi

iname="*"
filetype=""

while [[ $# -gt 0 ]]
do
    case $1 in
    -iname)
        shift
        iname=$1
        shift
        ;;
    -type)
        shift
        filetype=$1
        if [[ ${filetype} != [bcdpfls] ]]; then
            echo "find: Arguments to -type should contain only one letter"
            exit
        fi
        shift
    esac
done

print_if_matches() {
    if [[ ${1##*/} == $iname ]]; then
        if [[ $filetype == "" ]] ||
           [[ ($filetype == "l" || !(-h $1)) &&
           (($filetype == "b" && -b $1) ||
            ($filetype == "c" && -c $1) ||
            ($filetype == "d" && -d $1) ||
            ($filetype == "p" && -p $1) ||
            ($filetype == "f" && -f $1) ||
            ($filetype == "l" && -h $1) ||
            ($filetype == "s" && -S $1)) ]]; then
            echo $1
        fi
    fi
}

print_dir() {
    print_if_matches $1    

    for file in $1/*
    do
        if [[ -d $file && !(-h $file) ]]; then
            print_dir $file
        else
            print_if_matches $file
        fi
    done
}

print_dir $path
