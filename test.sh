#!/bin/bash
# test folders from https://github.com/nst/JSONTestSuite
cd "$(dirname "$0")"
rm -rf n_failed.txt y_failed.txt s_failed.txt n_success.txt y_success.txt s_success.txt
N_FILES="test_parsing/n*.json"
Y_FILES="test_parsing/y*.json"
I_FILES="test_parsing/i*.json"
for f in $N_FILES; do ./main "$(cat $f)"; if [ $? -eq 1 ]; then echo "$f" >> n_success.txt ; else echo "$f" >> n_failed.txt ; fi ;done
for f in $Y_FILES; do if ./main "$(cat $f)"; then echo "$f" >> y_success.txt ; else echo "$f" >> y_failed.txt; fi ;done
for f in $I_FILES; do if ./main "$(cat $f)"; then echo "$f" >> i_success.txt ; else echo "$f" >> i_failed.txt; fi ;done
