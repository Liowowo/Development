#include <stdio.h> // for "printf()"
#include <stdlib.h> // for "atoi()"
#include <string.h> // for "memset()"
#include <unistd.h> // for "close()"
#include <sys/socket.h> // for "socket(), connect(), send() and recv()"
#include <arpa/inet.h> //for "sockaddr_in and inet_addr()"

#define MAXSIZE 1024

void exchange (int client_socket){

	char cmd[MAXSIZE];
	char buffer[MAXSIZE];
	char strings_msg[] = "";
	FILE *output;
	int i;

	// Loop for exchange between Client & Server
	for(;;){

		bzero(cmd,MAXSIZE);

		// Receve and convert the socket from the client
		int bytes_of_socket  = recv(client_socket, cmd, sizeof(cmd),0);
		strncpy(strings_msg, cmd, bytes_of_socket);
		strings_msg[bytes_of_socket] = '\0';
		printf("%s \n",strings_msg );

		// Execute Linux command & create an output of this command
		output = popen (strings_msg, "r");
		if (output == NULL){
			fputs("Failed to execute command, try again ! \n", stderr);
		}

		else {
			int count = 1;
			while (fgets(buffer, MAXSIZE-1, output) != NULL){
				printf("OUTPUT[%d] : %s", count, buffer);
				count ++;
			}
		}
	}
}

int main(int argc, char *argv[]){

	//Configuration of IP address & port number
	int port = atoi(argv[1]);
	char *ip = argv[2];

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
	printf("[+] Bind to port number: %d and IP address: %s \n", port, ip);

	listen(server_socket,5); 
	printf("listening... \n");

	addr_size = sizeof(client_addr);
	client_socket = accept(server_socket, (struct sockaddr*)&client_addr,&addr_size);
	printf("[+] Client connected !\n");
	
	exchange(client_socket);

	return 0;
}