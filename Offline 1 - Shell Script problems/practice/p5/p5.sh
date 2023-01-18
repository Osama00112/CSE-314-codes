count=0
while true; do
    count=$(( $count + 1 ))
    ./temp.sh >out.txt 2>err.txt
    if [[ -n $(cat err.txt) ]];then 
        echo "exiting loop, Lines: $count"
        exit 0
    fi
done