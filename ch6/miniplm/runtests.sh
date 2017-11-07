#!/bin/sh
for f in tests/*.json
do
    echo Testing $f

    #process line by line, compare
    n=$(cat $f | jq '.[].start' | wc -l)
    echo n=$n
    i=0
    while [ $i -lt $n ]
    do
        str="$(cat $f | jq -c '.['$i']')"
	start=$(echo "$str" | jq .start)
	input=$(echo "$str" | jq .input)
	result=$(echo "$str" | jq -c .result)
        res=$(eval ./parse_test $start $input 2>&1)
        if [ "$?" -ne 0 ] && [ "$result" = "bad input" ]
        then
             printf 'OK:%14s\t%-12s====>\tbad input\n' "$start" "$input"
        elif [ "$res" = "$result" ]
        then
            printf 'OK:%14s\t%-12s====>\t%s\n' "$start" "$input" "$result"
        else
            printf 'ERROR:%14s\t%-12s====>\t%s\tRESULT="%s"\n' \
            		"$start" "$input" "$reslt" "$res"
        fi
        i=$(expr $i + 1)
    done
done

