cmake_minimum_required(VERSION 3.15)
project(gRPC_learning C CXX)

set(_PROTOBUF_LIBPROTOBUF protobuf)
set(_REFLECTION grpc++_reflection)
set(_GRPC_GRPCPP grpc++)

list(APPEND CMAKE_PREFIX_PATH "/home/jb/workspace/code/grpc/target/")
find_program(_PROTOBUF_PROTOC protoc)
find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)

set(GRPC_DIR Default CACHE STRING "Path to grpc lib and include")
if(GRPC_DIR STREQUAL "Default")
    message(STATUS "GRPC_DIR not set, using default of ${PROJECT_SOURCE_DIR}/../target")
    set(GRPC_DIR ${PROJECT_SOURCE_DIR}/../target)
else()
    message(STATUS "GRPC_DIR set to ${GRPC_DIR}")
endif()
set(CMAKE_PREFIX_PATH ${GRPC_DIR})
set(GRPC_INCLUDE_DIR ${GRPC_DIR}/include)
set(GRPC_LIB_DIR ${GRPC_DIR}/lib)


# Proto Generation
# Proto file
get_filename_component(hw_proto "specs.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)

# Generated sources
set(hw_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/specs.pb.cc")
set(hw_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/specs.pb.h")
set(hw_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/specs.grpc.pb.cc")
set(hw_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/specs.grpc.pb.h")
add_custom_command(
      OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${hw_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${hw_proto}"
      DEPENDS "${hw_proto}")

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${GRPC_INCLUDE_DIR})
link_directories(${GRPC_LIB_DIR})

# hw_grpc_proto
add_library(hw_grpc_proto
  ${hw_grpc_srcs}
  ${hw_grpc_hdrs}
  ${hw_proto_srcs}
  ${hw_proto_hdrs})

target_link_libraries(hw_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})

find_package(gRPC CONFIG REQUIRED)
add_executable(main main.cc)
target_link_libraries(main hw_grpc_proto gRPC::grpc++)
