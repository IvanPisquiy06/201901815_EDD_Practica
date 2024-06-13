#ifndef LISTA_CIRCULAR_DOBLE_H
#define LISTA_CIRCULAR_DOBLE_H

#include "Avion.h"
#include "Pasajero.h"
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

    Nodo* getCabeza() const {return cabeza;}

    void insertar(void* dato) {
        Nodo* nuevo = new Nodo(dato);
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

    Avion* eliminarAvion(const std::string& numero_de_registro) {
        if (!cabeza) return nullptr;

        Nodo* actual = cabeza;
        Nodo* anterior = nullptr;
        bool encontrado = false;

        do {
            Avion* avion = static_cast<Avion*>(actual->getDato());
            if (avion->numero_de_registro == numero_de_registro) {
                std::cout << "Avion encontrado" << std::endl;
                encontrado = true;
            }
            anterior = actual;
            actual = static_cast<Nodo*>(actual->getSiguiente());
        } while (actual != cabeza);

        if (!encontrado) return nullptr;

        if (actual == cabeza && actual->getSiguiente() == cabeza) {
            cabeza = nullptr;
        } else {
            if (actual == cabeza) cabeza = static_cast<Nodo*>(actual->getSiguiente());
            anterior->setSiguiente(actual->getSiguiente());
            static_cast<Nodo*>(actual->getSiguiente())->setAnterior(anterior);
        }
        Avion* avion = static_cast<Avion*>(actual->getDato());
        delete actual;
        return avion;
    }

    void mostrarAviones() const {
        if (!cabeza) {
            std::cout << "Lista vacía." << std::endl;
            return;
        }
        Nodo* actual = cabeza;
        do {
            Avion* avion = static_cast<Avion*>(actual->getDato());
            std::cout << "Numero de Registro: " << avion->numero_de_registro << ", Modelo: " << avion->modelo << ", Estado: " << avion->estado << std::endl;
            actual = static_cast<Nodo*>(actual->getSiguiente());
        } while (actual != cabeza);
    }

    void mostrarPasajeros() const {
        if (!cabeza) {
            std::cout << "Lista vacía." << std::endl;
            return;
        }
        Nodo* actual = cabeza;
        do {
            Pasajero* pasajero = static_cast<Pasajero*>(actual->getDato());
            std::cout << "Nombre: " << pasajero->nombre << ", vuelo: " << pasajero->vuelo << ", asiento: " << pasajero->asiento << ", Equipaje: " << pasajero->equipaje_facturado << std::endl;
            actual = static_cast<Nodo*>(actual->getSiguiente());
        } while (actual != cabeza);
    }

    void reordenarPorVuelo() {
    if (!cabeza || cabeza->getSiguiente() == cabeza) return;

    bool wasSwapped;
    do {
        Nodo* current = cabeza;
        wasSwapped = false;
        do {
            Nodo* next = static_cast<Nodo*>(current->getSiguiente());
            Pasajero* currentPasajero = static_cast<Pasajero*>(current->getDato());
            Pasajero* nextPasajero = static_cast<Pasajero*>(next->getDato());

            // Extract numeric part of vuelo and compare
            int currentVueloNum = std::stoi(currentPasajero->vuelo.substr(1));
            int nextVueloNum = std::stoi(nextPasajero->vuelo.substr(1));

            if (currentVueloNum > nextVueloNum) {
                // Swap logic for a doubly linked list
                Nodo* prev = static_cast<Nodo*>(current->getAnterior());
                Nodo* nextNext = static_cast<Nodo*>(next->getSiguiente());

                prev->setSiguiente(next);
                next->setAnterior(prev);

                next->setSiguiente(current);
                current->setAnterior(next);

                current->setSiguiente(nextNext);
                nextNext->setAnterior(current);

                if (current == cabeza) cabeza = next; // Update head if necessary
                wasSwapped = true;
            } else {
                current = next;
            }
        } while (static_cast<Nodo*>(current->getSiguiente()) != cabeza);
    } while (wasSwapped);
}

    bool estaVacia() const {
        return cabeza == nullptr;
    }
};

#endif // LISTA_CIRCULAR_DOBLE_H