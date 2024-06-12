#ifndef PASAJEROS_H
#define PASAJEROS_H

#include "Nodo.h"
#include "Pasajero.h"
#include <stdexcept>

class Cola {
private:
    Nodo* front;
    Nodo* rear;
    int size;

public:
    Cola() : front(nullptr), rear(nullptr), size(0) {}

    bool estaVacia() const {
        return size == 0;
    }

    void encolar(Pasajero* pasajero) {
        Nodo* newNode = new Nodo(static_cast<void*>(pasajero));
        if (rear == nullptr) {
            front = rear = newNode;
        } else {
            rear->setSiguiente(newNode);
            rear = newNode;
        }
        size++;
    }

    Pasajero* desencolar() {
        if (estaVacia()) {
            throw std::out_of_range("Queue is empty");
        }
        Nodo* temp = front;
        Pasajero* pasajero = static_cast<Pasajero*>(front->getDato());
        front = front->getSiguiente();
        if (front == nullptr) {
            rear = nullptr;
        }
        delete temp;
        size--;
        return pasajero;
    }

    Pasajero* peekFront() const {
        if (estaVacia()) {
            throw std::out_of_range("Queue is empty");
        }
        return static_cast<Pasajero*>(front->getDato());
    }

    ~Cola() {
        while (!estaVacia()) {
            desencolar();
        }
    }
};

#endif // PASAJEROS_H