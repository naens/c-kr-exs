#!/bin/sh

# test generator

echo "Enter a module name:"
read name
str="["
first=true
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
echo $str
#echo "$str" >> "tests/$name.json"
echo "$str" | jq . >> "tests/$name.json"
 