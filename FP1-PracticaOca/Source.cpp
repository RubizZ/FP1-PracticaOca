#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

const bool MODO_DEBUG = false;

const int NUM_CASILLAS = 63;
const int TURNOS_OCA = 1;
const int CASILLA_PUENTE_1 = 6;
const int CASILLA_PUENTE_2 = 12;
const int TURNOS_PUENTE = 1;
const int CASILLA_DADOS_1 = 26;
const int CASILLA_DADOS_2 = 53;
const int TURNOS_DADOS = 1;
const int CASILLA_POSADA = 19;
const int TURNOS_POSADA = -1;
const int CASILLA_PRISION = 52;
const int TURNOS_PRISION = -2;
const int CASILLA_POZO = 31;
const int TURNOS_POZO = -3;
const int CASILLA_LABERINTO = 42;
const int CASILLA_MUERTE = 58;

bool esOca(int casilla);
bool esPuente(int casilla);
bool esDados(int casilla);
bool esLaberinto(int casilla);
bool esMuerte(int casilla);
bool esPosada(int casilla);
bool esPrision(int casilla);
bool esPozo(int casilla);
bool esMeta(int casilla);
int siguienteOca(int casilla);
int siguientePuente(int casilla);
int siguienteDado(int casilla);
int siguienteLaberinto(int casilla);
int siguienteMuerte(int casilla);
int tirarDado();
int tirarDadoManual();
int quienEmpieza();
int efectoPosicion(int casillaActual);
int efectoTiradas(int casillaActual, int numeroDeTiradas);


int main() {
	srand(time(NULL));
	int casillaJ1 = 1, casillaJ2 = 1;
	int numeroDeTiradas = 1;
	int casillaActual = 1;
	int dado;
	int turno = quienEmpieza();
	
	cout << "JUEGO DE LA OCA" << endl;

	while (!esMeta(casillaActual)) { //Bucle principal en el que se desarrolla la partida hasta que alguien gane
		cout << "--------------------------" << endl << "Es el turno del jugador " << turno << endl;
		while (numeroDeTiradas >= 1 && !esMeta(casillaActual)) { //Bucle del turno de un jugador
			numeroDeTiradas--;

			//Modo debug del dado
			if (MODO_DEBUG) {
				dado = tirarDadoManual();
			}
			else dado = tirarDado();
			
			casillaActual += dado;
			casillaActual = efectoPosicion(casillaActual);
			numeroDeTiradas = efectoTiradas(casillaActual, numeroDeTiradas);
		}
		
		if (!esMeta(casillaActual)) { //Al terminar el turno, si no es meta se hacen todos los cambios necesarios para cambiar de turno
			
			//Cambia de tiradas de un jugador a otro segun las tiradas de penalizacion que tenga
			numeroDeTiradas = 1 - numeroDeTiradas;

			//Actualiza la casilla actual, guardandola en el jugador correspondiente y cambiando de turno
			if (turno == 1) {
				turno = 2;
				casillaJ1 = casillaActual;
				casillaActual = casillaJ2;
			}
			else if (turno == 2) {
				turno = 1;
				casillaJ2 = casillaActual;
				casillaActual = casillaJ1;
			}
		}
	}
	cout << "************************************************************";
	cout << endl << "**************** El ganador es el jugador " << turno << " ****************" << endl;
	cout << "************************************************************";
	return 0;
}

bool esOca(int casilla) {
	bool condicion;
	if ((casilla % 9 == 0 || casilla % 9 == 5)) {
		condicion = true;
	}
	else {
		condicion = false;
	}
	return condicion;
}
bool esPuente(int casilla) {
	bool condicion;
	if (casilla  == CASILLA_PUENTE_1 || casilla == CASILLA_PUENTE_2) {
		condicion = true;
	}
	else {
		condicion = false;
	}
	return condicion;
}
bool esDados(int casilla) {
	bool condicion;
	if (casilla == CASILLA_DADOS_1 || casilla == CASILLA_DADOS_2) {
		condicion = true;
	}
	else {
		condicion = false;
	}
	return condicion;
}
bool esLaberinto(int casilla) {
	bool condicion;
	if (casilla == CASILLA_LABERINTO) {
		condicion = true;
	}
	else {
		condicion = false;
	}
	return condicion;
}
bool esMuerte(int casilla) {
	bool condicion;
	if (casilla == CASILLA_MUERTE) {
		condicion = true;
	}
	else {
		condicion = false;
	}
	return condicion;
}
bool esPosada(int casilla) {
	bool condicion;
	if (casilla == CASILLA_POSADA) {
		condicion = true;
	}
	else {
		condicion = false;
	}
	return condicion;
}
bool esPrision(int casilla) {
	bool condicion;
	if (casilla == CASILLA_PRISION) {
		condicion = true;
	}
	else {
		condicion = false;
	}
	return condicion;
}
bool esPozo(int casilla) {
	bool condicion;
	if (casilla == CASILLA_POZO) {
		condicion = true;
	}
	else {
		condicion = false;
	}
	return condicion;
}
bool esMeta(int casilla) {
	bool condicion;
	if (casilla >= 63) {
		condicion = true;
	}
	else {
		condicion = false;
	}
	return condicion;
}

