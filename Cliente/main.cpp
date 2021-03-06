#include <WinSock2.h>
#include <stdlib.h>
#include <iostream>
#include <WS2tcpip.h>
#include <Windows.h>
#include <stdio.h>
#include <sstream>
#include <string>
//#include <windows.h>
#include <conio.h>
#include <time.h>
#include <thread>
#include <mutex>
#include <ctime>
#include <cstdio>
#pragma comment(lib,"Ws2_32.lib")
using namespace std;

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

int punts = -5;
int contadorP = 0;
int vides = 1;
string name[20];
int score[20];
int fail;
int cincuenta;
int cien;
int medio;
int entero;
int record;

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
class timer {
public:
	timer();
	void           start();
	bool           isRunning();
	unsigned long  getTime();
private:
	bool           resetted;
	bool           running;
	unsigned long  beg;
	unsigned long  end;
};
struct fantasma {
	int fdir; //direcci� del fantasma que pot prendre valors del 0-3 i pot ser inicialitzat rand() % 4 , arriba abajo izquierda derecha
	int _x, _y; // posic�� del fantasma
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
}
timer::timer() {
	resetted = true;
	running = false;
	beg = 0;
	end = 0;
}
void timer::start() {
	if (!running) {
		if (resetted)
			beg = (unsigned long)clock();
		else
			beg -= end - (unsigned long)clock();
		running = true;
		resetted = false;
	}
}
bool timer::isRunning() {
	return running;
}
unsigned long timer::getTime() {
	if (running)
		return ((unsigned long)clock() - beg) / CLOCKS_PER_SEC;
	else
		return end - beg;
}
void CutTheRopeRanking(char bufer[]) {
	std::string a, b, c, d, e, f, g, h, i_, j, k, l, m, n, o, p, q, r, s, t;
	int i = 0;
		for (i = 0; i <100 - 1; i++) {
			if (bufer[i] != '/') {
				a = a + bufer[i];
			}
			else if (bufer[i] == '/') {
				break;
			}
		}
		for (i = i + 1; i < 100 - 1; i++) {
			if (bufer[i] != '/') {
				b = b + bufer[i];
			}
			else if (bufer[i] == '/') {
				break;
			}
		}
		for (i = i + 1; i < 100 - 1; i++) {
			if (bufer[i] != '/') {
				c = c + bufer[i];
			}
			else if (bufer[i] == '/') {
				break;
			}
		}
		for (i = i + 1; i < 100 - 1; i++) {
			if (bufer[i] != '/') {
				d = d + bufer[i];
			}
			else if (bufer[i] == '/') {
				break;
			}
		}
		for (i = i + 1; i < 100 - 1; i++) {
			if (bufer[i] != '/') {
				e = e + bufer[i];
			}
			else if (bufer[i] == '/') {
				break;
			}
		}
		for (i = i + 1; i < 100 - 1; i++) {
			if (bufer[i] != '/') {
				f = f + bufer[i];
			}
			else if (bufer[i] == '/') {
				break;
			}
		}
		for (i = i + 1; i < 100 - 1; i++) {
			if (bufer[i] != '/') {
				g = g + bufer[i];
			}
			else if (bufer[i] == '/') {
				break;
			}
		}
		for (i = i + 1; i < 100 - 1; i++) {
			if (bufer[i] != '/') {
				h = h + bufer[i];
			}
			else if (bufer[i] == '/') {
				break;
			}
		}
		for (i = i + 1; i < 100 - 1; i++) {
			if (bufer[i] != '/') {
				i_ = i_ + bufer[i];
			}
			else if (bufer[i] == '/') {
				break;
			}
		}
		for (i = i + 1; i < 100 - 1; i++) {
			if (bufer[i] != '/') {
				j = j + bufer[i];
			}
			else if (bufer[i] == '/') {
				break;
			}
		}
		for (i = i + 1; i < 100 - 1; i++) {
			if (bufer[i] != '/') {
				k = k + bufer[i];
			}
			else if (bufer[i] == '/') {
				break;
			}
		}
		for (i = i + 1; i < 100 - 1; i++) {
			if (bufer[i] != '/') {
				l = l + bufer[i];
			}
			else if (bufer[i] == '/') {
				break;
			}
		}
		for (i = i + 1; i < 100 - 1; i++) {
			if (bufer[i] != '/') {
				m = m + bufer[i];
			}
			else if (bufer[i] == '/') {
				break;
			}
		}
		for (i = i + 1; i < 100 - 1; i++) {
			if (bufer[i] != '/') {
				n = n + bufer[i];
			}
			else if (bufer[i] == '/') {
				break;
			}
		}
		for (i = i + 1; i < 100 - 1; i++) {
			if (bufer[i] != '/') {
				o = o + bufer[i];
			}
			else if (bufer[i] == '/') {
				break;
			}
		}
		for (i = i + 1; i < 100 - 1; i++) {
			if (bufer[i] != '/') {
				p = p + bufer[i];
			}
			else if (bufer[i] == '/') {
				break;
			}
		}
		for (i = i + 1; i < 100 - 1; i++) {
			if (bufer[i] != '/') {
				q = q + bufer[i];
			}
			else if (bufer[i] == '/') {
				break;
			}
		}
		for (i = i + 1; i < 100 - 1; i++) {
			if (bufer[i] != '/') {
				r = r + bufer[i];
			}
			else if (bufer[i] == '/') {
				break;
			}
		}
		for (i = i + 1; i < 100 - 1; i++) {
			if (bufer[i] != '/') {
				s = s + bufer[i];
			}
			else if (bufer[i] == '/') {
				break;
			}
		}
		for (i = i + 1; i < 100 - 1; i++) {
			if (bufer[i] != '\0') {
				t = t + bufer[i];
			}
			else if (bufer[i] == '\0') {
				break;
			}
		}
		cout << "\n<RANKING>" << endl;
		cout << "1 -- " << a << " -- " << b << endl;
		cout << "2 -- " << c << " -- " << d << endl;
		cout << "3 -- " << e << " -- " << f << endl;
		cout << "4 -- " << g << " -- " << h << endl;
		cout << "5 -- " << i << " -- " << j << endl;
		cout << "6 -- " << k << " -- " << l << endl;
		cout << "7 -- " << m << " -- " << n << endl;
		cout << "8 -- " << o << " -- " << p << endl;
		cout << "9 -- " << q << " -- " << e << endl;
		cout << "10 -- " << s << " -- " << t << "\n" << endl;
}
void CutTheRopePlay(char bufer[], int &score, int &record) {
	std::string type;
	std::string tempScore;
	int i = 0;
	for (i = 0; i < 20 - 1; i++) {
		if (bufer[i] != '/') {
			tempScore = tempScore + bufer[i];
		}
		else if (bufer[i] == '/') {
			break;
		}
	}	
	for (i = i + 1; i < 20 - 1; i++) {
		if (bufer[i] != '\0') {
			type = bufer[i];
		}
		else if (bufer[i] == '\0') {
			break;
		}
	}
	record = std::stoi(type, nullptr, 10);
	score = std::stoi(tempScore, nullptr, 10);	
}

