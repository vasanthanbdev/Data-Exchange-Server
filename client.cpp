#include<iostream>
#include<string>
#include<zmq.hpp>

using namespace std;

int main() {
    zmq::context_t context(2);
    zmq::socket_t socket(context, ZMQ_REQ);
    socket.connect("tcp://localhost:5555");

    //get the input from the user
    cout << "Enter a command and absolute path if required: ";
    string input;
    getline(cin, input);

    //prepare the message
    zmq::message_t request(input.size());
    memccpy(request.data(), input.c_str(), '\0', input.size());
    
    // send the request
    socket.send(request, zmq::send_flags::none);

    // print the recieved output
    zmq::message_t reply;
    socket.recv(reply, zmq::recv_flags::none);
    cout << reply.to_string() << endl;

    return 0;
}