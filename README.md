# libHBase-1.0.0

"C" APIs for HBase.

    hbase-0.94
    hadoop-1.0.3
    java8

移除了对 [gtest](http://googletest.googlecode.com/files/gtest-1.7.0.zip) 的下载 !

## Building libHBase

```
  sh install.sh
```

This will build the tarball (libhbase-1.0-SNAPSHOT.tar.gz) containing the headers, shared library and the jars in the `target` directory with the following structure.

```
+---bin
+---conf
+---include
|   \---hbase
+---lib
|   \---native
\---src
    \---examples
        \---async
```

The headers can be found under `include` folder while the shared library to link against is under `lib/native`.

## Building and Running Unit Tests
libHBase uses [GTest](https://code.google.com/p/googletest/) as the test framework for unit/integration tests. During the build process, it automatically downloads and build the GTest. You will need to have `cmake` installed on the build machine to build the GTest framwork.

Runnig the unit tests currently requires you to set `LIBHBASE_ZK_QUORUM` to a valid HBase Zookeeper quorum. The default is `"localhost:2181"`. This can be either set as an environment variable or in [this configuration file](src/test/resources/config.properties).
```
LIBHBASE_ZK_QUORUM="<zk_host>:<zk_port>,..." mvn integration-test
```

## Building Applications with libHBase
For examples on how to use the APIs, please take a look at [this sample source](src/examples/async/example_async.c).

As the library uses JNI, you will need to have both `libhbase` and `libjvm` shared libraries in your application's library search path. The jars required for the library can be specified through either of the environment variables `CLASSPATH` or `HBASE_LIB_DIR`. Custom JVM options, for example `-Xmx`, etc can be specified using the environment variable `LIBHBASE_OPTS`.

You may find libjvm.so on linux in below path:

    $JAVA_HOME/jre/lib/amd64/server/libjvm.so

## Performance Testing
A performance test is included with the library which currently support sequential/random gets and puts. You can run the tests using this [shell script](bin/perftest.sh).


## Using the libhbase Library

### refer:

[mapr](https://mapr.com/docs/51/HBase/Using-the-libhbase-Librar_28216980.html)

   - create hbase table:

        	# hbase shell
        	# hbase(main)> create 'libhbase_test', {NAME => 'f1', VERSIONS => 2},{NAME => 'f2', VERSIONS => 2}
        	# hbase(main)> describe 'libhbase_test'
        	# hbase(main)> put 'libhbase_test', 'rowkey001', 'f1:col1', 'value01'
        	# hbase(main)> get 'libhbase_test', 'rowkey001'
        	# hbase(main)> scan 'libhbase_test'

    - build and run:

          mvn clean
          sh install.sh

          cd src/examples
          sh build-hello_hbase.sh

    - check table rows:
    
        	hbase(main)> scan 'libhbase_test'


### notes:

    @jnihelper.cc(ParseJavaException#438): Java exception: java.lang.IllegalArgumentException

    Not a host:port pair: PBUF

    项目包含的 jar 包和 hbase 服务器的版本不一致，将引用的 jar 包改成对应的版本即可 !
