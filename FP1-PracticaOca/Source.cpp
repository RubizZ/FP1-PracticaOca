/**
* Juego de la OCA v3 realizado por Rubén Hidalgo Arias y Álvaro Moreno Arribas
* ATENCION: El formato de guardado de partidas es diferente al dado en el pdf de la practica, es necesario añadir el nombre del tablero al principio de cada partida
*           (no hace falta que exista puesto que solo se usa para mostrarlo en pantalla) y añadir un -1 al final de cada partida, o jugar partidas de 0 y guardarlas
*/

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

const int MIN_JUGADORES = 2;
const int MAX_JUGADORES = 4;

const int NUM_FILAS_A_DIBUJAR = 3;

const int CENTINELA = 0;
const int CENTINELA_2 = -1;
//------------------------------------//

//Tipos definidos de casillas
typedef enum { NORMAL, OCA, PUENTE1, PUENTE2, POZO, POSADA, LABERINTO, DADO1, DADO2, CARCEL, CALAVERA } tCasilla;
typedef tCasilla tTablero[NUM_CASILLAS];

//Tipos definidos del estado de los jugadores
typedef struct {
	int casilla;
	int penalizaciones;
} tEstadoJugador;
typedef tEstadoJugador tEstadoJugadores[MAX_JUGADORES];

//Tipos definidos de las partidas no acabadas
typedef struct {
	tEstadoJugadores estadoJug;
    int numJugadores;
	int turno;
	tTablero tablero;
    string nombreTablero;
}tEstadoPartida;
typedef tEstadoPartida tTablaPartidas[MAX_PARTIDAS];
typedef struct {
	int cont;
	tTablaPartidas listaPartidas;
}tListaPartidas;

//Funciones que no cambian
void iniciaTablero(tTablero tablero);
bool cargaTableroNuevo(tTablero tablero, const string nombre);
tCasilla stringAtCasilla(string casilla);
int tirarDado();
int tirarDadoManual();
int saltaACasilla(const tTablero tablero, int casillaActual);
void buscaCasillaAvanzando(const tTablero tablero, tCasilla tipo, int& casillaActual);
void buscaCasillaRetrocediendo(const tTablero tablero, tCasilla tipo, int& casillaActual);
bool esCasillaPremio(const tTablero tablero, int casillaActual);
int quienEmpieza(int max);
void pintaNumCasilla(int fila, int casillasPorFila);
void pintaBorde(int casillasPorFila);
string casillaAstring(tCasilla casilla);

//Funciones modificadas
void iniciaJugadores(tEstadoPartida& partida);
void efectoTirada(const tTablero tablero, tEstadoJugador& estadoJug);
void tirada(const tTablero tablero, tEstadoJugador& estadoJug);
bool partida(tEstadoPartida& estado);
void pintaTablero(const tEstadoPartida& partida);
void pintaTipoCasilla(const tTablero tablero, int fila, int casillasPorFila);
void pintaJugadores(const tEstadoPartida estado, int fila, int casillasPorFil);
void cambioDeTurno(int& turno, const int numJugadores);

//Funciones nuevas
bool cargaPartidas(tListaPartidas& partidas, const string nombre);
void cargaTablero(tTablero tablero, ifstream& archivo);
int cargaJugadores(tEstadoJugadores& jugadores, ifstream& archivo);
void eliminarPartida(tListaPartidas& partidas, int indice); //Desplaza todas las partidas un lugar a la izquierda
void insertaNuevaPartida(tListaPartidas& partidas, const tEstadoPartida& partidaNueva); //Inserta una nueva partida en un archivo, o crea uno nuevo
void guardaPartidas(const tListaPartidas& partidas, const string nombre);
void guardaTablero(const tTablero tablero, ofstream& archivo);
void guardaJugadores(const tEstadoJugadores jugadores, const int numJugadores, ofstream& archivo);
void creaPartidaNueva(tEstadoPartida& partida); //Carga en partida una nueva partida
string casillaAstringCompleto(const tCasilla casilla); //Transforma tCasilla a un string completo ej. "PUENTE1" para que se reconozca al cargar partidas

