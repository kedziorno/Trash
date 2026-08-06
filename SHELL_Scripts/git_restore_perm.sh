#!/bin/sh
for i in `git status -uno | grep -v "modified content" | awk -F ':' '{print $2}'`; do
  diff_old="`git diff $i | grep "old mode"`";
  if [ x"$diff_old" != x"" ]; then
    git checkout $i
  fi
done

