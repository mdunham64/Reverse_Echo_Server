To start the program, navigate to the correct directory (client or server). Once there, type make and you will be given either a server executable or client executable pending which directory you are in. I have included an executable as well within each respective directory. 

First run the server, by typing ./server <listening_port_number>



Then run the client, by typing ./client <server_hostname> <port_number>


Once you receive the "Connected to server." message on the client you can type a message and after hitting return, the server will send back the reversed message.

To exit the program, type the phrase: "fin". This will tell the server to shutdown as well as the client.

