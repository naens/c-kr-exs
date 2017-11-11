#!/bin/sh
jq=jq
command -v $jq > /dev/null
if [ "$?" -eq 1 ]
then
    echo command "'$jq'" does not exist
    exit 1
fi

echo "Enter a module name:"
read name

fn="tests/$name.json"

str="["
first=true

if [ -f "$fn" ]
then
    n=$(cat $fn | $jq '.[].start' | wc -l)
    i=0
    while [ $i -lt $n ]
    do
        if [ "$first" = "false" ]
        then
            str="$str,"
        fi
        str=$str"$(cat $fn | $jq -c '.['$i']')"
        first=false
        i=$(expr $i + 1)
    done
fi

start=""
while true
do
    if [ "$start" = "" ]
    then
        echo "Enter a nonterminal:"
    else
        echo "Enter a nonterminal (currently $start):"
    fi
    read s
    if [ -n "$s" ]
    then
        start=$s
    fi

    echo "Enter an expression:"
    read input

    if [ -z "$input" ]
    then
        break
    fi

    if [ "$first" = "false" ]
    then
        str="$str,"$'\n'
    fi

    result=$(./parse_test "$start" "$input" 2>&1)

    echo start="$start" input="$input"
    echo result="$result"
    str=$str"{\"start\": \"$start\","$'\n'
    str=$str" \"input\": \"$input\","$'\n'
    str=$str" \"result\": $result}"

    first=false
done
str="$str]"

echo "Finished entering tests for module $name"
#echo $str
#echo "$str" >> "tests/$name.json"
echo "$str" | $jq . > "$fn"
