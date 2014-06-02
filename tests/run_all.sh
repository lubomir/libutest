#!/bin/bash

WD=$(dirname $0)

for x in $WD/*.c; do
    $WD/runner.sh ${x%.c}
done
