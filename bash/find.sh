#!/bin/bash

shopt -s dotglob
shopt -s nocasematch

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
            echo "find: Arguments to -type should contain only one letter" 1>&2
            exit
        fi
        shift
    esac
done

print_if_matches() {
    if [[ ${1##*/} == $iname ]]; then
        if [[ $filetype == "" ]] ||
           [[ ($filetype == "l" || ! -h $1) &&
           (($filetype == "b" && -b $1) ||
            ($filetype == "c" && -c $1) ||
            ($filetype == "d" && -d $1) ||
            ($filetype == "p" && -p $1) ||
            ($filetype == "f" && -f $1) ||
            ($filetype == "l" && -h $1) ||
            ($filetype == "s" && -S $1)) ]]; then
            echo "$1"
        fi
    fi
}

print_dir() {
    print_if_matches $1    

    if [[ ! -d $1 || -h $1 ]]; then
        return
    fi
        
    if [[ ! -r $1 ]]; then
        echo "find: \`$1': Permission denied" 1>&2
        return
    fi

    for file in $1/*
    do
        if [[ ! -a "$file" ]]; then
            continue
        fi 
        print_dir $file
    done
}

if [[ ! -a $path ]]; then
    echo "find: \`$path': No such file or directory" 1>&2
    exit
fi

print_dir ${path%/}
