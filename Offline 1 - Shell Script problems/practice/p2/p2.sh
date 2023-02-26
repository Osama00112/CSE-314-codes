#read line
#read pattern
for file in *.txt; do
    #cat  $arg|head -n$1|tail -n1|grep $2
    #echo $?
    pattern=$(cat $file | head -n$1 | tail -n1 | grep $2)
    #echo $pattern
    if [[ "$pattern" = "hello" ]];then 
        echo "pawa gese"
    else
        echo "nai"
    fi
    #true && echo "pawa gese"
    #false && echo "nai"
done

