#! /bin/bash

buildDir="build/"

cmake -H. -B$buildDir -G "Unix Makefiles"

cd $buildDir
make
