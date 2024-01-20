#include <iostream>
#include <string>
#include <cstring>
#include <zmq.hpp>
#include <cerrno>

using namespace std;


// function to execute the command and get the output
string execute_command(const char* command) {
    char buffer[256];
    string result = "";
    FILE* pipe = popen(command, "r");

    if (!pipe) throw runtime_error("popen() failed!");
    
    try {
        while (fgets(buffer, sizeof(buffer), pipe) != NULL) { //usign fgets to read the pipe line by line
            result+=buffer;                                  // and store it in buffer then update result
        }
    }
    catch(...) {
        pclose(pipe);
        throw;
    }
    
    pclose(pipe);
    return result;
}



// get file listing
string get_file_listing(string command){
    string output;
    if (command.empty()){
        output = "Error: Please enter the absolute path";
    }
    else {
        // execute the command and get output
        try {
            output = execute_command(command.c_str());
        } catch(...) {
            if (errno == EINVAL) {
                output = strerror(errno);
            }
        }
         
    }

    return output;
}




int main() {
    //creating the context and socket
    zmq::context_t context (2);
    zmq::socket_t socket (context, ZMQ_REP);
    socket.bind("tcp://*:5555");

    while (true) {
        //receive the request
        zmq::message_t request;
        socket.recv(request, zmq::recv_flags::none);
        cout << "Recieved " << request.to_string() << endl;

        // get the optput
        string command = request.to_string();
        string output = get_file_listing(command);

        
        // Prepare a reply message with the command output
        zmq::message_t reply(output.size());
        memcpy(reply.data(), output.c_str(), output.size());

        //sending the reqly message
        socket.send(reply, zmq::send_flags::none);
    }

    return 0;
}