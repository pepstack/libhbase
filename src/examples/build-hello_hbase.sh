#!/bin/bash
#
# ... @jnihelper.cc(ParseJavaException#438): Java exception: java.lang.IllegalArgumentException
#     Not a host:port pair: PBUF
#
#    项目包含的 jar 包和 hbase 服务器的版本不一致，将引用的 jar 包改成对应的版本即可 !
#
##############################################################################
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

##############################################################################
# hbase shell
# hbase(main)> create 'libhbase_test', {NAME => 'f1', VERSIONS => 2},{NAME => 'f2', VERSIONS => 2}
# hbase(main)> describe 'libhbase_test'
# hbase(main)> put 'libhbase_test', 'rowkey001', 'f1:col1', 'value01'
# hbase(main)> get 'libhbase_test', 'rowkey001'
# hbase(main)> scan 'libhbase_test'
##############################################################################
jdk_home_dir=$(echo $JAVA_HOME)

libhbase_ver="libhbase-1.0-SNAPSHOT"
libjvm_dir="$jdk_home_dir/jre/lib/amd64/server"

echo "* clean old target: $_cdir/hello_hbase"
rm -rf $_cdir/hello_hbase
echo -e "  ok.\n"


echo "* build hello_hbase.c"
cd $_cdir && gcc -o hello_hbase hello_hbase.c \
    -I$_cdir/../../target/$libhbase_ver/include \
    -L$_cdir/../../target/$libhbase_ver/lib/native -lhbase \
    -L$libjvm_dir -ljvm \
    -lpthread \
    -lrt
echo -e "  ok.\n"


echo "* run target: $_cdir/hello_hbase"
LD_LIBRARY_PATH=$_cdir/../../target/$libhbase_ver/lib/native:$libjvm_dir
HBASE_LIB_DIR=$_cdir/../../target/$libhbase_ver/lib

# Environment variable HBASE_CONF_DIR not set!
HBASE_CONF_DIR="/etc/hbase/conf"

#ZK_QUORUM="ha06.ztgame.com:2181,ha07.ztgame.com:2181,ha08.ztgame.com:2181"
ZK_QUORUM="localhost:2182"

export LD_LIBRARY_PATH && \
export HBASE_LIB_DIR && \
export HBASE_CONF_DIR && \
$_cdir/hello_hbase "$ZK_QUORUM"

echo -e "  ok.\n"

exit 0