#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#pragma comment (lib, "Ws2_32.lib")

struct Player {
	int score;
	std::string name;
};

void PrintRanking(Player ranking[]) {
	for (int i = 0; i < 10; i++) {
		cout << i+1 << " - Name: " << ranking[i].name;
		cout << " Score: " << ranking[i].score << endl;
	}
}

void OrdenarArray(Player ranking[]) {
	int auxScore;
	std::string auxName;
	for (int a = 0; a < 30; a++) {
		for (int i = 0; i < 10; i++) {
			if (ranking[i].score < ranking[i + 1].score) {
				auxScore = ranking[i + 1].score;
				auxName = ranking[i + 1].name;
				ranking[i + 1].score = ranking[i].score;
				ranking[i + 1].name = ranking[i].name;
				ranking[i].score = auxScore;
				ranking[i].name = auxName;
			}
		}
	}
}

void ReadTextFile() {
	Player ranking[10];
	std::string lectura;
	ifstream fEntrada;
	fEntrada.open("ranking.txt", ios::in);
	if (fEntrada.fail()) {
		cout << "<WARNING> File can not be opened" << endl;
	}
	else {
		bool state = false;
		int i = 0;
		for(int a = 0; a < 20; a++) {
			getline(fEntrada, lectura);
			if (state == false) {
				ranking[i].name = lectura;
				state = true;
			}
			else if (state == true) {
				ranking[i].score = std::stoi(lectura,nullptr,10);
				state = false;
				i++;
			}
		}
	}
	fEntrada.close();
	OrdenarArray(ranking);
	PrintRanking(ranking);
}



void WriteTextFile() {
	Player ranking[10];
	for (int i = 0; i < 10; i++) {
		ranking[i].name = "Test";
		ranking[i].score = 1234;
	}
	ofstream fSalida;
	fSalida.open("ranking.txt", ios::out);
	if (fSalida.fail()) {
		cout << "<WARNING> File can not be opened" << endl;
	}
	else {
		for (int i = 0; i < 10; i++) {
			fSalida << ranking[i].name << "\n" << ranking[i].score << endl;
		}
	}
	fSalida.close();
}

void server() {
	ReadTextFile();
	//WriteTextFile();
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
		//cout << bufer << endl;
	}
	else {
		cout << "INFO: Recv wrong" << endl;
	}
	
	

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