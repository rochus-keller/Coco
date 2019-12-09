#!/bin/sh
CocoR $1.ATG > output.txt

if cmp trace.txt $1_Trace.txt
then
	echo $1_Trace passed
fi

if cmp output.txt $1_Output.txt
then
	echo $1_Compilation passed
fi

if cmp Parser.cpp $1_Parser.cpp
then
	echo $1_Parser passed
fi

if cmp Scanner.cpp $1_Scanner.cpp
then
	echo $1_Scanner passed
fi
