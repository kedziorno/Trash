#!/bin/sh

git_list="`find $1 -iname ".git" -type d -print`"

for i in ${git_list}; do
  dir=`dirname ${i}`
  echo "cd ${dir}"
   cd ${dir}
  echo "create tar for .git"
   tar -czf git.tar.gz .git
  echo "remove .git"
   rm -rf .git
  echo "run fdupes on ."
   fdupes -r -S -d -N -f .
  echo "restore tar from git.tar.gz"
   tar -zxf git.tar.gz
  echo "delete git.tar.gz"
   rm -rf git.tar.gz
done

echo "Done."

