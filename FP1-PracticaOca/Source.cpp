#include <iostream>
#include <array>
#include <cstdlib>
#include <ctime>
#define sizeOf(type) ((char *)(&type+1)-(char*)(&type))

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
const int TURNOS_OCA_PUENTE_DADOS = 1;
const int TURNOS_POSADA = -1;
const int TURNOS_PRISION = -2;
const int TURNOS_POZO = -3;

bool esOca(int casilla) {
	int i = 0;
	bool condicion = false;
	while (i < sizeOf(CASILLAS_OCA)) {
		if (CASILLAS_OCA[i] != casilla) {
			condicion = true;
		}
		i++;
	}
	return condicion;
}
bool esPuente(int casilla) {
	int i = 0;
	bool condicion = false;
	while (i < sizeOf(CASILLAS_PUENTE)) {
		if (CASILLAS_PUENTE[i] != casilla) {
			condicion = true;
		}
		i++;
	}
	return condicion;
}
bool esDados(int casilla) {
	int i = 0;
	bool condicion = false;
	while (i < sizeOf(CASILLAS_DADOS)) {
		if (CASILLAS_DADOS[i] != casilla) {
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
	int siguiente = 0;
	array<int, 14> CASILLAS_OCA;
	while (int i = 0 < CASILLAS_OCA.size()) {
		if (CASILLAS_OCA[i] == casilla) {
			siguiente = CASILLAS_OCA[i + 1];
		}
		i++;
	}
	return siguiente;
}
int siguientePuente(int casilla) {
	int siguiente = 0;
	array<int, 2> CASILLAS_PUENTE;
	while (int i = 0 < CASILLAS_PUENTE.size() - 1) {
		if (CASILLAS_PUENTE[i] == casilla) {
			siguiente = CASILLAS_PUENTE[i + 1];
		}
		i++;
	}
	if (siguiente == casilla) {
		while (int i = CASILLAS_PUENTE.size() > 1) {
			if (CASILLAS_PUENTE[i] == casilla) {
				siguiente = CASILLAS_PUENTE[i + 1];
			}
			i--;
		}
	}
	return siguiente;
}
int siguienteDados(int casilla) {
	int siguiente = 0;
	array<int, 2> CASILLAS_DADO;
	while (int i = 0 < CASILLAS_DADO.size()) {
		if (CASILLAS_DADO[i] == casilla) {
			siguiente = CASILLAS_DADO[i + 1];
		}
		i++;
	}
	if (siguiente == casilla) {
		while (int i = CASILLAS_DADO.size() > 1) {
			if (CASILLAS_DADO[i] == casilla) {
				siguiente = CASILLAS_DADO[i + 1];
			}
			i--;
		}
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
	int dado = (rand() % 6) + 1;
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
	if (esOca(casillaActual) == true && esOca(casillaActual) != esMeta(casillaActual)) {
		cout << "De oca a oca y tiro por que me toca" << endl;
		siguiente = siguienteOca(casillaActual);
	} 
	else if (esPuente(casillaActual) == true) {
		cout << "De puente a puente y tiro porque me lleva la corriente" << endl;
		siguiente = siguientePuente(casillaActual);
	} 
	else if (esLaberinto(casillaActual) == true) {
		siguiente = siguienteLaberinto(casillaActual);
	} 
	else if (esMuerte(casillaActual) == true) {
		siguiente = siguienteMuerte(casillaActual);
	}
	else if (esDados(casillaActual) == true) {
		cout << "De dado a dado y tiro porque me ha tocado" << endl;
		siguiente = siguienteDados(casillaActual);
	}
	else siguiente = casillaActual;
	cout << "Has llegado a la casilla: " << siguiente << endl;

	return siguiente;
}
int efectoTiradas(int casillaActual, int numeroDeTiradas) {
	int numeroActualizado;
	if ((esOca(casillaActual) == true && esOca(casillaActual) != esMeta(casillaActual)) || esPuente(casillaActual) == true || esDados(casillaActual) == true) {
		cout << "Vuelves a tirar" << endl;
		numeroActualizado = numeroDeTiradas + TURNOS_OCA_PUENTE_DADOS;
	}
	else if (esPosada(casillaActual) == true) {
		cout << "No tiras en 1 turno" << endl;
		numeroActualizado = numeroDeTiradas + TURNOS_POSADA;
	}
	else if (esPrision(casillaActual) == true) {
		cout << "No tiras en 2 turnos" << endl;
		numeroActualizado = numeroDeTiradas + TURNOS_PRISION;
	}
	else if (esPozo(casillaActual) == true) {
		cout << "No tiras en 3 turnos" << endl;
		numeroActualizado = numeroDeTiradas + TURNOS_POZO;
	}
	else numeroActualizado = numeroDeTiradas;
	return numeroActualizado;
}

int main() {
	
	srand(time(NULL));
	
	bool inicio = true;
	int dado;
	int turnoDe = quienEmpieza();
	int casillaJ1 = 1, casillaJ2 = 1;
	int tiradasJ1 = 1, tiradasJ2 = 1;
	bool final = false;

	while (final == false) {
		if (inicio == false) {
			cout << endl << "Es el turno del jugador " << turnoDe << endl;
		}
		else {
			cout << "Empieza el jugador " << turnoDe << endl;
			inicio = false;
		}
		while (turnoDe == 1) {

			if (MODO_DEBUG == false) {
				dado = tirarDado();
			}
			else {
				dado = tirarDadoManual();
			}

			tiradasJ1--;

			cout << "Has sacado un " << dado << endl;

			casillaJ1 += dado;
			casillaJ1 = efectoPosicion(casillaJ1);

			tiradasJ1 = efectoTiradas(casillaJ1, tiradasJ1);

			if (tiradasJ2 <= 0) {
				tiradasJ2++;
			}
			if (tiradasJ1 <= 0) {
				turnoDe = 2;
			}
			if (esMeta(casillaJ1) == true) {
				cout << "Ha ganado el jugador 1" << endl;
				final = true;
				turnoDe = 0;
			}
		}
		while (turnoDe == 2) {

			if (MODO_DEBUG == false) {
				dado = tirarDado();
			}
			else {
				dado = tirarDadoManual();
			}

			tiradasJ2--;

			cout << "Has sacado un " << dado << endl;

			casillaJ2 += dado;
			casillaJ2 = efectoPosicion(casillaJ2);

			tiradasJ2 = efectoTiradas(casillaJ2, tiradasJ2);

			if (tiradasJ1 <= 0) {
				tiradasJ1++;
			}
			if (tiradasJ2 <= 0) {
				turnoDe = 1;
			}
			if (esMeta(casillaJ2) == true) {
				cout << "Ha ganado el jugador 2" << endl;
				final = true;
				turnoDe = 0;
			}
		}
	}
}

/*

- Arreglar esOca esPuente esDados
- Arreglar siguienteOca siguientePuente siguienteDados
- Probar los turnos

*/