void CutTheRopeAchi(char bufer[], int &fail,int &cincuenta,int &cien,int &medio, int &entero) {
	std::string achi1;
	std::string achi2;
	std::string achi3;
	std::string achi4;
	std::string achi5;
	int i = 0;
	for (i = i; i < 20 - 1; i++) {
		if (bufer[i] != '/') {
			achi1 = bufer[i];
		}
		else if (bufer[i] == '/') {
			break;
		}
	}
	for (i = i + 1; i < 20 - 1; i++) {
		if (bufer[i] != '/') {
			achi2 = bufer[i];
		}
		else if (bufer[i] == '/') {
			break;
		}
	}
	for (i = i + 1; i < 20 - 1; i++) {
		if (bufer[i] != '/') {
			achi3 = bufer[i];
		}
		else if (bufer[i] == '/') {
			break;
		}
	}
	for (i = i + 1; i < 20 - 1; i++) {
		if (bufer[i] != '/') {
			achi4 = bufer[i];
		}
		else if (bufer[i] == '/') {
			break;
		}
	}
	for (i = i + 1; i < 20 - 1; i++) {
		if (bufer[i] != '\0') {
			achi5 = bufer[i];
		}
		else if (bufer[i] == '\0') {
			break;
		}
	}
	fail = std::stoi(achi1, nullptr, 10);
	cincuenta = std::stoi(achi2, nullptr, 10);
	cien = std::stoi(achi3, nullptr, 10);
	medio = std::stoi(achi4, nullptr, 10);
	entero = std::stoi(achi5, nullptr, 10);
}
void achiv(timer t) {
	int temp;
	temp = t.getTime();
	if (punts == 50) cincuenta = 1;
	if (punts == 100) cien = 1;
	if (temp == 60) medio = 1;
	if (temp == 120) entero = 1;
	else if (punts == 0 && vides == 0) fail = 1;
}
void cliente(timer t, int *a) {
	string total;
	if (*a == 1) {
		achiv(t);
		system("cls");
		cout << "Escribe tu nombre para registrar tu ranking: " << endl;
		string name;
		cin >> name;
		char bufer[10];
		char bufer1[10];
		char bufer2[10];
		char bufer3[10];
		char bufer4[10];
		char bufer5[10];
		char bufer6[10];
		_itoa_s(punts, bufer, 10);
		_itoa_s(*a, bufer6, 10);
		_itoa_s(fail, bufer1, 10);
		_itoa_s(cincuenta, bufer2, 10);
		_itoa_s(cien, bufer3, 10);
		_itoa_s(medio, bufer4, 10);
		_itoa_s(entero, bufer5, 10);
		total = name + "/" + bufer6 + "/" + bufer + "/" + bufer1 + "/" + bufer2 + "/" + bufer3 + "/" + bufer4 + "/" + bufer5;
	}
	else {
		cout << "Escribe tu nombre para mostrar las puntuaciones: " << endl;
		string name;
		cin >> name;
		char bufer[10];
		_itoa_s(*a, bufer, 10);
		total = name + "/" + bufer + "/" + "0" + "/" + "0" + "/" + "0" + "/" + "0" + "/" + "0" + "/" + "0"	;
	}
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	struct addrinfo *addr;
	struct addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	getaddrinfo("192.168.123.51", "4539", &hints, &addr);

	SOCKET  sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	connect(sock, addr->ai_addr, addr->ai_addrlen);

	send(sock, total.data(),total.length()+1, 0);

	if (*a == 1) {
		char mes[512];
		int i = recv(sock, mes, 512, 0);
		mes[i - 1] = '\0';
		CutTheRopePlay(mes, punts, record);
		cout << "Puntos" << punts << endl;
		if (record == 1) {
			cout << "Tienes un nuevo record champion" << endl;
		}
		else {
			cout << "No record no Party" << endl;
		}
	}
	else if (*a == 2 || *a == 3) {
		char mes[512];
		int i = recv(sock, mes, 512, 0);
		mes[i - 1] = '\0';
		CutTheRopeRanking(mes);
	}
	else if (*a == 4) {
		char mes[512];
		int i = recv(sock, mes, 512, 0);
		mes[i - 1] = '\0';
		CutTheRopeAchi(mes, fail, cincuenta, cien, medio, entero);
		if (fail == 0) {
			cout << "Has conseguido no morir con 0 puntos, eres mejor que un bebe" << endl;
		}
		else if (fail == 1) {
			cout << "Eres muy malo tio" << endl;
		}
		if (cincuenta == 1) {
			cout << "Has conseguido 50 puntos en una partida" << endl;
		}
		else if (cincuenta == 0) {
			cout << "Como no has conseguido 50 puntos?" << endl;
		}
		if (cien == 1) {
			cout << "Has conseguido 100 puntos" << endl;
		}
		else if (cien == 0) {
			cout << "Tienes que mejorar, no has conseguido 100 puntos" << endl;
		}
		if (medio == 1) {
			cout << "O dios mio no te han comigo en 60 segundos" << endl;
		}
		else if (medio == 0) {
			cout << "No has durado ni 60 segundos" << endl;
		}
		if (entero == 1) {
			cout << "Guau, has durado 120 segundos son morir" << endl;
		}
		else if (entero == 0) {
			cout << "Aun te falta, no has durado 120 segundos" << endl;
		}

	}
	shutdown(sock, 1);
	closesocket(sock);

	WSACleanup();
}

