#ifndef NODO_H
#define NODO_H

#include <iostream>
using namespace std;

class Nodo
{
private:
    void* dato;
    Nodo* anterior;
    Nodo* siguiente;

public:
    Nodo(void* dato);
    Nodo* getSiguiente();
    Nodo* getAnterior();
    void setSiguiente(Nodo* siguiente);
    void setAnterior(Nodo* anterior);
    void* getDato();
    void setDato(void* dato); 
    ~Nodo();
};

Nodo::Nodo(void* dato)
{
    this->dato = dato;
    this->anterior = nullptr;
    this->siguiente = nullptr;
}

Nodo* Nodo::getSiguiente() { return siguiente; }
void Nodo::setSiguiente(Nodo* siguiente) { this->siguiente = siguiente; }
Nodo* Nodo::getAnterior() { return anterior; }
void Nodo::setAnterior(Nodo* anterior) { this->anterior = anterior; }
void* Nodo::getDato() { return dato; }
void Nodo::setDato(void* dato) { this->dato = dato; }
Nodo::~Nodo() {
}

#endif // NODO_H