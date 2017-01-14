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
		cout << i << " --- " << ranking[i].name;
		cout << " --- " << ranking[i].score << endl;
	}
}

void OrdenarRanking(Player ranking[]) {
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
void WriteRankingFile(Player ranking[]) {
	ofstream fSalida;
	fSalida.open("ranking/ranking.txt", ios::out);
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
void ReadRankingFile(Player ranking[], int playerScore, std::string playerName) {
	cout << "<RANKING OPENED>" << endl;
	std::string lectura;
	ifstream fEntrada;
	fEntrada.open("ranking/ranking.txt", ios::in);
	if (fEntrada.fail()) {
		cout << "<WARNING> File can not be opened" << endl;
	}
	else {
		bool state = false;
		int i = 0;
		for (int a = 0; a < 20; a++) {
			getline(fEntrada, lectura);
			if (state == false) {
				ranking[i].name = lectura;
				state = true;
			}
			else if (state == true) {
				ranking[i].score = std::stoi(lectura, nullptr, 10);
				state = false;
				i++;
			}
		}
	}
	fEntrada.close();
	OrdenarRanking(ranking);

	Player newUser;
	newUser.score = playerScore;
	newUser.name = playerName;

	for (int i = 0; i < 10; i++) {
		if (newUser.score > ranking[i].score) {
			for (int x = 9; x > i; x--) {
				ranking[x] = ranking[x - 1];
			}
			ranking[i] = newUser;
			break;
		}
	}

	PrintRanking(ranking);
	WriteRankingFile(ranking);
}

void WriteUserFile(Player userRanking[], std::string userName) {
	ofstream fSalida;
	std::string X = "users/", Y = userName, Z = ".txt", userRoute = X + Y + Z;
	fSalida.open(userRoute, ios::out);
	if (fSalida.fail()) {
		cout << "<WARNING> File can not be opened" << endl;
	}
	else {
		for (int i = 0; i < 10; i++) {
			fSalida << userRanking[i].name << "\n" << userRanking[i].score << endl;
		}
	}
	fSalida.close();
}
void ReadUserFile(std::string userName, int userScore, Player userRanking[]) {
	ifstream fEntrada;

	std::string X = "users/", Y = userName, Z = ".txt", userRoute = X + Y + Z;
	fEntrada.open(userRoute, ios::in);
	if (fEntrada.fail()) {
		cout << "<WARNING> File can not be opened" << endl;
		cout << "          User doesn't exist" << endl;
		cout << "          Creating a new file" << endl;
		Player newUser;
		newUser.score = userScore;
		newUser.name = userName;
		Player defaultUser;
		defaultUser.score = 000;
		defaultUser.name = "---";
		for (int i = 0; i < 10; i++) {
			if (i == 0) {
				userRanking[i] = newUser;
			}
			else {
				userRanking[i] = defaultUser;
			}
		}
		WriteUserFile(userRanking, userName);
		cout << "<RANKING> <USER> <" << userName << ">" << endl;
		PrintRanking(userRanking);
	}
	else {
		std::string lectura;
		bool state = false;
		int i = 0;
		for (int a = 0; a < 20; a++) {
			getline(fEntrada, lectura);
			if (state == false) {
				userRanking[i].name = lectura;
				state = true;
			}
			else if (state == true) {
				userRanking[i].score = std::stoi(lectura, nullptr, 10);
				state = false;
				i++;
			}
		}
		fEntrada.close();
		OrdenarRanking(userRanking);

		Player newUser;
		newUser.score = userScore;
		newUser.name = userName;

		for (int i = 0; i < 10; i++) {
			if (newUser.score > userRanking[i].score) {
				for (int x = 9; x > i; x--) {
					userRanking[x] = userRanking[x - 1];
				}
				userRanking[i] = newUser;
				break;
			}
		}
		cout << "<RANKING> <USER> <" << userName << ">" << endl;
		PrintRanking(userRanking);
		WriteUserFile(userRanking, userName);
	}
}

void CutTheRope(char bufer[], std::string &name, int &score) {
	std::string tempScore;
	int i = 0;
	for (i = 0; i < sizeof(bufer) - 1; i++) {
		if (bufer[i] != '/') {
			name = name + bufer[i];
		}
		else if (bufer[i] == '/') {
			break;
		}
	}

	for (i = i + 1; i < sizeof(bufer) - 1; i++) {
		if (bufer[i] != '\0') {
			tempScore = tempScore + bufer[i];
		}
	}

	score = std::stoi(tempScore, nullptr, 10);

}

void server() {
	cout << "<SERVER> Waiting" << endl;
	Player ranking[10];
	Player userRanking[10];

	std::string name;
	int score;

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
		cout << "<SERVER> Accept correct" << endl;
	}
	else {
		cout << "<SERVER> Acept wrong" << endl;
	}

	int i = recv(socKrec, bufer, 512, 0);
	bufer[i - 1] = '\0';
	if (recv) {
		cout << "<SERVER> Recv correct" << endl;
		cout << "<CLIENT SEND> Buffer: " << bufer << endl;
		CutTheRope(bufer, name, score);
		cout << "<SERVER> Name: " << name << endl;
		cout << "<SERVER> Score: " << score << endl;
	}
	else {
		cout << "INFO: Recv wrong" << endl;
	}

	shutdown(socKrec, SD_RECEIVE);
	closesocket(socKrec);
	WSACleanup();

	ReadRankingFile(ranking, score, name);
	ReadUserFile(name, score, userRanking);
}

int main(int argc, char **argv) { //port 5219
	for (;;) {
		server();
	}
	return 0;
}