int main() {
    srand(time(NULL));
    
    tEstadoPartida partidaNueva;
    int opcion = 0;
    char aux;

    cout << "Quieres empezar una nueva partida o continuar una antigua?\n1. Nueva partida\n2. Continuar partida\n3. Salir\n\n-> ";
    cin >> opcion;
    cin.get(aux);
    while (opcion < 1 || opcion > 3) {
        cout << "\nEsa opcion no es valida, escribe un numero del 1 al 3\n\n-> ";
        cin >> opcion;
        cin.get(aux);
    }
    switch (opcion) {
    case 1: //Empieza una partida de 0
        creaPartidaNueva(partidaNueva);
        if (!partida(partidaNueva)) { //Si no ha acabado la partida la guarda en un archivo
            tListaPartidas partidas;
            insertaNuevaPartida(partidas, partidaNueva); //Carga las partidas y guarda la no terminada
            if (partidas.cont != 0) cout << endl << "La partida se ha guardado en el slot " << partidas.cont << endl;
        }
        cout << "Terminando el programa..." << endl;
        break;
    case 2: //Carga una partida guardada
        tListaPartidas partidas;
        int indexPartida;
        string nombre;

        cout << "\nEscribe el nombre del archivo de partidas\n\n-> ";
        getline(cin, nombre);
        
        while (!cargaPartidas(partidas, nombre)) {
            cout << "No se han podido cargar las partidas" << endl;
            cout << "\nEscribe el nombre del archivo de partidas\n\n-> ";
            getline(cin, nombre);
        }
        
        if (partidas.cont == 1) cout << endl << "Hay " << partidas.cont << " partida guardadas" << endl;
        else cout << endl << "Hay " << partidas.cont << " partidas guardadas" << endl;
        
        cout << "Que partida quieres continuar?\n\n";
        for (int i = 0; i < partidas.cont; i++) { //Escribe en pantalla toda la informacion de las partidas guardadas en el archivo "nombre"
            cout << i + 1 << ". Jugando en " << partidas.listaPartidas[i].nombreTablero << " Turno de J" << partidas.listaPartidas[i].turno + 1 << endl;
            for (int j = 0; j < partidas.listaPartidas[i].numJugadores; j++) {
                cout << " J" << j + 1 << ": " << partidas.listaPartidas[i].estadoJug[j].casilla;
                if (partidas.listaPartidas[i].estadoJug[j].penalizaciones > 0) cout << " Penalizacion " << partidas.listaPartidas[i].estadoJug[j].penalizaciones << endl;
                else cout << endl;
            }
        }
        cout << "\n-> ";
        cin >> indexPartida;
        cin.get(aux);
        cout << "\nContinuando partida " << indexPartida << endl;
        indexPartida--;
        
        if (partida(partidas.listaPartidas[indexPartida])) { //Si partida() es true, la partida ha acabado, por lo que se elimina
            eliminarPartida(partidas, indexPartida);
            cout << endl << "La partida del slot " << indexPartida + 1 << " se ha eliminado" << endl;
            guardaPartidas(partidas, nombre);
            if (partidas.cont >= 1) cout << "Todas las partidas ubicadas en un slot >=" << indexPartida + 2 << " se van a desplazar un slot a la izquierda" << endl;
            else cout << "No hay partidas a guardar" << endl;
        }
        else { //Si es false, simplemente actualiza el archivo
            guardaPartidas(partidas, nombre);
            cout << endl << "La partida del slot " << indexPartida + 1 << " se ha actualizado" << endl;
        }
        cout << "Terminando el programa..." << endl;
        break;
    }
    if (opcion == 3) {
        cout << "Saliendo del programa" << endl;
    }
    return 777;
}
    
