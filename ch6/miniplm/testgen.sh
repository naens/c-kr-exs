#!/bin/sh

# test generator

echo "Enter a module name:"
read name
str="["
first=true
while true
do
    if [ "$first" = "false" ]
    then
        str="$str,"
    fi
    echo "Enter a nonterminal:"
    read start

    echo "Enter an expression:"
    read input

    result=$(./parse_test "$start" "$input" 2>&1)

    echo start="$start" input="$input"
    echo result="$result"

    echo "Enter one more item(y/n)?"
    read answer
    echo "Your answer is \"$answer\""
    if [ "$answer" = "n" ]
    then
        break
    fi
    first=false
done

echo "Finished entering tests for module $name"
 