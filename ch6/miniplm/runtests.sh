#!/bin/sh
jq=jq
command -v $jq > /dev/null
if [ "$?" -eq 1 ]
then
    echo command "'$jq'" does not exist
    exit 1
fi

for f in tests/*.json
do
    echo Testing $f

    #process line by line, compare
    n=$(cat $f | $jq '.[].start' | wc -l)
    i=0
    while [ $i -lt $n ]
    do
        str="$(cat $f | $jq -c '.['$i']')"
	start=$(echo "$str" | $jq .start)
	input=$(echo "$str" | $jq .input)
	result=$(echo "$str" | $jq -c .result)
	start=$(echo "$start" | sed -e 's/^.*"\(.*\)".*$/\1/')
	input=$(echo "$input" | sed -e 's/^.*"\(.*\)".*$/\1/')
        res=$(./parse_test $start "$input" 2>&1)
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

