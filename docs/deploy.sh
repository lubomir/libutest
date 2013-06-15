#!/bin/bash

set -e

branch=$(git rev-parse --abbrev-ref HEAD)
DIR=$(dirname $0)
tempdir=$(mktemp -d)

if [ ! -d .git ]; then
    echo "Not in toplevel directory!"
    exit 1
fi

if ! git diff-index --quiet HEAD --; then
    echo "Repo is not clean!"
    exit 1
fi

cp -r "$DIR/_site" "$tempdir"

git checkout gh-pages
rm -rf *
mv $tempdir/_site/* .
git add * || echo "adding failed"
git commit -q -a || echo "No changes!"
git checkout $branch

rm -rf "$tempdir"
