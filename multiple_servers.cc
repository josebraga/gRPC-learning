#include <thread>
#include <grpcpp/grpcpp.h>
#include "specs.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using helloworld::Greeter;
using helloworld::HelloReply;
using helloworld::HelloRequest;


class GreeterServiceImpl final : public Greeter::Service {
  Status SayHello(ServerContext* context, const HelloRequest* request,
                  HelloReply* reply) override {
    std::string prefix("Hello ");
    reply->set_message(prefix + request->name());
    return Status::OK;
  }

  Status SayHelloAgain(ServerContext* context, const HelloRequest* request,
                  HelloReply* reply) override {
    std::string prefix("You Again? Hello ");
    reply->set_message(prefix + request->name());
    return Status::OK;
  }
};

int main(int argc, char** argv) {
    std::string server_address1("0.0.0.0:50051");
    GreeterServiceImpl service1;
    grpc::ServerBuilder builder1;
    builder1.AddListeningPort(server_address1, grpc::InsecureServerCredentials());
    builder1.RegisterService(&service1);
    std::unique_ptr<grpc::Server> server1(builder1.BuildAndStart());

    std::string server_address2("0.0.0.0:50052");
    GreeterServiceImpl service2;
    grpc::ServerBuilder builder2;
    builder2.AddListeningPort(server_address2, grpc::InsecureServerCredentials());
    builder2.RegisterService(&service2);
    std::unique_ptr<grpc::Server> server2(builder2.BuildAndStart());

    std::jthread server_thread1([&server1](){
      std::cout << "wait 1st thread\n";
      server1->Wait();
    });

    std::jthread server_thread2([&server2](){
      std::cout << "wait 2nd thread\n";
      server2->Wait();
    });

    std::string server_address3("0.0.0.0:50052");
    GreeterServiceImpl service3;
    grpc::ServerBuilder builder3;
    builder3.AddListeningPort(server_address3, grpc::InsecureServerCredentials());
    builder3.RegisterService(&service3);
    std::unique_ptr<grpc::Server> server3(builder3.BuildAndStart());

    std::cout << "wait 3r thread\n";
    server3->Wait();

    return 0;
}
