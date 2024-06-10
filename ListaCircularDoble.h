#ifndef LISTA_CIRCULAR_DOBLE_H
#define LISTA_CIRCULAR_DOBLE_H

#include "Avion.h"
#include <iostream>

class Nodo {
public:
    Avion* avion;
    Nodo* siguiente;
    Nodo* anterior;

    Nodo(Avion* avion) : avion(avion), siguiente(nullptr), anterior(nullptr) {}
};

class ListaCircularDoble {
private:
    Nodo* cabeza;

public:
    ListaCircularDoble() : cabeza(nullptr) {}

    void insertar(Avion* avion) {
        Nodo* nuevo = new Nodo(avion);
        if (!cabeza) {
            cabeza = nuevo;
            cabeza->siguiente = cabeza;
            cabeza->anterior = cabeza;
        } else {
            Nodo* ultimo = cabeza->anterior;
            nuevo->siguiente = cabeza;
            nuevo->anterior = ultimo;
            cabeza->anterior = nuevo;
            ultimo->siguiente = nuevo;
        }
    }

    void mostrar() {
        if (!cabeza) {
            std::cout << "Lista vacía." << std::endl;
            return;
        }
        Nodo* actual = cabeza;
        do {
            std::cout << "Avion: " << actual->avion->vuelo << ", Modelo: " << actual->avion->modelo << ", Estado: " << actual->avion->estado << std::endl;
            actual = actual->siguiente;
        } while (actual != cabeza);
    }
};

#endif
