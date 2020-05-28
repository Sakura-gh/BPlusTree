#!/bin/bash
# read executable file name to exec_name
read -p "please enter the executable file name: " exec_name
# if exec file not exists, then find all '.cpp' file and compile and generate exec file 
# if [ ! -f ${exec_name} ]; then

# find all '.cpp' file and compile and generate exec file 
    compile="g++"
    for filename in $(ls *); do
        if [ "${filename##*.}" = "cpp" ]; then
            compile="${compile} ${filename}"
        fi
    done
    compile="${compile} -std=c++11 -o ${exec_name}"
    echo "compile...$complie"
    eval $compile

# fi

# run the exec file
eval "./$exec_name"




#if [ ! -f $filename ]; then
#    eval "g++ main.cpp API.cpp Attribute.cpp BufferManager.cpp CatalogManager.cpp Condition.cpp IndexManager.cpp Interpreter.cpp RecordManager.cpp -std=c++11 -o main"
#    echo "complier...exec...$filename"
#    eval "./main"
#else
#    echo "exec...$filename"
#    eval "./main"
#fi


