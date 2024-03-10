#include <iostream>
#include <string>
#include <cstring>
#include <zmq.hpp>
#include <cerrno>
#include <thread>

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


// server thread
void zmq_server() {
    //creating the context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REP);
    socket.bind("tcp://*:5554");

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

        // sleep
        sleep(1);
    }
}


// client thread
void zmq_client() {
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
}


int main() {
    std::thread server_thread(zmq_server);
    std::thread client_thread(zmq_client);

    server_thread.join();
    server_client.join();

    return 0;
}
