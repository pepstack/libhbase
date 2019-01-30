#!/bin/bash
#
# build libhbase on linux
#   https://github.com/mapr/libhbase
######################################################################
_file=$(readlink -f $0)
_cdir=$(dirname $_file)
_name=$(basename $_file)

# Treat unset variables as an error
set -o nounset

# Treat any error as exit
set -o errexit

# Set characters encodeing
#   LANG=en_US.UTF-8;export LANG
LANG=zh_CN.UTF-8;export LANG

mkdir -p ~/.m2/repository/.cache/maven-download-plugin/

cp $_cdir/gtest-1.7.0.zip ~/.m2/repository/.cache/maven-download-plugin/

mvn install -DskipTests
