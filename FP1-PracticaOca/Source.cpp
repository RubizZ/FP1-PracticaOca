#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <time.h>

using namespace std;

const int MODO_DEBUG = false;

//-------------CONSTANTES-------------//
const int CASILLA_INICIAL = 1;
const int NUM_CASILLAS = 63;

const int TURNOS_POSADA = 1;
const int TURNOS_CARCEL = 2;
const int TURNOS_POZO = 3;
const int RETROCESO_LABERINTO = 12;

const int MAX_PARTIDAS = 10;

const int NUM_JUGADORES = 2;
const int MAX_JUGADORES = 4;

const int NUM_FILAS_A_DIBUJAR = 3;

const int CENTINELA = 0;
//------------------------------------//

//Tipos definidos de casillas
typedef enum { NORMAL, OCA, PUENTE1, PUENTE2, POZO, POSADA, LABERINTO, DADO1, DADO2, CARCEL, CALAVERA } tCasilla;
typedef tCasilla tTablero[NUM_CASILLAS];

//Tipos definidos del estado de los jugadores
typedef struct {
	int casilla;
	int penalizaciones;
} tEstadoJugador;
typedef tEstadoJugador tEstadoJugadores[NUM_JUGADORES];

//Tipos definidos de las partidas no acabadas
typedef struct {
	tEstadoJugadores estadoJug;
	int turno;
	tTablero tablero;
}tEstadoPartida;
typedef tEstadoPartida tTablaPartidas[MAX_PARTIDAS];
typedef struct {
	int cont;
	tTablaPartidas listaPartidas;
}tListaPartidas;

//Funciones que no cambian
void iniciaTablero(tTablero tablero);
bool cargaTablero(tTablero tablero);
tCasilla stringAtCasilla(string casilla);
int saltaACasilla(const tTablero tablero, int casillaActual);
void buscaCasillaAvanzando(const tTablero tablero, tCasilla tipo, int& casillaActual);
void buscaCasillaRetrocediendo(const tTablero tablero, tCasilla tipo, int& casillaActual);
bool esCasillaPremio(const tTablero tablero, int casillaActual);

//Funciones modificadas
void iniciaJugadores(tEstadoJugadores jugadores);
void efectoTirada(const tTablero tablero, tEstadoJugador& estadoJug);
void tirada(const tTablero tablero, tEstadoJugador& estadoJug);
int tirarDado();
int tirarDadoManual();
int partida(tEstadoPartida& estado);
void pintaTablero(const tEstadoPartida& partida);

int main() {
	
    return 0;
}

//Funciones principales
int partida(tEstadoPartida& estado) {//Hay que cambiar esto en plan esta funcion.
    tJugadores jugadores, penalizaciones;
    int casillaActual = 0;
    int turno;
    bool finPartida = false;

    iniciaJugadores(jugadores, penalizaciones);
    pintaTablero(tablero, jugadores);

    turno = quienEmpieza(); // Del 0 hasta NUM_JUGADORES - 1
    cout << "Empieza el jugador " << turno + 1 << endl;

    while (!finPartida) {
        penalizaciones[turno] = tirada(tablero, casillaActual);
        if (casillaActual >= NUM_CASILLAS - 1) {
            finPartida = true;
            jugadores[turno] = NUM_CASILLAS - 1;
        }
        else jugadores[turno] = casillaActual;
        pintaTablero(tablero, jugadores);

        while (esCasillaPremio(tablero, casillaActual) && !finPartida) {
            if (casillaActual < NUM_CASILLAS - 1) {
                cout << "Vuelves a tirar" << endl;
                penalizaciones[turno] = tirada(tablero, casillaActual);
                if (casillaActual >= NUM_CASILLAS - 1) {
                    finPartida = true;
                    jugadores[turno] = NUM_CASILLAS - 1;
                }
                else jugadores[turno] = casillaActual;
                pintaTablero(tablero, jugadores);
            }
        }
        if (!finPartida) {
            cambioDeTurno(turno);
            while (penalizaciones[turno] > 0) {
                if (penalizaciones[turno] != 1) cout << "El jugador " << turno + 1 << " esta penalizado, le quedan " << penalizaciones[turno] << " turnos sin jugar" << endl;
                else cout << "El jugador " << turno + 1 << " esta penalizado, le queda " << penalizaciones[turno] << " turno sin jugar" << endl;
                penalizaciones[turno]--;
                cambioDeTurno(turno);
            }
            cout << "Es el turno del jugador " << turno + 1 << endl << endl;
            casillaActual = jugadores[turno];
        }
    }
    return turno;
}
void tirada(const tTablero tablero, tEstadoJugador& estadoJug) {
    int dado;

    //Tira dados
    if (!MODO_DEBUG) {
        dado = tirarDado();
        cout << "Has sacado un " << dado << endl;
    }
    else dado = tirarDadoManual();

    estadoJug.casilla += dado;
    cout << "Estas en la casilla " << estadoJug.casilla + 1 << endl;

    if (estadoJug.casilla < NUM_CASILLAS - 1) efectoTirada(tablero, estadoJug);

}
void efectoTirada(const tTablero tablero, tEstadoJugador& estadoJug) {
    tCasilla tipo;
    if (estadoJug.casilla < NUM_CASILLAS - 1) {
        switch (tablero[estadoJug.casilla]) {
        case OCA:
            cout << "De oca a oca y tiro porque me toca" << endl;
            estadoJug.casilla = saltaACasilla(tablero, estadoJug.casilla);
            break;
        case DADO1:
            cout << "De dado a dado y tiro porque me ha tocado" << endl;
            estadoJug.casilla = saltaACasilla(tablero, estadoJug.casilla);
            break;
        case DADO2:
            cout << "De dado a dado y tiro porque me ha tocado" << endl;
            estadoJug.casilla = saltaACasilla(tablero, estadoJug.casilla);
            break;
        case PUENTE1:
            cout << "De puente a puente y tiro porque me lleva la corriente" << endl;
            estadoJug.casilla = saltaACasilla(tablero, estadoJug.casilla);
            break;
        case PUENTE2:
            cout << "De puente a puente y tiro porque me lleva la corriente" << endl;
            estadoJug.casilla = saltaACasilla(tablero, estadoJug.casilla);
            break;
        case CALAVERA:
            cout << "Has caido en la muerte" << endl;
            estadoJug.casilla = saltaACasilla(tablero, estadoJug.casilla);
            break;
        case LABERINTO:
            estadoJug.casilla = saltaACasilla(tablero, estadoJug.casilla);
            cout << "Has caido en el laberinto" << endl;
            break;
        case POSADA:
            cout << "Has caido en la posada" << endl;
            cout << "Te quedas " << TURNOS_POSADA << " turno sin jugar" << endl;
            estadoJug.penalizaciones = TURNOS_POSADA;
            break;
        case CARCEL:
            cout << "Has caido en la carcel" << endl;
            cout << "Te quedas " << TURNOS_CARCEL << " turnos sin jugar" << endl;
            estadoJug.penalizaciones = TURNOS_CARCEL;
            break;
        case POZO:
            cout << "Has caido en el pozo" << endl;
            cout << "Te quedas " << TURNOS_POZO << " turnos sin jugar" << endl;
            estadoJug.penalizaciones = TURNOS_POZO;
            break;
        }
    }
}

