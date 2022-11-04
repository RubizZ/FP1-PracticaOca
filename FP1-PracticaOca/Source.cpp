#include <iostream>
//#include <array>
#include <cstdlib>
#include <ctime>
//#define sizeOf(type) ((char *)(&type+1)-(char*)(&type))

using namespace std;

const bool MODO_DEBUG = true;

const int NUM_CASILLAS = 63;
const int CASILLAS_OCA[] = {5, 9, 14, 18, 23, 27, 32, 36, 41, 45, 50, 54, 59, 63};
const int CASILLAS_PUENTE[] = {6, 12};
const int CASILLAS_DADOS[] = {26, 53};
const int CASILLA_POSADA = 19;
const int CASILLA_PRISION = 52;
const int CASILLA_POZO = 31;
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
int cambioDeTurno(int casillaActual, int& casillaJ1, int& casillaJ2, int& turno, int& numeroDeTiradas);


int main() {
	srand(time(NULL));
	int casillaJ1 = 1, casillaJ2 = 1;
	int numeroDeTiradas = 1;
	int casillaActual = 1;
	int dado;
	int turno = quienEmpieza();
	
	cout << "JUEGO DE LA OCA" << endl;

	while (!esMeta(casillaActual)) {
		cout << "--------------------------" << endl << "Es el turno del jugador " << turno << endl;
		while (numeroDeTiradas >= 1 && !esMeta(casillaActual)) {
			numeroDeTiradas--;

			//Modo debug del dado
			if (MODO_DEBUG) {
				dado = tirarDadoManual();
			}
			else dado = tirarDado();
			/*
			//Comprobar que el dado se ha pasado la meta para volver para atras
			if (casillaActual + dado < NUM_CASILLAS) {
				casillaActual += dado;
			}
			else casillaActual = NUM_CASILLAS - (dado - (NUM_CASILLAS - casillaActual));
			*/
			casillaActual += dado;
			casillaActual = efectoPosicion(casillaActual);
			numeroDeTiradas = efectoTiradas(casillaActual, numeroDeTiradas);
		}
		
		if (!esMeta(casillaActual)) {
			
			//Actualiza las casilla de los jugadores, cambia el turno y cambia de tiradas de un jugador a otro segun las tiradas de penalizacion que tenga
			casillaActual = cambioDeTurno(casillaActual, casillaJ1, casillaJ2, turno, numeroDeTiradas);

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
	int i = 0;
	bool condicion = false;
	while (i < 2) {
		if (CASILLAS_PUENTE[i] == casilla) {
			condicion = true;
		}
		i++;
	}
	return condicion;
}
bool esDados(int casilla) {
	int i = 0;
	bool condicion = false;
	while (i < 2) {
		if (CASILLAS_DADOS[i] == casilla) {
			condicion = true;
		}
		i++;
	}
	return condicion;
}
bool esLaberinto(int casilla) {
	bool condicion;
	if (casilla == CASILLA_LABERINTO) {
		condicion = true;
	}
	else condicion = false;
	return condicion;
}
bool esMuerte(int casilla) {
	bool condicion;
	if (casilla == CASILLA_MUERTE) {
		condicion = true;
	}
	else condicion = false;
	return condicion;
}
bool esPosada(int casilla) {
	bool condicion;
	if (casilla == CASILLA_POSADA) {
		condicion = true;
	}
	else condicion = false;
	return condicion;
}
bool esPrision(int casilla) {
	bool condicion;
	if (casilla == CASILLA_PRISION) {
		condicion = true;
	}
	else condicion = false;
	return condicion;
}
bool esPozo(int casilla) {
	bool condicion;
	if (casilla == CASILLA_POZO) {
		condicion = true;
	}
	else condicion = false;
	return condicion;
}
bool esMeta(int casilla) {
	bool condicion;
	if (casilla >= 63) {
		condicion = true;
	}
	else condicion = false;
	return condicion;
}

int siguienteOca(int casilla) {
	int i = 0;
	int siguiente = 0;
	while (i < 14) {
		if (CASILLAS_OCA[i] == casilla) {
			siguiente = CASILLAS_OCA[i + 1];
		}
		i++;
	}
	return siguiente;
}
int siguientePuente(int casilla) {
	int i = 0;
	int siguiente = 0;
	while (i < 2) {
		if (CASILLAS_PUENTE[i] == casilla) {
			if (i != 1) {
				siguiente = CASILLAS_PUENTE[(i + 1)];
			}
			else {
				siguiente = CASILLAS_PUENTE[(i - 1)];
			}
		}
		i++;
	}
	return siguiente;
}
int siguienteDados(int casilla) {
	int i = 0;
	int siguiente = 0;
	while (i < 2) {
		if (CASILLAS_DADOS[i] == casilla) {
			if (i != 1) {
				siguiente = CASILLAS_DADOS[(i + 1)];
			}
			else {
				siguiente = CASILLAS_DADOS[(i - 1)];
			}
		}
		i++;
	}
	return siguiente;
}
int siguienteLaberinto(int casilla) {
	int siguiente = casilla - 12;
	return siguiente;
}
int siguienteMuerte(int casilla) {
	int siguiente = 1;
	return siguiente;
}

int tirarDado() {
	int i;
	cout << endl << "Pulsa enter para tirar el dado" << endl;
	i = getc(stdin);
	int dado = (rand() % 6) + 1;
	cout << "Has sacado un " << dado << endl;
	return dado;
}
int tirarDadoManual() {
	int dado;
	cout << "Introduce el valor del dado: ";
	cin >> dado;
	return dado;
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
		siguiente = siguienteDados(casillaActual);
		cout << "Has llegado a la casilla: " << siguiente << endl;
	}
	else siguiente = casillaActual;

	return siguiente;
}
int efectoTiradas(int casillaActual, int numeroDeTiradas) {
	int numeroActualizado;
	if (esOca(casillaActual) == true || esPuente(casillaActual) == true || esDados(casillaActual) == true) {
		cout << "Vuelves a tirar" << endl;
		numeroActualizado = numeroDeTiradas + TURNOS_OCA_PUENTE_DADOS;
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
int cambioDeTurno(int casillaActual, int& casillaJ1, int& casillaJ2, int& turno, int& numeroDeTiradas) {
	if (turno == 1) {
		casillaJ1 = casillaActual;
		casillaActual = casillaJ2;
		turno = 2;
	}
	else if (turno == 2) {
		casillaJ2 = casillaActual;
		casillaActual = casillaJ1;
		turno = 1;
	}
	numeroDeTiradas = 1 - numeroDeTiradas;
	return casillaActual;
}