void Menu(int *a) {
	cout << "Menu Principal" << endl;
	cout << endl << endl;
	cout << "Selecciona que quieres hacer: " << endl;
	cout << "1 -- Jugar" << endl;
	cout << "2 -- Consultar ranking general" << endl;
	cout << "3 -- Consultar mis mejores puntuaciones" << endl;
	cout << "4 -- Consultar mis achievements" << endl;
	cout << "5 -- Salir" << endl;
	cin >> *a;
	system("cls");
}
void GameLoop(fantasma *f1, fantasma *f2, fantasma *f3, fantasma *f4) {
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
void main() {//CLIENTE        ---------->   PORT -> 5219  IP-> 192.168.123.59
	int a = 0;
	timer t;
	t.start();
	for (;;) {
		Menu(&a);	
		if (a == 1) {
			fantasma ghostA = inicialitzarFantasma(41, 14, 2);
			fantasma ghostB = inicialitzarFantasma(43, 14, 3);
			fantasma ghostC = inicialitzarFantasma(40, 14, 4);
			fantasma ghostD = inicialitzarFantasma(39, 14, 5);
			pintar_mapa();
			fantasma* f1 = &ghostA;
			fantasma* f2 = &ghostB;
			fantasma* f3 = &ghostC;
			fantasma* f4 = &ghostD;
			while (vides > 0 && punts < 1950) {
				achiv(t);
				GameLoop(f1, f2, f3, f4);
			}
			for (int i = 0; i <= vides; i++) {
				gotoxy(5, i + 27);
				printf(" ");
			}
			if (vides == 0) {
				cliente(t, &a);
				vides = 1;
				system("pause");
				system("cls");
				a = 1;
				//Menu(&a);
			}
		}
		else if (a == 2) {
			cliente(t, &a);
			Menu(&a);
		}
		else if (a == 3) {
			cliente(t, &a);
			Menu(&a);
		}
		else if (a == 4) {
			cliente(t, &a);
			Menu(&a);
		}
		else if (a == 5) {
			exit(0);
		}
	}
	system("pause");
}



