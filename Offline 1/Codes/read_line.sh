#!/bin/bash

# IFS= (or IFS='') prevents leading/trailing whitespace from being trimmed
# -r prevents backslash escapes from being interpreted

[[ $# -ne 1 ]] && exit 1

pat=$1

# while read line; do
while read line; do
    if echo "$line" | grep -q "$1"; then
        echo "$line"
    fi
    # if grep -q "cin" <(echo "$line"); then
    #     echo "$line"
    # fi
done
