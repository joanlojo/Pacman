#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;
#pragma comment (lib, "Ws2_32.lib")
void server() {
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	struct addrinfo addrInit;
	struct addrinfo *addrSender;
	struct addrinfo *addrDest;

	char bufer[512];
	ZeroMemory(&addrInit, sizeof(addrInit));
	addrInit.ai_family = AF_INET;
	addrInit.ai_socktype = SOCK_STREAM;
	addrInit.ai_flags = AI_PASSIVE;
	addrInit.ai_protocol = IPPROTO_TCP;

	getaddrinfo(NULL, "4539", &addrInit, &addrDest);


	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	bind(sock, addrDest->ai_addr, addrDest->ai_addrlen);
	listen(sock, 1); // 1 = numero de conexiones permitidas
	SOCKET socKrec = accept(sock, NULL, NULL);
	if (accept) {
		cout << "INFO: Accept correct" << endl;
	}
	else {
		cout << "INFO: Acept wrong" << endl;
	}

	int i = recv(socKrec, bufer, 512, 0);
	bufer[i - 1] = '\0';
	if (recv) {
		cout << "INFO: Recv correct" << endl;
		cout << bufer << endl;
	}
	else {
		cout << "INFO: Recv wrong" << endl;
	}
	//cout << bufer << endl;
	shutdown(socKrec, SD_RECEIVE);
	closesocket(socKrec);
	WSACleanup();
}
int main(int argc, char **argv) { //port 5219
	for (;;) {
		server();
	}
	return 0;
}