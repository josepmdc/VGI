#! /bin/bash

buildDir="build/"

./scripts/generate.sh $1

cmake --build $buildDir
./$buildDir/VGI
