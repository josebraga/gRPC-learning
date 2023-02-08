# Environment
export MY_INSTALL_DIR=$HOME/workspace/code/grpc/target
mkdir -p $MY_INSTALL_DIR
export PATH="$MY_INSTALL_DIR/bin:$PATH"

# Clone gRPC
git clone --recurse-submodules -b v1.50.0 --depth 1 --shallow-submodules https://github.com/grpc/grpc $HOME/workspace/code/grpc/

# Build gRPC
```
$ cd $HOME/workspace/code/grpc/grpc
$ mkdir -p cmake/build
$ pushd cmake/build
$ cmake -DgRPC_INSTALL=ON \
      -DgRPC_BUILD_TESTS=OFF \
      -DCMAKE_INSTALL_PREFIX=$MY_INSTALL_DIR \
      ../..
$ make -j 4
$ make install
$ popd
```

# Build my example
```
$ cd $HOME/workspace/code/grpc/gRPC-learning
$ mkdir -p build
$ pushd build
$ cmake -DCMAKE_PREFIX_PATH=$MY_INSTALL_DIR ..
$ make -j 4
```