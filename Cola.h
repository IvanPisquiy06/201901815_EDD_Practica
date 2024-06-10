#ifndef COLA_H
#define COLA_H

#include "Pasajero.h"
#include <queue>

class Cola {
private:
    std::queue<Pasajero*> cola;

public:
    void encolar(Pasajero* pasajero) {
        cola.push(pasajero);
    }

    Pasajero* desencolar() {
        if (cola.empty()) {
            return nullptr;
        }
        Pasajero* pasajero = cola.front();
        cola.pop();
        return pasajero;
    }

    bool estaVacia() {
        return cola.empty();
    }
};

#endif