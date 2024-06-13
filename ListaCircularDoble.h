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

    Avion* eliminar(const std::string& numero_de_registro) {
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

    void mostrar() const {
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

    bool estaVacia() const {
        return cabeza == nullptr;
    }
};

#endif // LISTA_CIRCULAR_DOBLE_H