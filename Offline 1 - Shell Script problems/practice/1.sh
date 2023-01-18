#!/bin/bash

n=$#
echo $n
if [[ $n -eq 3 ]]; then
    echo "3 args"
else
    echo "missing args"
fi

for file in *; do   
    echo -n "$file is : "
    #if [[ -f $file ]]; then   
    #    echo "file"
    #else 
    #    echo "not file"
    #fi
done