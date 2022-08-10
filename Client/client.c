#include <stdio.h> // for "printf()"
#include <stdlib.h> // for "atoi()"
#include <string.h> // for "bzero()"
#include <unistd.h> // for "close()"
#include <sys/socket.h> // for "socket(), connect(), send() and recv()"
#include <arpa/inet.h> // for "sockaddr_in and inet_addr()"

#define MAXSIZE 9192

int main(int argc, char *argv[]){

	//Configuration of IP address & port number
	int port = atoi(argv[1]);
	char *ip = argv[2];

	// Declaration of variables for the exchange
	char cmd[MAXSIZE];
	char strings_msg[MAXSIZE+1];

	// Declaration of variables for the socket programming
	int client_socket, client_state;
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);

	struct timeval tv;
	tv.tv_sec = 2;
	tv.tv_usec = 0;

	// Check if the user put the arguments
	if (argc != 3) {
         printf("You need to specify port number and IP address ! \n");
         exit(1);
     }

	// Initialization of the socket
	client_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (client_socket < 0){
		perror ("[-] Socket error");
		exit (1);
	}

	// Check if the client can connect with the server
	if ((client_state = connect(client_socket, (struct sockaddr*)&addr, sizeof(addr))) < 0) {
		printf("[-] Connection Failed ! Wrong port number or IP address. \n");
		return -1;
	}

	printf("[+] Connected to the server.\n");

	// Loop for exchange between Client & Server
	while(1){

		// Put the value of the memory at 0
		bzero(cmd, MAXSIZE);

		printf("[+] Enter a command : ");

		// Check if the command = "quit" for stopping and closing the connection
		if (strcmp(fgets(cmd, MAXSIZE, stdin),"quit\n") == 0){
			printf("[+] Connection with the server closed !\n");
			close(client_socket);
			exit(0);
		}

		// Send the command to the server
		else {
			printf("\n");
			send (client_socket, cmd, strlen(cmd), 0);
			sleep(1);

			setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

			// Receive the result of the command
			int bytes_of_socket  = recv(client_socket, cmd, sizeof(cmd),0);
			strncpy(strings_msg, cmd, bytes_of_socket);
			strings_msg[bytes_of_socket] = '\0';
			printf("%s \n",strings_msg );
		}
		
	}

	exit(0);
}