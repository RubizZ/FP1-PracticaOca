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
int saltaACasilla(const tTablero tablero, int casillaActual);
void buscaCasillaAvanzando(const tTablero tablero, tCasilla tipo, int& casillaActual);
void buscaCasillaRetrocediendo(const tTablero tablero, tCasilla tipo, int& casillaActual);
bool esCasillaPremio(const tTablero tablero, int casillaActual);
int quienEmpieza(int max);
void pintaNumCasilla(int fila, int casillasPorFila);
void pintaBorde(int casillasPorFila);
string casillaAstring(tCasilla casilla);

//Funciones modificadas
void iniciaJugadores(tEstadoJugadores jugadores, int numJugadores);
void efectoTirada(const tTablero tablero, tEstadoJugador& estadoJug);
void tirada(const tTablero tablero, tEstadoJugador& estadoJug);
int tirarDado();
int tirarDadoManual();
bool partida(tEstadoPartida& estado);
void pintaTablero(const tEstadoPartida& partida);
void pintaTipoCasilla(const tTablero tablero, int fila, int casillasPorFila);
void pintaJugadores(const tEstadoPartida estado, int fila, int casillasPorFila);
void cambioDeTurno(int& turno, const int numJugadores);

//Funciones nuevas
bool cargaPartidas(tListaPartidas& partidas);
void cargaTablero(tTablero tablero, ifstream& archivo);
int cargaJugadores(tEstadoJugadores& jugadores, ifstream& archivo);
void eliminarPartida(tListaPartidas& partidas, int indice);
//bool insertaNuevaPartida(tListaPartidas& partidas, const tEstadoPartida& partidaOca); //A priori no va a hacer falta
void guardaPartidas(const tListaPartidas& partidas);
void guardaTablero(const tTablero tablero, ofstream& archivo);
void guardaJugadores(const tEstadoJugadores jugadores, const int numJugadores, ofstream& archivo);
void creaPartidaNueva(tEstadoPartida partida);


int main() { //IMPORTANTE CAMBIAR TODAS LAS CONSTANTES NUM_JUGADORES a numJugadores que esta almacenado en partidas.listaPartidas[].numJugadores <---------------------------
    srand(time(NULL));
    
    tListaPartidas partidas;
    int opcion, indexPartida = -1;
    bool acabada = true;

    cout << "Quieres empezar una nueva partida o continuar una antigua?\n1. Nueva partida\n2. Cargar partida\n3. Salir\n\n-> ";
    cin >> opcion;
    while (opcion < 1 || opcion > 3) {
        cout << "Esa opcion no es valida, escribe un numero del 1 al 3\n\n-> ";
        cin >> opcion;
    }
    char aux;
    cin.get(aux);
    switch (opcion) {
    case 1:
        int input;
        cout << "\nQuieres cargar un fichero de partidas? (Esta partida se gurdara en el primer slot disponible)\n1. Continuar sin fichero\n2. Cargar fichero\n\n-> ";
        cin >> input;
        while (opcion < 1 || opcion > 2) {
            cout << "Esa opcion no es valida, escribe un numero del 1 al 2\n\n-> ";
            cin >> opcion;
        }
        switch (input) {
        case 1:
            partidas.cont = 0;
            indexPartida = partidas.cont;
            creaPartidaNueva(partidas.listaPartidas[indexPartida]);
            partidas.cont++;
            acabada = partida(partidas.listaPartidas[indexPartida]);
            break;
        case 2:
            while (!cargaPartidas(partidas)) cout << "No se han podido cargar las partidas" << endl;

            if (partidas.cont < MAX_PARTIDAS) {
                indexPartida = partidas.cont; //Posicion del primer index vacio y cambiado a otra variable para que sea mas legible
                creaPartidaNueva(partidas.listaPartidas[indexPartida]);
                partidas.cont++;
                acabada = partida(partidas.listaPartidas[indexPartida]);
            }
            else {
                cout << "Hay el maximo (" << MAX_PARTIDAS << ") de partidas guardadas, termina una de las anteriores para crear otra nueva";
                acabada = true;
            }
            break;
        }
        break;
    case 2:
        while (!cargaPartidas(partidas)) cout << "No se han podido cargar las partidas" << endl;
        cout << "Que partida quieres continuar? 1-" << partidas.cont << "\n\n-> ";
        for (int i = 0; i < partidas.cont; i++) {
            cout << "Partida " << i + 1 << ": " << partidas.listaPartidas[i].numJugadores << "jugadores en el tablero " << partidas.listaPartidas[i].nombreTablero;
        }
        cin >> indexPartida;
        indexPartida--;
        acabada = partida(partidas.listaPartidas[indexPartida]);
        break;
    }
    if (opcion != 3) {
        if (!acabada) {
            guardaPartidas(partidas);
            cout << "La partida se va a guardar en el slot " << indexPartida + 1 << endl;
        }
        else {
            eliminarPartida(partidas, indexPartida);
            guardaPartidas(partidas);
        }
    }
    return 0;
}

