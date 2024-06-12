#ifndef LISTA_CIRCULAR_DOBLE_H
#define LISTA_CIRCULAR_DOBLE_H

#include "Avion.h"
#include "Nodo.h"
#include <iostream>

class ListaCircularDoble {
private:
    Nodo* cabeza;

public:
    ListaCircularDoble() : cabeza(nullptr) {}

    ~ListaCircularDoble() {
        if (!cabeza) return;
        Nodo* actual = static_cast<Nodo*>(cabeza->getSiguiente());
        while (actual != cabeza) {
            Nodo* siguiente = static_cast<Nodo*>(actual->getSiguiente());
            delete actual;
            actual = siguiente;
        }
        delete cabeza;
        cabeza = nullptr;
    }

    void insertar(Avion* avion) {
        Nodo* nuevo = new Nodo(avion);
        if (!cabeza) {
            cabeza = nuevo;
            nuevo->setSiguiente(nuevo);
            nuevo->setAnterior(nuevo);
        } else {
            Nodo* ultimo = static_cast<Nodo*>(cabeza->getAnterior());
            ultimo->setSiguiente(nuevo);
            nuevo->setAnterior(ultimo);
            nuevo->setSiguiente(cabeza);
            cabeza->setAnterior(nuevo);
        }
    }

    void mostrar() const {
        if (!cabeza) {
            std::cout << "Lista vacía." << std::endl;
            return;
        }
        Nodo* actual = cabeza;
        do {
            Avion* avion = static_cast<Avion*>(actual->getDato());
            std::cout << "Avion: " << avion->vuelo << ", Modelo: " << avion->modelo << ", Estado: " << avion->estado << std::endl;
            actual = static_cast<Nodo*>(actual->getSiguiente());
        } while (actual != cabeza);
    }
};

#endif