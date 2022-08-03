#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main() {

	//Initiazation of ip adress & port number
	const char *ip = "127.0.0.1";
	const int port = 4444;

	int server_socket,client_socket;

	struct sockaddr_in server_addr, client_addr;
	socklen_t addr_size;
	char buffer[1024];
	int n;

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0){
		perror ("[-] Socket error");
		exit (1);
	}

	printf("[+] TCP server socket started.\n");

	memset(&server_addr, '\0', sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = port;
	server_addr.sin_addr.s_addr = inet_addr(ip);

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
	printf("[+] Client connecte.\n");
	
	addr_size = sizeof(client_addr);
		client_socket = accept(server_socket, (struct sockaddr*)&client_addr,&addr_size);
		printf("[+] Client connecte.\n");

	//while (1){
		
	//}
	return 0;
}