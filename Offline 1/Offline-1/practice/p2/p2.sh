#read line
#read pattern
for arg in *.txt; do
    head -n$1 $arg|tail -n1|grep $2
    #echo $?
    if false;then 
        echo "pawa gese"
    else
        echo "nai"
    fi
    #true && echo "pawa gese"
    #false && echo "nai"
done

