#!/bin/bash
#######################################
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

libhbase_ver="libhbase-1.0-SNAPSHOT"

echo "* clean old target: $_cdir/hello_hbase"
rm -rf $_cdir/hello_hbase
echo -e "  ok.\n"


echo "* build hello_hbase.c"
cd $_cdir && gcc -o hello_hbase hello_hbase.c \
    -I$_cdir/../../target/$libhbase_ver/include \
    -L$_cdir/../../target/$libhbase_ver/lib/native -lhbase \
    -L/usr/local/java/jre/lib/amd64/server -ljvm \
    -lpthread \
    -lrt
echo -e "  ok.\n"


echo "* run target: $_cdir/hello_hbase"
LD_LIBRARY_PATH=$_cdir/../../target/$libhbase_ver/lib/native:/usr/local/java/jre/lib/amd64/server
HBASE_LIB_DIR=$_cdir/../../target/$libhbase_ver/lib

# Environment variable HBASE_CONF_DIR not set!

export LD_LIBRARY_PATH && export HBASE_LIB_DIR && $_cdir/hello_hbase

echo -e "  ok.\n"

exit 0