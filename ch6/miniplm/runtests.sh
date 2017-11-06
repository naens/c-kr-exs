#!/bin/sh
for f in tests/*.tst
do
    echo Testing $f

    #process line by line, compare
    while read ln;
    do
    	start=$(echo "$ln" | cut -f1)
    	instr=$(echo "$ln" | cut -f2)
    	reslt=$(echo "$ln" | cut -f3)
    	if [ -n "$start" ] && [ -n "$instr" ] && [ -n "$reslt" ]
    	then
    	    res="$(./parse_test $start \"$instr\" 2> /dev/null)"
    	    if [ "$?" -ne 0 ] && [ "$reslt" = "BAD_EXPR" ]
    	    then
                printf 'OK:%14s\t%-12s====>\tBAD_EXPR\n' "$start" "$instr"
            elif [ "$res" = "$reslt" ]
            then
                printf 'OK:%14s\t%-12s====>\t%s\n' "$start" "$instr" "$reslt"
            else
                printf 'ERROR:%14s\t%-12s====>\t%s\tRESULT=%s\n' \
                		"$start" "$instr" "$reslt" "$res"
            fi
    	fi
    done < $f
done

