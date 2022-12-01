#!/bin/bash

echo number of arguments is $#
a=1
for i in $*
do
    echo "arg no. $a is $i"
    a=`$a + 1`
done

for i in $(ls)
do  
    echo "$i"
done


fact(){
    if [[ $1 -eq 0 ]]; then
        echo 1
        return
    fi

    last=`fact $(( $1 - 1 ))`

    echo $(( $1 * last ))
}

fact $1
