#include <iostream>
#include <list>
#include <algorithm>
using namespace std;

const int NUM_CASILLAS = 63;
const list<int> CASILLAS_OCA {5, 9, 14, 18, 23, 27, 32, 36, 41, 45, 50, 54, 59, 63};
const list<int> CASILLAS_PUENTE = {6, 12};
const list<int> CASILLAS_DADOS = {26, 53};
const int CASILLA_POSADA = 19;
const int CASILLA_PRISION = 52;
const int CASILLA_POZO = 31;
const int CASILLA_LABERINTO = 42;
const int CASILLA_MUERTE = 58;
const int TURNOS_POSADA = 1;

bool esOca(int casilla) {
	bool condicion;
	if (find(CASILLAS_OCA.begin(), CASILLAS_OCA.end(), casilla) != CASILLAS_OCA.end()) {
		condicion = true;
	}
	else condicion = false;
	return condicion;
}
bool esPuente(int casilla) {
	bool condicion;
	if (find(CASILLAS_PUENTE.begin(), CASILLAS_PUENTE.end(), casilla) != CASILLAS_PUENTE.end()) {
		condicion = true;
	}
	else condicion = false;
	return condicion;
}
bool esDados(int casilla) {
	bool condicion;
	if (find(CASILLAS_DADOS.begin(), CASILLAS_DADOS.end(), casilla) != CASILLAS_DADOS.end()) {
		condicion = true;
	}
	else condicion = false;
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
	
}