#include <WinSock2.h>
#include <stdlib.h>
#include <iostream>
#include <WS2tcpip.h>
#include <Windows.h>
#include <stdio.h>

#pragma comment(lib,"Ws2_32.lib")
using namespace std;

/*
mutex m;

#define ARRIBA     72      // CONSTANTS AMB LES FLETXES DEL TECLAT
#define IZQUIERDA  75
#define DERECHA    77
#define ABAJO      80
#define ESC        27
#define ENTER      13

int backcolor = 0;
int dir = 0;
int x = 39, y = 22;
int anteriorpx, anteriorpy;

long int punts = -5;
int contadorP = 0;
int vides = 3;

void setCColor(int color) // colores del juego
{
	static HANDLE hConsole;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole, color | (backcolor * 0x10 + 0x100));
}

int color[7] = { // tipos de colores
	0x009, //azul cllaro
	0x00E,// amarillo claro
	0x00C, //rojo claro
	0x002, //verde
	0x00B, //aqua claro
	0x005, //violeta
	0x00F //blanco brillante
};

struct fantasma {
	int fdir; //direcció del fantasma que pot prendre valors del 0-3 i pot ser inicialitzat rand() % 4 , arriba abajo izquierda derecha
	int _x, _y; // posicíó del fantasma
	int col; //color del fantasma
};

void gotoxy(int x, int y)  // funcio que posiciona el cursor a la coordenada (x,y)
{
	HANDLE hCon;
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hCon, dwPos);
}

char mapa[50][100] = {
	"                                                      ",
	"                  AXXXXXXXXXXXXXXXXXXXB AXXXXXXXXXXXXXXXXXXXB",
	"                  Y___________________Y Y___________________Y",
	"                  Y_AXXXXXB_AXXXXXXXB_Y Y_AXXXXXXXB_AXXXXXB_Y",
	"                  Y_Y     Y_Y       Y_Y Y_Y       Y_Y     Y_Y",
	"                  Y_DXXXXXC_DXXXXXXXC_DXC_DXXXXXXXC_DXXXXXC_Y",
	"                  Y________|_________|___|_________|________Y",
	"                  Y_AXXXXXB_AXB_AXXXXXXXXXXXXXB_AXB_AXXXXXB_Y",
	"                  Y_DXXXXXC_Y Y_DXXXXB   AXXXXC_Y Y_DXXXXXC_Y",
	"                  Y_________Y Y______Y   Y______Y Y_________Y",
	"                  DXXXXXXXB_Y DXXXXB_Y   Y_AXXXXC Y_AXXXXXXXC",
	"                          Y_Y AXXXXC_DXXXC_DXXXXB Y_Y        ",
	"                          Y_Y Y_________________Y Y_Y        ",
	"                  XXXXXXXXC_DXC_AXXXXXX XXXXXXB_DXC_DXXXXXXXX",
	"                 O_________|____Y      *      Y____|________o",
	"                  XXXXXXXXB_AXB_DXXXXXXXXXXXXXC_AXB_AXXXXXXXX",
	"                          Y_Y Y_________________Y Y_Y        ",
	"                          Y_Y Y_AXXXXXXXXXXXXXB_Y Y_Y        ",
	"                  AXXXXXXXC_DXC_DXXXXB   AXXXXC_DXC_DXXXXXXXB",
	"                  Y________|_________Y   Y_________|________Y",
	"                  Y_AXXXXXB_AXXXXXXB_Y   Y_AXXXXXXB_AXXXXXB_Y",
	"                  Y_DXXXB Y_DXXXXXXC_DXXXC_DXXXXXXC_Y AXXXC_Y",
	"                  Y_____Y Y_________|_____|_________Y Y_____Y",
	"                  DXXXB_Y Y_AXB_AXXXXXXXXXXXXXB_AXB_Y Y_AXXXC",
	"                  AXXXC_DXC_Y Y_DXXXXB   AXXXXC_Y Y_DXC_DXXXB",
	"                  Y_________Y Y______Y   Y______Y Y_________Y",
	"                  Y_AXXXXXXXC DXXXXB_Y   Y_AXXXXC DXXXXXXXB_Y",
	"                  Y_DXXXXXXXXXXXXXXC_DXXXC_DXXXXXXXXXXXXXXC_Y",
	"                  Y_________________|_____|_________________Y",
	"                  DXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXC",
};

*/
/*
void pintar_mapa()// Funcio que imprimeix el mapa basant-se en el mapa codificat
{
	for (int i = 0; i < 78; i++) {
		for (int j = 0; j < 30; j++) {
			gotoxy(i, j);

			if (mapa[j][i] == 'X') {
				setCColor(color[0]);
				printf("%c", 205);
			}
			if (mapa[j][i] == '_') {
				setCColor(color[1]);
				printf("%c", 250);
			}
			else if (mapa[j][i] == 'Y') {
				setCColor(color[0]);
				printf("%c", 186);
			}
			else if (mapa[j][i] == 'A') {
				setCColor(color[0]);
				printf("%c", 201);
			}
			else if (mapa[j][i] == 'B') {
				setCColor(color[0]);
				printf("%c", 187);
			}
			else if (mapa[j][i] == 'C') {
				setCColor(color[0]);
				printf("%c", 188);
			}
			else if (mapa[j][i] == 'D') {
				setCColor(color[0]);
				printf("%c", 200);
			}

		}
	}
}

void tecleig() {
	if (_kbhit()) { //detecta si se apreta alguna tecla del teclado
		char tecla = _getch(); // reconoce la tecla y la guarda el una var
		switch (tecla) {
		case ARRIBA:
			dir = 0;
			break;

		case ABAJO:
			dir = 1;
			break;

		case DERECHA:
			dir = 2;
			break;

		case IZQUIERDA:
			dir = 3;
			break;
		}
	}

}

void pintarPacman(int x, int y) {

	setCColor(color[1]);
	gotoxy(x, y);
	printf("%c", 2);


}

void borrarPacman(int x, int y) {
	gotoxy(x, y);
	printf(" ");
}
void mourePacman()
{
	m.lock();
	anteriorpx = x;
	anteriorpy = y;
	tecleig();
	borrarPacman(x, y);
	if (dir == 0) {
		if (mapa[y - 1][x] == '|' || mapa[y - 1][x] == '_' || mapa[y - 1][x] == ' ')
			y--;
	}
	else if (dir == 1) {
		if (mapa[y + 1][x] == '|' || mapa[y + 1][x] == '_' || mapa[y + 1][x] == ' ')
			y++;
	}
	else if (dir == 2) {
		if (mapa[y][x + 1] == '|' || mapa[y][x + 1] == '_' || mapa[y][x + 1] == ' ')
			x++;
		if (mapa[y][x + 1] == 'o') {
			x = 18; y = 14;
		}
	}
	else if (dir == 3) {
		if (mapa[y][x - 1] == '|' || mapa[y][x - 1] == '_' || mapa[y][x - 1] == ' ')
			x--;
		if (mapa[y][x - 1] == 'O') {
			x = 60; y = 14;
		}
	}
	if (mapa[y][x] == '_' || mapa[y][x] == '|') {
		mapa[y][x] = ' ';
		punts += 5;
		if (vides < 3) {
			contadorP += 5;
			if (contadorP == 25) {
				vides += 1;
				contadorP = 0;
			}
		}
	}
	borrarPacman(anteriorpx, anteriorpy);
	pintarPacman(x, y);
	m.unlock();
}

fantasma inicialitzarFantasma(int x, int y, int color) {
	fantasma a;
	a._y = y;
	a._x = x;
	a.col = color;
	a.fdir = rand() % 4;
	return a;
}

void pintarFantasma(fantasma* ghost) {
	setCColor(color[ghost->col]);
	gotoxy(ghost->_x, ghost->_y);
	printf("%c", 6);
}
void borrarFantasma(fantasma* ghost) {
	gotoxy(ghost->_x, ghost->_y);
	printf(" ");
}
void xocPacman(fantasma* ghost) {
	if (ghost->_x == x && ghost->_y == y || (ghost->_x == anteriorpx && ghost->_y == anteriorpy)) { //creo q esto esta incompleto, hay q comprobar mas cosas
		borrarPacman(x, y);
		x = 39;
		y = 22;
		dir = 4;
		vides--;
		punts -= 25;
		if (punts <= 25)
			punts = 0;
	}
}

void moureFantasma(fantasma* ghost, float random) {

	m.lock();
	int bolx = ghost->_x, boly = ghost->_y;


	if (mapa[ghost->_y][ghost->_x] == '*') {
		ghost->fdir = 0;
	}
	borrarFantasma(ghost);
	if (ghost->fdir == 0) {
		if (mapa[ghost->_y - 1][ghost->_x] != 'X' && mapa[ghost->_y - 1][ghost->_x] != 'A' && mapa[ghost->_y - 1][ghost->_x] != 'Y' &&
			mapa[ghost->_y - 1][ghost->_x] != 'B' && mapa[ghost->_y - 1][ghost->_x] != 'C' && mapa[ghost->_y - 1][ghost->_x] != 'D')
			ghost->_y--;
		else ghost->fdir = random;
	}
	else if (ghost->fdir == 1) {
		if (mapa[ghost->_y + 1][ghost->_x] != 'X' && mapa[ghost->_y + 1][ghost->_x] != 'A' && mapa[ghost->_y + 1][ghost->_x] != 'Y' &&
			mapa[ghost->_y + 1][ghost->_x] != 'B' && mapa[ghost->_y + 1][ghost->_x] != 'C' && mapa[ghost->_y + 1][ghost->_x] != 'D')
			ghost->_y++;
		else ghost->fdir = random;
	}
	else if (ghost->fdir == 2) {
		if (mapa[ghost->_y][ghost->_x + 1] != 'X' && mapa[ghost->_y][ghost->_x + 1] != 'A' && mapa[ghost->_y][ghost->_x + 1] != 'Y' &&
			mapa[ghost->_y][ghost->_x + 1] != 'B' && mapa[ghost->_y][ghost->_x + 1] != 'C' && mapa[ghost->_y][ghost->_x + 1] != 'D')
			ghost->_x++;

		else ghost->fdir = random;
	}
	else if (ghost->fdir == 3) {
		if (mapa[ghost->_y][ghost->_x - 1] != 'X' && mapa[ghost->_y][ghost->_x - 1] != 'A' && mapa[ghost->_y][ghost->_x - 1] != 'Y' &&
			mapa[ghost->_y][ghost->_x - 1] != 'B' && mapa[ghost->_y][ghost->_x - 1] != 'C' && mapa[ghost->_y][ghost->_x - 1] != 'D')
			ghost->_x--;
		else ghost->fdir = random;
	}
	if (ghost->_x <= 17)
		ghost->_x = 61;
	else if (ghost->_x >= 62)
		ghost->_x = 18;
	if (mapa[boly][bolx] == '_') {
		setCColor(color[1]);
		gotoxy(bolx, boly);
		printf("%c", 250);
	}
	borrarFantasma(ghost);
	pintarFantasma(ghost);
	m.unlock();
}


void marcador() {
	m.lock();

	gotoxy(5, 4);
	printf("PUNTS");
	gotoxy(5, 6);
	printf("    ");
	setCColor(color[6]);

	gotoxy(5, 6);
	printf("%ld", punts);

	setCColor(color[1]);
	gotoxy(5, 25);
	printf("VIDES");
	for (int i = 0; i <= vides; i++) {
		gotoxy(5, i + 27); printf(" ");

	}
	for (int i = 0; i < vides; i++) {
		gotoxy(5, i + 27); printf("%c", 2);

	}
	setCColor(color[2]);
	gotoxy(70, 27); printf("%c", 169);
	m.unlock();

}*/
void main() {//CLIENTE        ---------->   PORT -> 5219  IP-> 192.168.123.59
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);


	struct addrinfo *addr;
	struct addrinfo hints;
	const char bufer[] = "hola valen";
	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	getaddrinfo("192.168.1.34", "459", &hints, &addr);

	SOCKET  sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	connect(sock, addr->ai_addr, addr->ai_addrlen);

	send(sock, bufer, sizeof(bufer) / sizeof(char), 0);


	shutdown(sock, 1);
	closesocket(sock);

	WSACleanup();

	system("pause");
}

