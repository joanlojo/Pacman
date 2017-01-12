#include <WinSock2.h>
#include <stdlib.h>
#include <iostream>
#include <WS2tcpip.h>
#include <Windows.h>
#include <stdio.h>

#pragma comment(lib,"Ws2_32.lib")

using std::cout;
using std::cin;


void main() {//CLIENTE        ---------->   PORT -> 5219  IP-> 192.168.123.59
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);


	struct addrinfo *addr;
	struct addrinfo hints;
	const char bufer[] = "SEND NUDES <3";
	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	getaddrinfo("192.168.123.51", "5219", &hints, &addr);

	SOCKET  sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	connect(sock, addr->ai_addr, addr->ai_addrlen);
	send(sock, bufer, sizeof(bufer) / sizeof(char), 0);

	shutdown(sock, 1);
	closesocket(sock);

	WSACleanup();

	system("pause");
}