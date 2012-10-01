#!/bin/bash

getopts "-type:-iname:" $options
echo $options

shopt -s dotglob
shopt -s nocaseglob

path="."

if [[ $# -gt 0 ]]; then
    path=$1
    shift
fi

echo $path

iname="*"
print_files=1
print_dirs=1

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
        case $1 in
        f)
            print_dirs=0
            ;;
        d)
            print_files=0
            ;;
        esac
    esac
done

print_if_matches() {
	if [[ ${1##*/} == $iname ]]; then
		echo $1
	fi
}

print_dir() {
	if [[ (-d $1 && $print_dirs == 1) || (!(-d $1) && $print_files == 1) ]]; then
		print_if_matches $1	
	fi

	for file in $1/*
	do
		if [[ -d $file ]]; then
			print_dir $file
		else
			if [[ $print_files == 1 ]]; then
				print_if_matches $file
			fi
		fi
	done
}

print_dir $path