//Funciones principales
bool partida(tEstadoPartida& partida) {
    bool acabada = true, ganador = false;

    pintaTablero(partida);

    if (partida.turno == -1) { //Si se empieza una partida nueva partida.turno se "inicializa" a -1
        partida.turno = quienEmpieza(partida.numJugadores);
        cout << "Empieza el jugador " << partida.turno + 1 << endl;
    } else cout << "Es el turno del jugador " << partida.turno + 1 << endl << endl; //Si no es -1 es porque es una partida empezada

    while (!ganador && acabada) {
        
        string input;
        cout << "Pulsa enter para tirar el dado o escribe SALIR. ";
        getline(cin, input);

        if (input != "SALIR") { 
            tirada(partida.tablero, partida.estadoJug[partida.turno]);
            if (partida.estadoJug[partida.turno].casilla >= NUM_CASILLAS - 1) {
                partida.estadoJug[partida.turno].casilla = NUM_CASILLAS - 1;
                pintaTablero(partida);
                cout << "Ha ganado el jugador " << partida.turno + 1 << endl;
                ganador = true;
            }
            else pintaTablero(partida);

            while (esCasillaPremio(partida.tablero, partida.estadoJug[partida.turno].casilla) && input != "SALIR") {
                if (partida.estadoJug[partida.turno].casilla < NUM_CASILLAS - 1) {
                    cout << "Vuelves a tirar" << endl;
                    cout << "Pulsa enter para tirar el dado o escribe SALIR. ";
                    getline(cin, input);
                    if (input != "SALIR") {
                        tirada(partida.tablero, partida.estadoJug[partida.turno]);
                        if (partida.estadoJug[partida.turno].casilla >= NUM_CASILLAS - 1) {
                            partida.estadoJug[partida.turno].casilla = NUM_CASILLAS - 1;
                            pintaTablero(partida);
                            cout << "Ha ganado el jugador " << partida.turno + 1 << endl;
                            ganador = true;
                        }
                        else pintaTablero(partida);
                    }
                    else acabada = false;
                }
            }
            if (!ganador) {
                cambioDeTurno(partida.turno, partida.numJugadores);
                while (partida.estadoJug[partida.turno].penalizaciones > 0) {
                    if (partida.estadoJug[partida.turno].penalizaciones != 1) cout << "El jugador " << partida.turno + 1 << " esta penalizado, le quedan " << partida.estadoJug[partida.turno].penalizaciones << " turnos sin jugar" << endl;
                    else cout << "El jugador " << partida.turno + 1 << " esta penalizado, le queda " << partida.estadoJug[partida.turno].penalizaciones << " turno sin jugar" << endl;
                    partida.estadoJug[partida.turno].penalizaciones--;
                    cambioDeTurno(partida.turno, partida.numJugadores);
                }
                cout << "Es el turno del jugador " << partida.turno + 1 << endl << endl;
            }
        }
        else acabada = false; //Si el usuario quiere salir, acabada se convierte en false
    }
    return acabada;
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
void cambioDeTurno(int& turno, const int numJugadores) {
    if (turno < numJugadores - 1) {
        turno++;
    }
    else turno = 0;
}

//Funciones del dado
int tirarDado() {
    return (rand() % 6) + 1;
}
int tirarDadoManual() {
    int dado;
    cout << "Introduce el valor del dado: ";
    cin >> dado;
    return dado;
}

//Funciones iniciales para establecer el tablero y jugadores
void iniciaTablero(tTablero tablero) { //Inicializa todo el tablero a normal excepto NUM_CASILLAS-1 que es la OCA 63
	for (int i = 0; i < NUM_CASILLAS - 1; i++) {
		tablero[i] = NORMAL;
	}
	tablero[NUM_CASILLAS - 1] = OCA;
}
bool cargaTableroNuevo(tTablero tablero, const string nombre) {
    ifstream archivo;
    int posicion;
    string tipo;
    tCasilla tipotCasilla;
    bool satisfactorio = false;

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
void iniciaJugadores(tEstadoPartida& partida) {
    for (int i = 0; i < partida.numJugadores; i++) {
        partida.estadoJug[i].casilla = 0;
        partida.estadoJug[i].penalizaciones = 0;
    }
    partida.turno = -1;
}
int quienEmpieza(int max) {
    return rand() % max;
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
        pintaJugadores(partida, fila, casillasPorFila);
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
void pintaJugadores(const tEstadoPartida estado, int fila, int casillasPorFila) {
     int casilla;

     int blancos = MAX_JUGADORES - estado.numJugadores;
     string b = "";
     for (int i = 0; i < blancos; i++) b = b + " ";
     cout << "|";
     for (int i = 1; i <= casillasPorFila; i++) {
         casilla = i - 1 + fila * casillasPorFila;
         for (int jug = 0; jug < estado.numJugadores; jug++) {
             if (estado.estadoJug[jug].casilla == casilla)
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
     default:
         cadena = " ";
         break;
     }
     return cadena;
 }

//Funciones del sistema de partidas
void insertaNuevaPartida(tListaPartidas& partidas, const tEstadoPartida& partidaNueva) {
    ifstream input;
    string nombre;
    int numeroPartidas = 0;
    bool aux;

    cout << "\nEscribe el nombre del archivo de partidas\n\n-> ";
    getline(cin, nombre);

    input.open(nombre);

    if (input.is_open()) {
        input >> numeroPartidas;
        input.close();
        if (numeroPartidas >= 0 && numeroPartidas < 10) {
            aux = cargaPartidas(partidas, nombre);
            if (partidas.cont == 1) cout << endl << "Hay " << partidas.cont << " partida guardada" << endl;
            else cout << endl << "Hay " << partidas.cont << " partidas guardadas" << endl;
            partidas.listaPartidas[partidas.cont] = partidaNueva;
            partidas.cont++;
            guardaPartidas(partidas, nombre);
        }
        else {
            int opcion = 0;

            cout << endl << "El archivo no tiene el formato de un archivo de partidas o se ha superado el maximo de partidas guardadas en el" << endl;
            cout << "Quieres sobreescribir el archivo?\n1. Si\n2. No, elegir otro archivo\n3. No, eliminar partida\n\n-> ";
            cin >> opcion;
            char aux2;
            cin.get(aux2);
            while (opcion < 1 || opcion > 3) {
                cout << "\nEsa opcion no es valida, escribe un numero del 1 al 3\n\n-> ";
                cin >> opcion;
                cin.get(aux2);
            }
            switch (opcion) {
            case 1:
                cout << "\nEstas seguro de que quieres sobreescribir el archivo " << nombre << "? Se va a borrar todo su contenido\n1. Si\n2. No\n\n-> ";
                cin >> opcion;
                cin.get(aux2);
                while (opcion < 1 || opcion > 2) {
                    cout << "\nEsa opcion no es valida, escribe un numero del 1 al 2\n\n-> ";
                    cin >> opcion;
                    cin.get(aux2);
                }
                switch (opcion) {
                case 1:
                    partidas.cont = 1;
                    partidas.listaPartidas[partidas.cont - 1] = partidaNueva;
                    guardaPartidas(partidas, nombre);
                    break;
                case 2:
                    partidas.cont = 0;
                    cout << "Eliminando partida..." << endl;
                    break;
                }
                break;
            case 2:
                insertaNuevaPartida(partidas, partidaNueva);
                break;
            case 3:
                partidas.cont = 0;
                cout << "Eliminando partida..." << endl;
                break;
            }
        }
    }
    else {
        cout << endl << "Se va a crear un nuevo archivo de guardado llamado " << nombre << endl;
        partidas.cont = 1;
        partidas.listaPartidas[partidas.cont - 1] = partidaNueva;
        guardaPartidas(partidas, nombre);
    }
}
bool cargaPartidas(tListaPartidas& partidas, const string nombre) {
    ifstream archivo;
    bool satisfactorio = false;

    archivo.open(nombre);

    if (archivo.is_open()) {
        archivo >> partidas.cont;
        
        if (partidas.cont >= 1) {
            for (int i = 0; i < partidas.cont; i++) {
                archivo >> partidas.listaPartidas[i].nombreTablero;
                iniciaTablero(partidas.listaPartidas[i].tablero);
                cargaTablero(partidas.listaPartidas[i].tablero, archivo);
                archivo >> partidas.listaPartidas[i].turno;
                partidas.listaPartidas[i].numJugadores = cargaJugadores(partidas.listaPartidas[i].estadoJug, archivo);
            }
            satisfactorio = true;
        }
        archivo.close();
    }
    return satisfactorio;
}
void guardaPartidas(const tListaPartidas& partidas, string nombre) {
    ofstream archivo;

    archivo.open(nombre);

    archivo << partidas.cont << endl;
    for (int i = 0; i < partidas.cont; i++) {
        archivo << partidas.listaPartidas[i].nombreTablero << endl;
        guardaTablero(partidas.listaPartidas[i].tablero, archivo);
        archivo << partidas.listaPartidas[i].turno << endl;
        guardaJugadores(partidas.listaPartidas[i].estadoJug, partidas.listaPartidas[i].numJugadores, archivo);
    }
    archivo.close();
}

//Subfunciones del sistema de partidas
void cargaTablero(tTablero tablero, ifstream& archivo) {
    int posicion;
    string tipo;
    tCasilla tipotCasilla;
    archivo >> posicion;
    while (posicion != CENTINELA) {
        char aux;
        archivo.get(aux);
        getline(archivo, tipo);
        tipotCasilla = stringAtCasilla(tipo);
        tablero[posicion - 1] = tipotCasilla;
        archivo >> posicion;
    }
    tablero[NUM_CASILLAS - 1] = OCA;
}
int cargaJugadores(tEstadoJugadores& jugadores, ifstream& archivo) {
    int input;
    int numJugador = 0;

    archivo >> input;
    while (input != CENTINELA_2) {
        jugadores[numJugador].casilla = input;
        archivo >> jugadores[numJugador].penalizaciones;
        archivo >> input;
        numJugador++;
    }
    return numJugador;
}
void guardaTablero(const tTablero tablero, ofstream& archivo){
    int posicion = 0;
    tCasilla casilla;
    while (posicion < NUM_CASILLAS - 1) {
        casilla = tablero[posicion];
        if (casilla != NORMAL) {
            archivo << posicion + 1 << " " << casillaAstringCompleto(casilla) << endl;
        }
        posicion++;
    }
    archivo << CENTINELA << endl;
}
void guardaJugadores(const tEstadoJugadores jugadores, const int numJugadores, ofstream& archivo) {
    int numJugador = 0;
    while (numJugador < numJugadores) {
        archivo << jugadores[numJugador].casilla << " " << jugadores[numJugador].penalizaciones << endl;
        numJugador++;
    }
    archivo << CENTINELA_2 << endl;                           
}
void creaPartidaNueva(tEstadoPartida& partida) {
    char aux;
    cout << "\nCuantos jugadores (" << MIN_JUGADORES << "-" << MAX_JUGADORES << ") van a jugar la partida? \n\n-> ";
    cin >> partida.numJugadores;
    cin.get(aux);
    while (partida.numJugadores < MIN_JUGADORES || partida.numJugadores > MAX_JUGADORES) {
        cout << "\nIntroduce un numero entre " << MIN_JUGADORES << " y " << MAX_JUGADORES << "\n\n-> ";
        cin >> partida.numJugadores;
        cin.get(aux);
    }
    iniciaJugadores(partida);

    iniciaTablero(partida.tablero);
    cout << "\nIntroduce el nombre del archivo del tablero: \n\n-> ";
    getline(cin, partida.nombreTablero);
    while (!cargaTableroNuevo(partida.tablero, partida.nombreTablero)) {
        cout << "\nHa ocurrido un error al abrir el archivo, prueba otra vez\n\n-> ";
        getline(cin, partida.nombreTablero);
    }
}
void eliminarPartida(tListaPartidas& partidas, int index) {
    for (int i = index; i < partidas.cont - 1; i++) {
        partidas.listaPartidas[i] = partidas.listaPartidas[i + 1];
    }
    partidas.cont--;
}
string casillaAstringCompleto(const tCasilla casilla) {
    string cadena;
    switch (casilla) {
    case NORMAL:
        cadena = " ";
        break;
    case OCA:
        cadena = "OCA";
        break;
    case DADO1:
        cadena = "DADO1";
        break;
    case DADO2:
        cadena = "DADO2";
        break;
    case PUENTE1:
        cadena = "PUENTE1";
        break;
    case PUENTE2:
        cadena = "PUENTE2";
        break;
    case POSADA:
        cadena = "POSADA";
        break;
    case CALAVERA:
        cadena = "CALAVERA";
        break;
    case LABERINTO:
        cadena = "LABERINTO";
        break;
    case POZO:
        cadena = "POZO";
        break;
    case CARCEL:
        cadena = "CARCEL";
        break;
    default:
        cadena = " ";
        break;
    }
    return cadena;
}