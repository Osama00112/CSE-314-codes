#!/bin/bash

n=$#
echo $n
if [[ $n -gt 0 ]]; then
    echo "3 args"
else
    echo "Usage script1.sh filename1 filename2 ..."
fi

for file in $*; do   
    if [[ -f $file ]]; then   
        echo "regular file"
        if [[ -x $file ]]; then   
            echo "$file is currently executable."
            ls -l $file
            echo "changing permission"
            chmod a-x $file
            ls -l $file
            echo "currently not executable"
        else 
            echo "not exec"
        fi
    else 
        echo "not file"
    fi
done