int siguienteOca(int casilla) {
	int nuevaCasilla;
	if (casilla % 9 == 0) {
		nuevaCasilla = casilla + 5;
	}
	else {
		nuevaCasilla = casilla + 4;
	}
	return nuevaCasilla;

}
int siguientePuente(int casilla) {
	int nuevaCasilla;
	if (casilla == CASILLA_PUENTE_1) {
		nuevaCasilla = CASILLA_PUENTE_2;
	}
	else {
		nuevaCasilla = CASILLA_PUENTE_1;
	}
	return nuevaCasilla;
}
int siguienteDado(int casilla) {
	int nuevaCasilla;
	if (casilla == CASILLA_DADOS_1) {
		nuevaCasilla = CASILLA_DADOS_2;
	}
	else {
		nuevaCasilla = CASILLA_DADOS_1;
	}
	return nuevaCasilla;
}
int siguienteLaberinto(int casilla) {
	int nuevaCasilla= casilla - 12;
	return nuevaCasilla;
}
int siguienteMuerte(int casilla) {
	int nuevaCasilla = 1;
	return nuevaCasilla;
}

int tirarDado() {
	string i;

	//Pregunta para que el usuario "tire" un dado, no siguiendo con el codigo hasta que pulse enter
	cout << endl << "Pulsa enter para tirar el dado" << endl;
	
	//Guarda todo lo que el usuario escribe en una string basura i hasta que le da al enter para pasar a la siguiente instruccion
	getline(cin, i);

	int dado = (rand() % 6) + 1;
	cout << "Has sacado un " << dado << endl;
	return dado;
}
int tirarDadoManual() {
	double dado = 0;
	cout << "Introduce el valor del dado: ";
	cin >> dado;
	return int(dado); //Si el usuario escribe un numero decimal, se castea en uno entero para evitar errores
}
int quienEmpieza() {
	int empieza = (rand() % 2) + 1;
	return empieza;
}
int efectoPosicion(int casillaActual) {
	int siguiente;
	cout << "Te has movido a la casilla: " << casillaActual << endl;
	if (esOca(casillaActual) == true && !esMeta(casillaActual)) {
		cout << "De oca a oca y tiro por que me toca" << endl;
		siguiente = siguienteOca(casillaActual);
		cout << "Has llegado a la casilla: " << siguiente << endl;
	}
	else if (esPuente(casillaActual) == true) {
		cout << "De puente a puente y tiro porque me lleva la corriente" << endl;
		siguiente = siguientePuente(casillaActual);
		cout << "Has llegado a la casilla: " << siguiente << endl;
	}
	else if (esLaberinto(casillaActual) == true) {
		cout << "Has caido en el laberinto" << endl;
		siguiente = siguienteLaberinto(casillaActual);
		cout << "Has llegado a la casilla: " << siguiente << endl;
	}
	else if (esMuerte(casillaActual) == true) {
		cout << "Has caido en la muerte" << endl;
		siguiente = siguienteMuerte(casillaActual);
		cout << "Has llegado a la casilla: " << siguiente << endl;
	}
	else if (esDados(casillaActual) == true) {
		cout << "De dado a dado y tiro porque me ha tocado" << endl;
		siguiente = siguienteDado(casillaActual);
		cout << "Has llegado a la casilla: " << siguiente << endl;
	}
	else siguiente = casillaActual;
	cout << endl;
	return siguiente;
}
int efectoTiradas(int casillaActual, int numeroDeTiradas) {
	int numeroActualizado;
	if (esOca(casillaActual) == true && !esMeta(casillaActual)) {
		cout << "Vuelves a tirar" << endl;
		numeroActualizado = numeroDeTiradas + TURNOS_OCA;
	}
	else if (esPuente(casillaActual) == true) {
		cout << "Vuelves a tirar" << endl;
		numeroActualizado = numeroDeTiradas + TURNOS_PUENTE;
	}
	else if (esDados(casillaActual) == true) {
		cout << "Vuelves a tirar" << endl;
		numeroActualizado = numeroDeTiradas + TURNOS_DADOS;
	}
	else if (esPosada(casillaActual) == true) {
		cout << "Has caido en la posada" << endl << "No tiras en 1 turno" << endl;
		numeroActualizado = numeroDeTiradas + TURNOS_POSADA;
	}
	else if (esPrision(casillaActual) == true) {
		cout << "Has caido en la prision" << endl << "No tiras en 2 turnos" << endl;
		numeroActualizado = numeroDeTiradas + TURNOS_PRISION;
	}
	else if (esPozo(casillaActual) == true) {
		cout << "Has caido en el pozo" << endl << "No tiras en 3 turnos" << endl;
		numeroActualizado = numeroDeTiradas + TURNOS_POZO;
	}
	else numeroActualizado = numeroDeTiradas;
	return numeroActualizado;
}
