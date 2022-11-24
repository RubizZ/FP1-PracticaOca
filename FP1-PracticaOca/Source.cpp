#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <time.h>

using namespace std;

const int MODO_DEBUG = true;

const int CASILLA_INICIAL = 1;
const int NUM_CASILLAS = 63;

const int TURNOS_POSADA = 1;
const int TURNOS_CARCEL = 2;
const int TURNOS_POZO = 3;
const int RETROCESO_LABERINTO = 12;
const int NUM_JUGADORES = 2;
const int MAX_JUGADORES = 4;
const int NUM_FILAS_A_DIBUJAR = 3;
const int CENTINELA = 0;

typedef enum { NORMAL, OCA, PUENTE1, PUENTE2, POZO, POSADA, LABERINTO, DADO1, DADO2, CARCEL, CALAVERA } tCasilla;
typedef tCasilla tTablero[NUM_CASILLAS];
typedef int tJugadores[NUM_JUGADORES];

//Subprogramas para cargar el tablero
void iniciaTablero(tTablero tablero);
bool cargaTablero(tTablero tablero);
tCasilla stringAtCasilla(string casilla);

//Subprogramas para pintar el tablero o mostrar informaci�n
void pintaTablero(const tTablero tablero, const tJugadores casillasJ);
void pintaNumCasilla(int fila, int casillasPorFila);
void pintaBorde(int casillasPorFila);
void pintaTipoCasilla(const tTablero tablero, int fila, int casillasPorFila);
void pintaJugadores(const tJugadores casillasJ, int fila, int casillasPorFila);
string casillaAstring(tCasilla casilla);

//Subprogramas de la partida
int tirarDado();
int tirarDadoManual();
int quienEmpieza();
int saltaACasilla(tTablero tablero, int casillaActual);
int buscaCasillaAvanzando(tTablero tablero, int casillaActual);
int buscaCasillaRetrocediendo(tTablero tablero, int casillaActual);
int efectoTirada(tTablero tablero, int& casillaActual);
void iniciaJugadores(tJugadores jugadores, tJugadores penalizaciones);
int partida(tTablero tablero, tJugadores jugadores, tJugadores penalizaciones);
int tirada(tTablero tablero, int& casillaActual);
int cambioDeTurno(int turno);

int main() {
    srand(time(NULL));
    tTablero tablero;
    tJugadores jugadores, penalizaciones;
    
    //Inicializa el tablero a NORMAL y carga el tablero del archivo a introducir
    iniciaTablero(tablero);
    while (!cargaTablero(tablero)) cout << "Ha ocurrido un error al abrir el archivo" << endl;

    cout << "El ganador de la partida es el jugador " << partida(tablero, jugadores, penalizaciones) + 1 << endl;
    return 0;
}