/*	fantasma ghostA = inicialitzarFantasma(41, 14, 2);
	fantasma ghostB = inicialitzarFantasma(43, 14, 3);
	fantasma ghostC = inicialitzarFantasma(40, 14, 4);
	fantasma ghostD = inicialitzarFantasma(39, 14, 5);
	pintar_mapa();
	fantasma* f1 = &ghostA;
	fantasma* f2 = &ghostB;
	fantasma* f3 = &ghostC;
	fantasma* f4 = &ghostD;


	while (vides > 0 && punts < 1950) {
		srand(time(NULL));
		thread M = thread(marcador);
		thread Pacman = thread(mourePacman);
		thread F1 = thread(moureFantasma, f1, rand() % 4);
		thread F2 = thread(moureFantasma, f2, rand() % 4);
		thread F3 = thread(moureFantasma, f3, rand() % 4);
		thread F4 = thread(moureFantasma, f4, rand() % 4);

		M.join();
		xocPacman(f1);
		xocPacman(f2);
		xocPacman(f3);
		xocPacman(f4);
		Pacman.join();

		F1.join();
		F2.join();
		F3.join();
		F4.join();

		Sleep(110);
	}
	for (int i = 0; i <= vides; i++) {
		gotoxy(5, i + 27);
		printf(" ");
	}*/

