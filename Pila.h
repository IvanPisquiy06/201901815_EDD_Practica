#ifndef PILA_H
#define PILA_H

#include <stack>
#include "Pasajero.h"

class Pila {
private:
    std::stack<Pasajero*> pila;

public:
    void apilar(Pasajero* pasajero) {
        pila.push(pasajero);
    }

    Pasajero* desapilar() {
        if (pila.empty()) {
            return nullptr;
        }
        Pasajero* pasajero = pila.top();
        pila.pop();
        return pasajero;
    }

    bool estaVacia() {
        return pila.empty();
    }
};

#endif // PILA_H
