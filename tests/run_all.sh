#!/bin/bash

WD=$(dirname $0)

TEST_LIST=$(ls $WD | egrep '^[0-9]*-[^.]*$')

for x in $TEST_LIST; do
    $WD/runner.sh $WD/$x
done