//Funciones iniciales para establecer el tablero y jugadores
void iniciaTablero(tTablero tablero) { //Inicializa todo el tablero a normal excepto NUM_CASILLAS-1 que es la OCA 63
	for (int i = 0; i < NUM_CASILLAS - 1; i++) {
		tablero[i] = NORMAL;
	}
	tablero[NUM_CASILLAS - 1] = OCA;
}
bool cargaTablero(tTablero tablero) {
    ifstream archivo;
    string nombre;
    int posicion;
    string tipo;
    tCasilla tipotCasilla;
    bool satisfactorio = false;

    cout << "Introduce el nombre del fichero del tablero: ";
    getline(cin, nombre);

    archivo.open(nombre);

    if (archivo.is_open()) {
        archivo >> posicion;
        while (posicion != CENTINELA) {
            char aux;
            archivo.get(aux);
            getline(archivo, tipo);
            tipotCasilla = stringAtCasilla(tipo);
            tablero[posicion - 1] = tipotCasilla;
            archivo >> posicion;
        }
        satisfactorio = true;
        archivo.close();
    }
    return satisfactorio;
}
tCasilla stringAtCasilla(string casilla) {
    tCasilla cadena = NORMAL;
    if (casilla == "OCA") cadena = OCA;
    else if (casilla == "DADO1") cadena = DADO1;
    else if (casilla == "DADO2") cadena = DADO2;
    else if (casilla == "PUENTE1") cadena = PUENTE1;
    else if (casilla == "PUENTE2") cadena = PUENTE2;
    else if (casilla == "POSADA") cadena = POSADA;
    else if (casilla == "CALAVERA") cadena = CALAVERA;
    else if (casilla == "LABERINTO") cadena = LABERINTO;
    else if (casilla == "POZO") cadena = POZO;
    else if (casilla == "CARCEL") cadena = CARCEL;
    return cadena;
}
void iniciaJugadores(tEstadoJugadores jugadores) {
    tEstadoJugador estado;
    estado.casilla = 0;
    estado.penalizaciones = 0;
    //Situa a todos los jugadores en la casilla 1 (con index 0) y les asigna 0 penalizaciones
    for (int i = 0; i < NUM_JUGADORES; i++) {
        jugadores[i] = estado;
    }
}

//Funciones del dado
int tirarDado() {
    string i;

    //Pregunta para que el usuario "tire" un dado, no siguiendo con el codigo hasta que pulse enter
    cout << "Pulsa enter para tirar el dado";

    //Guarda todo lo que el usuario escribe en una string basura i hasta que le da al enter para pasar a la siguiente instruccion
    getline(cin, i);

    return (rand() % 6) + 1;
}
int tirarDadoManual() {
    int dado;
    cout << "Introduce el valor del dado: ";
    cin >> dado;
    return dado;
}

