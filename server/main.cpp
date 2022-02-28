#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <iostream>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>


int main(int argc, char *argv[])
{

    if (argc > 1 && *(argv[1]) == '-')
    {
        std::cout << "Incorrect arguments. please provide <port number>";
        exit(1);
    }

    int listenPort = atoi(argv[1]);


    // Create a socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }

    // create server socket and assign port to it
    struct sockaddr_in myaddr;
    memset(&myaddr, 0, sizeof(struct sockaddr_in));
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(listenPort); //listen port from above
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind a socket to the address
    int res = bind(server_socket, (struct sockaddr*) &myaddr, sizeof(myaddr));
    if (res < 0)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }

    // Set the "LINGER" timeout to zero, to close the listen socket.
    //This helps when debugging and stopping/starting the program alot
    struct linger linger_opt = { 1, 0 }; // Linger active, timeout 0
    setsockopt(server_socket, SOL_SOCKET, SO_LINGER, &linger_opt, sizeof(linger_opt));

    // setup listening
    res = listen(server_socket, 1);
    if (res < 0)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }

    std::cout << "Server setup and listening on port: " << listenPort << std::endl;

    //accept a connection and store the peer IP and info in a struct
    struct sockaddr_in peeraddr;
    socklen_t peeraddr_len;
    int client = accept(server_socket, (struct sockaddr*) &peeraddr, &peeraddr_len);
    if (client < 0)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }

    std::cout << "New client has connected." << std::endl;

    write(client, "Connected to Server.\n", 1024);

    while(true){
        char buffer[1024];
        res = read(client, buffer, 1024);
        if (res < 0) {
            std::cerr << "Error: " << strerror(errno) << std::endl;
            exit(1);
        }
        buffer[res] = 0;
        std::cout << "message from client: " << buffer;

        //creating string representation of buffer to check for exiting later.
        size_t length = strlen(buffer);
        char temp[length]; //buffer length
        memset(temp, '\0', length +1);
        std::string bufferstring = strncpy(temp, buffer, length-1); // -1 removes the \n at the end

        // below creates the reversed string to be sent back
        char* response = (char*)malloc((length + 1) * sizeof(char));
        response[1] = '\0';
        int i;
        for (i = 0; i < length; i++){
            response[i] = buffer[length - 1 - i];
        }

        bzero(buffer, sizeof(buffer)); //zero out buffer
        std::cout << "Sending back: " << response << std::endl;
        write(client, response, 1024);

        //check if time to exit

        if (bufferstring == "fin"){
            break;
        }

    }

    close(client);
    return 0;
}