int tirarDado() {
    string i;

    //Pregunta para que el usuario "tire" un dado, no siguiendo con el codigo hasta que pulse enter
    cout << endl << "Pulsa enter para tirar el dado" << endl;

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
int quienEmpieza() {
    return rand() % NUM_JUGADORES;
}

void iniciaJugadores(tJugadores jugadores, tJugadores penalizaciones) {
    //Situa a todos los jugadores en la casilla 1 (con index 0) y les asigna 0 penalizaciones
    for (int i = 0; i < NUM_JUGADORES; i++) {
        jugadores[i] = 0;
        penalizaciones[i] = 0;
    }
}
int partida(tTablero tablero, tJugadores jugadores, tJugadores penalizaciones) {
    int casillaActual = 0;
    int turno = quienEmpieza(); // Del 0 hasta NUM_JUGADORES - 1
    bool finPartida = false;

    iniciaJugadores(jugadores, penalizaciones);
    pintaTablero(tablero, jugadores);
    cout << "Empieza el jugador " << turno + 1 << endl;

    while (!finPartida) {
        penalizaciones[turno] = tirada(tablero, casillaActual);
        if (casillaActual >= NUM_CASILLAS - 1) {
            finPartida = true;
            jugadores[turno] = NUM_CASILLAS - 1;
        } else jugadores[turno] = casillaActual;
        pintaTablero(tablero, jugadores);

        if (!finPartida) {
            turno = cambioDeTurno(turno);
            while (penalizaciones[turno] > 0) {
                penalizaciones[turno]--;
                turno = cambioDeTurno(turno);
            }
            cout << "Es el turno del jugador " << turno + 1 << endl;
            casillaActual = jugadores[turno];
        }
    }
    return turno;
}
int tirada(tTablero tablero, int& casillaActual) {

    int dado;
    int penalizacion = 0;

    //Tira dados
    if (!MODO_DEBUG) dado = tirarDado();
    else dado = tirarDadoManual();

    cout << "Has sacado un " << dado << endl;

    casillaActual += dado;

    cout << "Estas en la casilla " << casillaActual + 1 << endl;

    if (casillaActual < NUM_CASILLAS - 1) {
        penalizacion = efectoTirada(tablero, casillaActual);
    }

    return penalizacion;
}
int efectoTirada(tTablero tablero, int& casillaActual) {
    int penalizacion = 0;
    
    if (casillaActual < NUM_CASILLAS - 1) {
        switch (tablero[casillaActual]) {
        case OCA:
            if (casillaActual < NUM_CASILLAS - 1) {
                cout << "De oca a oca y tiro porque me toca" << endl;
                casillaActual = saltaACasilla(tablero, casillaActual);
                cout << "Avanzas hasta la casilla " << casillaActual + 1 << endl;
                penalizacion = tirada(tablero, casillaActual);
            }
            break;
        case DADO1:
            cout << "De dado a dado y tiro porque me ha tocado" << endl;
            casillaActual = saltaACasilla(tablero, casillaActual);
            cout << "Avanzas hasta la casilla " << casillaActual + 1 << endl;
            penalizacion = tirada(tablero, casillaActual);
            break;
        case DADO2:
            cout << "De dado a dado y tiro porque me ha tocado" << endl;
            casillaActual = saltaACasilla(tablero, casillaActual);
            cout << "Retrocedes hasta la casilla " << casillaActual + 1 << endl;
            penalizacion = tirada(tablero, casillaActual);
            break;
        case PUENTE1:
            cout << "De puente a puente y tiro porque me lleva la corriente" << endl;
            casillaActual = saltaACasilla(tablero, casillaActual);
            cout << "Avanzas hasta la casilla " << casillaActual + 1 << endl;
            penalizacion = tirada(tablero, casillaActual);
            break;
        case PUENTE2:
            cout << "De puente a puente y tiro porque me lleva la corriente" << endl;
            casillaActual = saltaACasilla(tablero, casillaActual);
            cout << "Retrocedes hasta la casilla " << casillaActual + 1 << endl;
            penalizacion = tirada(tablero, casillaActual);
            break;
        case CALAVERA:
            cout << "Has caido en la muerte" << endl;
            casillaActual = saltaACasilla(tablero, casillaActual);
            cout << "Vuelve a la casilla" << casillaActual + 1 << endl;
            break;
        case LABERINTO:
            cout << "Has caido en el laberinto" << endl;
            casillaActual = saltaACasilla(tablero, casillaActual);
            cout << "Retrocedes hasta " << casillaActual + 1 << endl;
            break;
        case POSADA:
            cout << "Has caido en la posada" << endl;
            cout << "Te quedas " << TURNOS_POSADA << " turno sin jugar" << endl;
            penalizacion = TURNOS_POSADA;
            break;
        case CARCEL:
            cout << "Has caido en la carcel" << endl;
            cout << "Te quedas " << TURNOS_CARCEL << " turnos sin jugar" << endl;
            penalizacion = TURNOS_CARCEL;
            break;
        case POZO:
            cout << "Has caido en el pozo" << endl;
            cout << "Te quedas " << TURNOS_POZO << " turnos sin jugar" << endl;
            penalizacion = TURNOS_POZO;
            break;
        }
    }
    return penalizacion;
}
int cambioDeTurno(int turno) {
    if (turno < NUM_JUGADORES - 1) {
        turno++;
    }
    else turno = 0;
    return turno;
}

int saltaACasilla(tTablero tablero, int casillaActual) {
    switch (tablero[casillaActual]) {
    case OCA:
    case DADO1:
    case PUENTE1:
        casillaActual = buscaCasillaAvanzando(tablero, casillaActual);
        break;
    case DADO2:
    case PUENTE2:
        casillaActual = buscaCasillaRetrocediendo(tablero, casillaActual);
        break;
    case CALAVERA:
        casillaActual = 0;
        break;
    case LABERINTO:
        casillaActual -= 12;
        break;
    }
    return casillaActual;
}
int buscaCasillaAvanzando(tTablero tablero, int casillaActual) {
    casillaActual++;
    switch (tablero[casillaActual - 1]) {
    case OCA:
        while (tablero[casillaActual] != OCA) {
            casillaActual++;
        }
        break;
    case PUENTE1:
        while (tablero[casillaActual] != PUENTE2) {
            casillaActual++;
        }
        break;
    case DADO1:
        while (tablero[casillaActual] != DADO2) {
            casillaActual++;
        }
        break;
    }
    return casillaActual;
}
int buscaCasillaRetrocediendo(tTablero tablero, int casillaActual) {
    casillaActual--;
    switch (tablero[casillaActual + 1]) {
    case PUENTE2:
        while (tablero[casillaActual] != PUENTE1) {
            casillaActual--;
        }
        break;
    case DADO2:
        while (tablero[casillaActual] != DADO1) {
            casillaActual--;
        }
        break;
    }
    return casillaActual;
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
void pintaTablero(const tTablero tablero, const tJugadores casillasJ) {
    int casillasPorFila = NUM_CASILLAS / NUM_FILAS_A_DIBUJAR; 
    cout << endl;
    for (int fila = 0; fila < NUM_FILAS_A_DIBUJAR; fila++) {
        pintaBorde(casillasPorFila);
        pintaNumCasilla(fila, casillasPorFila);
        pintaTipoCasilla(tablero, fila, casillasPorFila);
        pintaJugadores(casillasJ, fila, casillasPorFila);
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
void pintaTipoCasilla(const tTablero tablero, int fila, int casillasPorFila) {

    for (int i = 1; i <= casillasPorFila; i++)
        cout << "|" << setw(4) << setfill(' ') << casillaAstring(tablero[i - 1 + fila * casillasPorFila]);

    cout << "|" << endl;

}
void pintaJugadores(const tJugadores casillasJ, int fila, int casillasPorFila) {
    int casilla;

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


//-----------------------------------------CODIGO UTIL PARA USAR DESPUES-----------------------------------------//

//Comprobar que el dado se ha pasado la meta para volver para atras no usado en la v1
/*

if (casillaActual + dado < NUM_CASILLAS) {
    casillaActual += dado;
}
else casillaActual = NUM_CASILLAS - (dado - (NUM_CASILLAS - casillaActual));

*/

//Cout de el array tTablero
/*

for (int i = 0; i < NUM_CASILLAS; i++) {
    cout << i << " " << tablero[i] << endl;
}

 */