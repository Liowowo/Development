#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAXSIZE 1024

void exchange (int client_socket){

	char cmd[MAXSIZE];
	char strings_msg[] = "";
	FILE *output;
	int i;

	// Loop for exchange between Client & Server
	for(;;){

		bzero(cmd, MAXSIZE);

		printf("[+] Enter a command : ");
		fgets(cmd, MAXSIZE, stdin);
		printf("\n");

		if (cmd == NULL){
			fputs("Failed to execute command, try again ! \n", stderr);
		}
		
		send (client_socket, cmd, strlen(cmd), 0);
		sleep(0.5);

		int bytes_of_socket  = recv(client_socket, cmd, sizeof(cmd),0);
		strncpy(strings_msg, cmd, bytes_of_socket);
		strings_msg[bytes_of_socket] = '\0';
		printf("%s \n",strings_msg );

	}
}

int main(int argc, char *argv[]){

	//Configuration of IP address & port number
	int port = atoi(argv[1]);
	char *ip = argv[2];

	int client_socket, client_state;

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
	
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket < 0){
		perror ("[-] Socket error");
		exit (1);
	}

	if ((client_state = connect(client_socket, (struct sockaddr*)&addr, sizeof(addr))) < 0) {
		printf("[-] Connection Failed ! Wrong port number or IP address \n");
		return -1;
	}

	printf("[+] Connected to the server.\n");

	exchange(client_socket);

	exit(0);
}