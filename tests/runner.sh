#!/bin/bash

INFILE=$1.c
STRINGS=$1.strs
EXIT_CODE_FILE=$1.ret

BINARY=$(mktemp)
OUTPUT=$(mktemp)
WD=$(dirname $0)

FAILURE=0

cleanup()
{
    rm -r $BINARY $OUTPUT
}

trap cleanup EXIT

print_out()
{
    echo "Got output:"
    echo "------------------------------------------------"
    cat $OUTPUT
    echo "------------------------------------------------"
}

gcc -o $BINARY -Wall -Wextra -std=c99 $INFILE \
    -I$WD/../src -lutest -L$WD/../src/ -L$WD/../src/.libs

if [ $? -ne 0 ]; then
    echo "Compilation failed"
    exit 1
fi

EXIT_CODE=0
if [ -f $EXIT_CODE_FILE ]; then
    EXIT_CODE=$(cat $EXIT_CODE_FILE)
fi

VALGRIND="valgrind -q --trace-children=yes"
if [ -f $1.no-valgrind ]; then
    VALGRIND=""
fi

$VALGRIND $BINARY >$OUTPUT
REAL_CODE=$?
if [ $REAL_CODE != $EXIT_CODE ]; then
    echo "Expected exit code $EXIT_CODE, got $REAL_CODE"
    print_out
    exit 1
fi

if [ -f $STRINGS ]; then
    cat $STRINGS | while read LINE; do
        if ! grep -q "$LINE" $OUTPUT; then
            echo "Failed to produce <$LINE>"
            exit 1
        fi
    done
    if [ $? -ne 0 ]; then
        FAILURE=1
    fi
fi

if [ $FAILURE -eq 0 ]; then
    echo "OK"
else
    print_out
fi
