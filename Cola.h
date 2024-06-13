#ifndef PASAJEROS_H
#define PASAJEROS_H

#include "Nodo.h"
#include "Pasajero.h"
#include <stdexcept>

class Cola {
private:
    Nodo* frente;
    Nodo* cola;
    int tamaño;

public:
    Cola() : frente(nullptr), cola(nullptr), tamaño(0) {}

    bool estaVacia() const {
        return tamaño == 0;
    }

    void encolar(Pasajero* pasajero) {
        Nodo* newNode = new Nodo(static_cast<void*>(pasajero));
        if (cola == nullptr) {
            frente = cola = newNode;
        } else {
            cola->setSiguiente(newNode);
            cola = newNode;
        }
        tamaño++;
    }

    Pasajero* desencolar() {
        if (estaVacia()) {
            throw std::out_of_range("Queue is empty");
        }
        Nodo* temp = frente;
        Pasajero* pasajero = static_cast<Pasajero*>(frente->getDato());
        frente = frente->getSiguiente();
        if (frente == nullptr) {
            cola = nullptr;
        }
        delete temp;
        tamaño--;
        return pasajero;
    }

    Pasajero* peekFront() const {
        if (estaVacia()) {
            throw std::out_of_range("Cola vacía");
        }
        return static_cast<Pasajero*>(frente->getDato());
    }

    ~Cola() {
        while (!estaVacia()) {
            desencolar();
        }
    }
};

#endif // PASAJEROS_H