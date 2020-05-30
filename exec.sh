#!/bin/bash
# read executable file name to exec_name
read -p "please enter the executable file name: " exec_name
# if exec file not exists, then find all '.cpp' file and compile and generate exec file 
# if [ ! -f ${exec_name} ]; then

# find all '.cpp' file and compile and generate exec file 
    compile="g++"
    for filename in $(ls .); do
        if [ "${filename##*.}" = "cpp" ]; then
            compile="${compile} ${filename}"
        fi
    done
    compile="${compile} -std=c++11 -o ${exec_name}.o"
    echo "$compile"
    echo "compile..."
    eval $compile

# fi

# run the exec file
echo "./${exec_name}.o"
echo -e "execute...\n"
eval "./${exec_name}.o"