//Funciones principales
bool partida(tEstadoPartida& partida) {//Hay que cambiar esto en plan esta funcion.
    bool acabada = true, ganador = false;

    pintaTablero(partida);

    partida.turno = quienEmpieza(partida.numJugadores);
    cout << "Empieza el jugador " << partida.turno + 1 << endl;

    while (!ganador && acabada) {
        
        string input;
        cout << "Pulsa enter para tirar el dado o escribe SALIR. ";
        getline(cin, input);

        if (input != "SALIR") {
            tirada(partida.tablero, partida.estadoJug[partida.turno]);
            if (partida.estadoJug[partida.turno].casilla >= NUM_CASILLAS - 1) {
                partida.estadoJug[partida.turno].casilla = NUM_CASILLAS - 1;
                pintaTablero(partida);
                cout << "Ha ganado el jugador " << partida.turno + 1;
                acabada = true;
            }
            else {
                partida.estadoJug[partida.turno].casilla = partida.estadoJug[partida.turno].casilla;
                pintaTablero(partida);
            }

            while (esCasillaPremio(partida.tablero, partida.estadoJug[partida.turno].casilla) && !ganador) {
                if (partida.estadoJug[partida.turno].casilla < NUM_CASILLAS - 1) {
                    cout << "Vuelves a tirar" << endl;
                    tirada(partida.tablero, partida.estadoJug[partida.turno]);
                    if (partida.estadoJug[partida.turno].casilla >= NUM_CASILLAS - 1) {
                        ganador = true;
                        partida.estadoJug[partida.turno].casilla = NUM_CASILLAS - 1;
                    }
                    pintaTablero(partida);
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
        else acabada = false;
 
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
void iniciaJugadores(tEstadoJugadores jugadores, int numJugadores) {
    for (int i = 0; i < numJugadores; i++) {
        jugadores[i].casilla = 0;
        jugadores[i].penalizaciones = 0;
    }
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
     }
     return cadena;
 }

//Funciones del sistema de partidas
bool cargaPartidas(tListaPartidas& partidas) {
    ifstream archivo;
    string nombre;
    bool satisfactorio = false;

    cout << "Escribe el nombre del archivo de partidas, si no existe se creara\n\n-> ";
    getline(cin, nombre);

    archivo.open(nombre);

    if (archivo.is_open()) {
        archivo >> partidas.cont;
        
        if (partidas.cont >= 1) {
            cout << "Hay " << partidas.cont << " partidas guardadas" << endl;
            for (int i = 0; i++; i < partidas.cont) {
                archivo >> partidas.listaPartidas[i].nombreTablero;
                cargaTablero(partidas.listaPartidas[i].tablero, archivo);
                archivo >> partidas.listaPartidas[i].turno;
                partidas.listaPartidas[i].numJugadores = cargaJugadores(partidas.listaPartidas[i].estadoJug, archivo);
            }
            satisfactorio = true;
        }
        else {
            cout << "Este fichero no es un archivo de partidas, borralo o cambiale el nombre";
        }
        archivo.close();
    }
    else {
        cout << "No existe el archivo" << nombre << endl;
    }
    return satisfactorio;
}
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
void eliminarPartida(tListaPartidas& partidas, int index) {
    for (int i = index; i < partidas.cont - 1; i++) {
        partidas.listaPartidas[i] = partidas.listaPartidas[i + 1];
    }
    partidas.cont--;
}
void guardaPartidas(const tListaPartidas& partidas){
    ofstream archivo;
    string nombre;

    cout << "Introduce el nombre del fichero en el que guardar las partidas: ";
    getline(cin, nombre);

    archivo.open(nombre);
    
    if (archivo.is_open()) {
        archivo << partidas.cont << endl;
        for (int i = 0; i < partidas.cont; i++) {
            archivo << partidas.listaPartidas[i].nombreTablero << endl;
            guardaTablero(partidas.listaPartidas[i].tablero, archivo);
            archivo << partidas.listaPartidas[i].turno << endl;
            guardaJugadores(partidas.listaPartidas[i].estadoJug, partidas.listaPartidas[i].numJugadores, archivo);
        }
        archivo.close();
    }
    else {
        cout << "No se ha podido guardar las partidas";
    }
}
void guardaTablero(const tTablero tablero, ofstream& archivo){
    int posicion = 0;
    tCasilla casilla;
    while (posicion < NUM_CASILLAS) {
        casilla = tablero[posicion];
        if (casilla != NORMAL) {
            archivo << posicion << " " << casillaAstring(casilla) << endl;
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
void creaPartidaNueva(tEstadoPartida partida) {
    cout << "\nCuantos jugadores (" << MIN_JUGADORES << "-" << MAX_JUGADORES << ") van a jugar la partida? \n\n-> ";
    cin >> partida.numJugadores;
    string aux;
    getline(cin, aux);
    while (partida.numJugadores < MIN_JUGADORES || partida.numJugadores > MAX_JUGADORES) {
        cout << "\nIntroduce un numero entre " << MIN_JUGADORES << " y " << MAX_JUGADORES << "\n\n-> ";
        cin >> partida.numJugadores;
    }
    iniciaJugadores(partida.estadoJug, partida.numJugadores);

    iniciaTablero(partida.tablero);
    cout << "\nIntroduce el nombre del fichero del tablero: \n\n-> ";
    getline(cin, partida.nombreTablero);
    while (!cargaTableroNuevo(partida.tablero, partida.nombreTablero)) {
        cout << "\nHa ocurrido un error al abrir el archivo, prueba otra vez\n\n-> ";
        getline(cin, partida.nombreTablero);
    }
}


//cargaJugadores antiguo
/*
void cargaJugadores(tEstadoJugadores& jugadores, ifstream& archivo) {
    for (int i = 0; i++; i < NUM_JUGADORES) {
        archivo >> jugadores[i].casilla;
        archivo >> jugadores[i].penalizaciones;

    }
}
*/
