#include <iostream>
#include <sstream>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

static void usage();

int main(int argc, char *argv[])
{
    if (argc > 1 && *(argv[1]) == '-')
    {
        std::cout << "please enter the host you are wanting to connect to as a 1st arg." << std::endl;
        exit(1);
    }

    if (argc > 1 && *(argv[2]) == '-')
    {
        std::cout << "please enter the port number you are wanting to connect to. as a 2nd arg" << std::endl;
        exit(1);
    }

    // Create socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }

    // Fill in server IP address
    struct sockaddr_in server;
    int serverAddrLen;
    bzero( &server, sizeof( server ) );


    char* peerHost = argv[1]; // change to ===> argv[1] once ready for CSEgrid;

    // Resolve server address (convert from symbolic name to IP number)
    struct hostent *host = gethostbyname(peerHost);
    if (host == NULL)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }

    server.sin_family = AF_INET;
    short peerPort = (short) atoi(argv[2]); // change to ====> argv[2] once ready for CSEgrid

    server.sin_port = htons(peerPort);

    // Print a resolved address of server (the first IP of the host)
    std::cout << "connecting to server: " << peerHost << std::endl;

    // Write resolved IP address of a server to the address structure
    memmove(&(server.sin_addr.s_addr), host->h_addr_list[0], 4);

    // Connect to the remote server
    int res = connect(client_socket, (struct sockaddr*) &server, sizeof(server));
    if (res < 0)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }

    char buffer[1024];
    res = read(client_socket, buffer, 1024);
    if (res < 0)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }

    std::cout << "Received: " << buffer;

    while(true) {

        bzero(buffer, sizeof(buffer)); //zero out memset
        int n = 0;
        std::cout << "Please enter your phrase: ";
        while ((buffer[n++] = getchar()) != '\n');
        write(client_socket, buffer, 1024);

        //creating string representation of buffer to check for exiting later.
        size_t length = strlen(buffer);
        char temp[length]; //buffer length
        memset(temp, '\0', length +1);
        std::string bufferstring = strncpy(temp, buffer, length-1); // -1 removes the \n at the end


        char bufferrecv[1024];
        res = read(client_socket, bufferrecv, 1024);
        if (res < 0)
        {
            std::cerr << "Error: " << strerror(errno) << std::endl;
            exit(1);
        }

        std::cout << "Received from server: " << bufferrecv << std::endl;

        if (bufferstring == "fin"){
            break;
        }

        bzero(bufferrecv, sizeof(bufferrecv)); //zero out memset
    }
    close(client_socket);
    return 0;
}

