#! /bin/bash

buildDir="build/"

builType="DEBUG"
if [ $1 = "RELEASE" ]; then
    builType=$1
fi

cmake -H. -B$buildDir -G "Unix Makefiles" -D CMAKE_BUILD_TYPE=$builType