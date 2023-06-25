# Environment

```
export GRPC_INSTALL_DIR=<select path>
mkdir -p $GRPC_INSTALL_DIR
export PATH="$GRPC_INSTALL_DIR/bin:$PATH"
```

# Clone and build gRPC dependency

```
git clone --recurse-submodules -b v1.50.0 --depth 1 \
  --shallow-submodules https://github.com/grpc/grpc

cd grpc
mkdir -p cmake/build
pushd cmake/build
cmake -DgRPC_INSTALL=ON \
      -DgRPC_BUILD_TESTS=OFF \
      -DCMAKE_INSTALL_PREFIX=$GRPC_INSTALL_DIR \
      ../..
make -j
make install
popd
```

# Build code example
```
mkdir -p build
pushd build
cmake -DCMAKE_PREFIX_PATH=$GRPC_INSTALL_DIR ..
make -j
popd
```
