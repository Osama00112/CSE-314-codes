diffChecker(){
    count=0

    while IFS= read -r line
        do
        if [[ ${line:0:1} == "<" || ${line:0:1} == ">" ]];then
            count=$(( $count + 1 ))
        fi
        done < "$1"
    echo $count
}

copyChecker(){
    count=0
    while IFS= read -r line
    do
        count=$(( $count + 1 ))
    done < "$1"
    echo $count
}

echo "student_id,score">output.csv
max_score=100
max_std_id=5
args=$#
std_count=1805121

if [[ $args -gt 0 ]]; then
    max_score=$1
fi
if [[ $args -gt 1 ]]; then
    max_std_id=$2
fi

for file in Submissions/*; do   
    #echo -n "$file is : "
    if [[ (( !(-f $file) )) ]]; then   
        #echo "not file"
        last_digit=${file: -1}        
        if [[ $last_digit -gt $max_std_id ]]; then
            break
        fi
        last_7digits=${file: -7}
        if [[ $std_count -ne $last_7digits ]]; then
            score=0
            echo "$std_count,$score">>output.csv
            std_count=$(( $std_count + 1 ))
        fi
        

        if [[ (( !(-e $file/$last_7digits.sh) )) ]]; then
            echo "doesnt exist"
            score=0
            echo "$std_count,$score">>output.csv
            std_count=$(( $std_count + 1 ))
            continue
        fi
        bash $file/$last_7digits.sh>output.txt
        diff -w output.txt AcceptedOutput.txt>diff.txt
        mismatch=$(diffChecker diff.txt)
        score=$(( $max_score - $(( $mismatch * 5 )) ))
        if [[ $score -lt 0 ]];then
            score=0
        fi
        for file2 in Submissions/*; do
            diff -w $file2/*.sh $file/${file: -7}.sh>diff2.txt
            copyMatch=$(copyChecker diff2.txt)
            if [[ $file == $file2 ]]; then
                copyMatch=1
            fi
        
            if [[ $copyMatch -eq 0 ]]; then
                score=$(( - $score ))
                break
            fi
        done
        echo "${file: -7},$score">>output.csv

    fi
    std_count=$(( std_count + 1 ))
done
rm diff.txt
rm diff2.txt
rm output.txt
