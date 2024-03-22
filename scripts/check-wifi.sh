#!/bin/sh
wifi="$(ip a | grep wlano | grep inet | wc -l)"
if [ $wifi -gt 1 ] 
then
    exit 1
fi
exit 0