//Funciones para analizar el efecto de la tirada
bool esCasillaPremio(const tTablero tablero, int casillaActual) {
    bool condicion = false;
    switch (tablero[casillaActual]) {
    case OCA:
    case PUENTE1:
    case PUENTE2:
    case DADO1:
    case DADO2:
        condicion = true;
        break;
    }
    return condicion;
}
int saltaACasilla(const tTablero tablero, int casillaActual) {
    switch (tablero[casillaActual]) {
    case OCA:
        buscaCasillaAvanzando(tablero, OCA, casillaActual);
        cout << "Avanzas hasta la casilla " << casillaActual + 1 << endl;
        break;
    case DADO1:
        buscaCasillaAvanzando(tablero, DADO2, casillaActual);
        cout << "Avanzas hasta la casilla " << casillaActual + 1 << endl;
        break;
    case PUENTE1:
        buscaCasillaAvanzando(tablero, PUENTE2, casillaActual);
        cout << "Avanzas hasta la casilla " << casillaActual + 1 << endl;
        break;
    case DADO2:
        buscaCasillaRetrocediendo(tablero, DADO1, casillaActual);
        cout << "Retrocedes hasta la casilla " << casillaActual + 1 << endl;
        break;
    case PUENTE2:
        buscaCasillaRetrocediendo(tablero, PUENTE1, casillaActual);
        cout << "Retrocedes hasta la casilla " << casillaActual + 1 << endl;
        break;
    case CALAVERA:
        casillaActual = 0;
        cout << "Vuelves a la casilla " << casillaActual + 1 << endl;
        break;
    case LABERINTO:
        casillaActual -= 12;
        cout << "Retrocedes hasta la casilla " << casillaActual + 1 << endl;
        break;
    }
    return casillaActual;
}
void buscaCasillaAvanzando(const tTablero tablero, tCasilla tipo, int& casillaActual) {
    casillaActual++;
    while (tablero[casillaActual] != tipo) {
        casillaActual++;
    }
}
void buscaCasillaRetrocediendo(const tTablero tablero, tCasilla tipo, int& casillaActual) {
    casillaActual--;
    while (tablero[casillaActual] != tipo) {
        casillaActual--;
    }
}

//Funciones para pintar el tablero
void pintaTablero(const tEstadoPartida& partida) {
    int casillasPorFila = NUM_CASILLAS / NUM_FILAS_A_DIBUJAR;
    cout << endl;

    for (int fila = 0; fila < NUM_FILAS_A_DIBUJAR; fila++) {
        pintaBorde(casillasPorFila);
        pintaNumCasilla(fila, casillasPorFila);
        pintaTipoCasilla(partida.tablero, fila, casillasPorFila);
        pintaJugadores(partida.estadoJug.casilla, fila, casillasPorFila);
    }
    pintaBorde(casillasPorFila);
    cout << endl;
}
void pintaBorde(int casillasPorFila) {

     for (int i = 1; i <= casillasPorFila; i++)
         cout << "|====";

     cout << "|" << endl;

 }
void pintaNumCasilla(int fila, int casillasPorFila) {

     for (int i = 1; i <= casillasPorFila; i++)
         cout << "| " << setw(2) << setfill('0') << i + fila * casillasPorFila << " ";

     cout << "|" << endl;

 }
void pintaTipoCasilla(tEstadoPartida& estado, int fila, int casillasPorFila) {

     for (int i = 1; i <= casillasPorFila; i++)
         cout << "|" << setw(4) << setfill(' ') << casillaAstring(estado.tablero[i - 1 + fila * casillasPorFila]);
      
     cout << "|" << endl;

 }
void pintaJugadores(tEstadoPartida& estado, int fila, int casillasPorFila) {
     

     int blancos = MAX_JUGADORES - NUM_JUGADORES;
     string b = "";
     for (int i = 0; i < blancos; i++) b = b + " ";
     cout << "|";
     for (int i = 1; i <= casillasPorFila; i++) {
         casilla = i - 1 + fila * casillasPorFila;
         for (int jug = 0; jug < NUM_JUGADORES; jug++) {
             if (casillasJ[jug] == casilla)
                 cout << jug + 1;
             else
                 cout << " ";
         }
         cout << b;
         cout << "|";
     }
     cout << endl;

 }
string casillaAstring(tCasilla casilla) {
     string cadena;
     switch (casilla) {
     case NORMAL:
         cadena = " ";
         break;
     case OCA:
         cadena = "OCA";
         break;
     case DADO1:
     case DADO2:
         cadena = "DADO";
         break;
     case PUENTE1:
     case PUENTE2:
         cadena = "PNTE";
         break;
     case POSADA:
         cadena = "PSDA";
         break;
     case CALAVERA:
         cadena = "MUER";
         break;
     case LABERINTO:
         cadena = "LBRN";
         break;
     case POZO:
         cadena = "POZO";
         break;
     case CARCEL:
         cadena = "CRCL";
         break;
     }
     return cadena;
 }
