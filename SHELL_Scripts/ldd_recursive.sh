#!/bin/bash

# print recursive tree on *.so files and show "not found" missing libraries

# (it's browse paths recursively in simple way,
# so the missing file may appear two or more times)
# (example run on gvim take very very long time)

ldd_recursive()
{
  IFS=$'\n'
  for ldd_list in `ldd $1 | sort | xargs -d "\n" -I _ echo _`; do
    left_pattern=${ldd_list%"=>"*}
    right_pattern=${ldd_list#*"=>"}
    if [ ${right_pattern} = " not found" ]; then
      echo "--> *${left_pattern}* not found"
      continue
    else
      omit_files=`echo ${right_pattern} | grep -v -E "(linux-vdso)|(statically linked)|(linux-gate)"`;
      if [ $? != 1 ]; then
        output="`echo ${right_pattern} | awk -F " " '{print $1}'`"
        indent=$2;
        indent="    ${indent}";
        echo "${indent}${output}"
        ldd_recursive ${output} "${indent}";
      fi
    fi
  done
}

# hash array version, todo more test

declare -A hashmap

ldd_recursive_hash()
{
  flag=""
  IFS=$'\n'
  for ldd_list in `ldd $1 | xargs -d "\n" -I _ echo _`; do
    left_pattern=${ldd_list%"=>"*}
    right_pattern=${ldd_list#*"=>"}
    if [ ${right_pattern} = " not found" ]; then
      echo "--> *${left_pattern}* not found"
      continue
    else
      omit_files=`echo ${right_pattern} | grep -v -E "(linux-vdso)|(statically linked)|(linux-gate)"`;
      if [ $? != 1 ]; then
        output="`echo ${right_pattern} | awk -F " " '{print $1}'`"
        file=`basename ${output}`
        for key in ${!hashmap[@]}; do
          if [ $key = $file ]; then
            flag="x"
          fi
        done
        if [ x"${flag}" = x"x" ]; then
          flag=""
          continue
        else
          hashmap[$file]="$file";
          indent=$2;
          indent="    ${indent}";
          echo "${indent}${file}"
          ldd_recursive_hash ${output} "${indent}";
        fi
      fi
    fi
  done
}

#ldd_recursive $1 ""
ldd_recursive_hash $1 ""

