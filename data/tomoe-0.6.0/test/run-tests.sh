#!/bin/sh

for dict_module in $DICT_MODULES; do
    echo "tests for $dict_module"
    TOMOE_DICT_MODULE="$dict_module" $RUBY `dirname $0`/run-test.rb || exit 1
done
