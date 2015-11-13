#!/bin/sh

# Data and Conf
if [ -n "$1" ]; then
    mkdir $1;
	mkdir $1"/html/";
    mkdir $1"/html/days/";
    mkdir $1"/html/reunions/";
    mkdir $1"/html/starts/";
    mkdir $1"/html/odds/";
    mkdir $1"/html/arrivals/";
#    mkdir $1"/json/";
#    mkdir $1"/json/starts/";
#    mkdir $1"/json/arrivals/";
#    mkdir $1"/training-sets/";
#    mkdir $1"/races/";
#    mkdir $1"/brains/";
    if [ -n "$2" ]; then
            sed -i -- "s|PATH_TO_DATA|$1|g" $2
    fi
    if [ -z "$2" ]; then
            echo "You need to indicate where the conf file is : ./init.sh [path-to-data] [path-to-conf]";
    fi
fi
if [ -z "$1" ]; then
    echo "You need to choose a directory for the data : ./init.sh [path-to-data]";
fi

