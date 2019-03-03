#!/bin/bash

count=0
n=0
tsize=0

for f in src/*.cpp
do
	count=`expr $(wc -l < $f) + $count`
	n=`expr $n + 1` 
	tsize=`expr $tsize + $(stat -c%s "$f")`
done

for f in includes/*.hpp
do
	count=`expr $(wc -l < $f) + $count`
	n=`expr $n + 1`
	tsize=`expr $tsize + $(stat -c%s $f)`
done

echo $count "Lines"
echo $n " Files" 
echo `expr $tsize / 1000` " Kb"
echo "Aprox. " `expr $count / $n` " lines per file."
