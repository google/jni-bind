#!/bin/bash

################################################################################
# Copyright 2021 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
################################################################################
#
# Prints the contents of the file suitable to be stitched into the one header.
#
# Specifically, there is no need for repeating the Apache license, nor any need
# to repeat the #ifdef guards (there will just be one for the whole file.
#
# $1 File path.
# $2 Directory of constraint (filters to to targets only under this dir).
# $3 Path to release leader .inc.
# $4 Path to release trailer .inc.
# $@ All arguments.

reverse() {
  args=(${@})
  args_size=${#@}

  declare -a ret=()
  for (( i=1; i<=$args_size; i++ ))
  do
    ret+=("${args[$args_size-$i]}")
  done

  echo ${ret[@]}
}

################################################################################
# $1 The directory of constraint
# $@ All the labels in the directory.
explode_all_deps() {
  args=(${@})
  directory_of_constraint=$1

  declare -a ret=()
  for (( i=0; i<=${#@}; i++ ))
  do
     grep -q $directory_of_constraint <<< ${args[$i]}
     if  [[ $? -ne 0 ]]; then
       continue
     fi

     grep -q "\.h$" <<< ${args[$i]}
     if  [[ $? -ne 0 ]]; then
       continue
     fi

     ret=$(sed "s/:/\//" <<< ${args[$i]})
     ret=$(sed "s/\/\//.\//" <<< $ret)
     echo $ret
  done
}

# $@ A list of all the headers to expand inline.
paste_header_contents_from_filenames() {
  arr=(${@})
  arr_size=${#arr[@]}
  for (( i=0; i<$arr_size; i++ ))
  do
    # Skip the Apache license since it's always the same length this works.
    # Also, skip the ifdef and defin guards which always lead the file.
    tail -n +19 ${arr[$i]} > /tmp/jni_bind_file_preprocess

    # Chop off the endif which is always in the same location at the end.
    # (i.e. #endif // namespace jni{::metaprogramming} statement).
    head -n -1 /tmp/jni_bind_file_preprocess > /tmp/jni_bind_file_preprocess_2

    # Remove local include statements (these will be inlined by this script).
    sed -i "s/#include \".*//" /tmp/jni_bind_file_preprocess_2

    # Remove excess whitespace in the file.
    cat -s /tmp/jni_bind_file_preprocess_2 > /tmp/jni_bind_file_preprocess_3

    cat /tmp/jni_bind_file_preprocess_3
  done
}

################################################################################
# Main.
################################################################################
all_targets=$(cat $1)
all_headers="$(sed "s/[0-9]\+ //g" <<< $all_targets)"
all_headers=$(reverse $all_headers)
all_headers=$(explode_all_deps $2 $all_headers)
all_headers_inline=$(paste_header_contents_from_filenames ${all_headers[@]})

cat $3
echo "$all_headers_inline"
cat $4
