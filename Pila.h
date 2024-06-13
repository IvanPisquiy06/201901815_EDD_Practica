#ifndef PILA_H
#define PILA_H

#include "Nodo.h"

class Pila {
private:
    Nodo* cabeza;

public:
    Pila() : cabeza(nullptr) {}

    ~Pila() {
        while (cabeza != nullptr) {
            desapilar(); 
        }
    }

    void apilar(Pasajero* pasajero) {
        Nodo* nuevoNodo = new Nodo(pasajero);
        if (cabeza != nullptr) {
            nuevoNodo->setSiguiente(cabeza);
        }
        cabeza = nuevoNodo;
    }

    Pasajero* desapilar() {
        if (cabeza == nullptr) {
            return nullptr; 
        }
        Nodo* nodoAEliminar = cabeza;
        Pasajero* pasajero = static_cast<Pasajero*>(nodoAEliminar->getDato());
        cabeza = cabeza->getSiguiente();
        delete nodoAEliminar;
        return pasajero;
    }
};

#endif // PILA_H