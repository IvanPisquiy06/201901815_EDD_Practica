#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include "libs/json.hpp"
#include "ListaCircularDoble.h"
#include "Cola.h"
#include "Pila.h"
#include "Pasajero.h"
#include "Avion.h"

using json = nlohmann::json;

ListaCircularDoble listaDisponibles;
ListaCircularDoble listaMantenimiento;
ListaCircularDoble listaPasajerosRegistrados;
Cola colaPasajeros;
Pila pilaEquipajes;

void cargarAviones(const std::string& archivo) {
    std::ifstream ifs(archivo);
    if (!ifs.is_open()) {
        std::cerr << "Error opening file: " << archivo << std::endl;
        return;
    }
    json obj;
    ifs >> obj; 

    for (const auto& avion : obj) {
        Avion* nuevoAvion = new Avion(
            avion["vuelo"].get<std::string>(),
            avion["numero_de_registro"].get<std::string>(),
            avion["modelo"].get<std::string>(),
            avion["fabricante"].get<std::string>(),
            avion["ano_fabricacion"].get<int>(),
            avion["capacidad"].get<int>(),
            avion["peso_max_despegue"].get<int>(),
            avion["aerolinea"].get<std::string>(),
            avion["estado"].get<std::string>()
        );
        if (nuevoAvion->estado == "Disponible") {
            listaDisponibles.insertar(nuevoAvion);
        } else if (nuevoAvion->estado == "Mantenimiento") {
            listaMantenimiento.insertar(nuevoAvion);
        }
    }
}

void cargarPasajeros(const std::string& archivo) {
    std::ifstream ifs(archivo);
    if (!ifs.is_open()) {
        std::cerr << "Error opening file: " << archivo << std::endl;
        return;
    }
    json obj;
    ifs >> obj;

    for (const auto& pasajero : obj) {
        Pasajero* nuevoPasajero = new Pasajero(
            pasajero["nombre"].get<std::string>(),
            pasajero["nacionalidad"].get<std::string>(),
            pasajero["numero_de_pasaporte"].get<std::string>(),
            pasajero["vuelo"].get<std::string>(),
            pasajero["asiento"].get<int>(),
            pasajero["destino"].get<std::string>(),
            pasajero["origen"].get<std::string>(),
            pasajero["equipaje_facturado"].get<int>()
        );
        colaPasajeros.encolar(nuevoPasajero);
    }
}

void mantenimiento(const std::string accion, const std::string& numero_de_registro) {
    Avion* avion = nullptr;

    if (accion == "Ingreso" && listaDisponibles.estaVacia() == false) {
        avion = listaDisponibles.eliminarAvion(numero_de_registro);
        if (avion) {
            avion->estado = "Mantenimiento";
            listaMantenimiento.insertar(avion);
            std::cout << "Avion " << numero_de_registro << " ingresado a la lista de mantenimiento." << std::endl;
        }
    } else if (accion == "Salida" && listaMantenimiento.estaVacia() == false) {
        avion = listaMantenimiento.eliminarAvion(numero_de_registro);
        if (avion) {
            avion->estado = "Disponible";
            listaDisponibles.insertar(avion);
            std::cout << "Avion " << numero_de_registro << " salio de la lista de mantenimiento." << std::endl;
        }
    } else {
        std::cout << "Accion no valida o avion no encontrado." << std::endl;
    }

    if (!avion) {
        std::cout << "Avion " << numero_de_registro << " no encontrado." << std::endl;
    }
}

void ingresoEquipaje() {
    Pasajero* pasajero = colaPasajeros.desencolar();
    if (pasajero && pasajero->equipaje_facturado > 0) {
        pilaEquipajes.apilar(pasajero);
        std::cout << "Equipaje registrado para pasajero: " << pasajero->nombre << std::endl;
    }
    if(pasajero){
        listaPasajerosRegistrados.insertar(pasajero);
    } else{
        std::cout << "No hay pasajeros en la cola" << std::endl;
    }
}

void ejecutarMovimientos(const std::string& archivo) {
    std::ifstream file(archivo);
    std::string line;

    while (std::getline(file, line)) {
        line.erase(std::remove(line.begin(), line.end(), ';'), line.end());
        std::istringstream iss(line);
        std::string action;
        std::getline(iss, action, ',');

        if (action == "IngresoEquipajes") {
            ingresoEquipaje();
        } else if (action == "MantenimientoAviones") {
            std::string accion, numero_de_registro;
            std::getline(iss, accion, ',');
            std::getline(iss, numero_de_registro);
            mantenimiento(accion, numero_de_registro);
        }
    }
}

void mostrarMenu() {
    std::cout << "1. Cargar aviones" << std::endl;
    std::cout << "2. Cargar pasajeros" << std::endl;
    std::cout << "3. Mostrar aviones" << std::endl;
    std::cout << "4. Mostrar pasajeros" << std::endl;
    std::cout << "5. Carga de movimientos" << std::endl;
    std::cout << "6. Salir" << std::endl;
}

int main() {
    int opcion;
    std::string archivo;
    std::string numero_de_registro;
    std::string accion;

    while (true) {
        mostrarMenu();
        std::cin >> opcion;
        std::string archivo_prueba;

        switch (opcion) {
            case 1:
                std::cout << "Ingrese el nombre del archivo de aviones: ";
                std::cin >> archivo;
                archivo_prueba = "C:\\Proyectos\\U\\EDD\\practica\\archivos_prueba\\aviones.json";
                cargarAviones(archivo_prueba);
                break;
            case 2:
                std::cout << "Ingrese el nombre del archivo de pasajeros: ";
                std::cin >> archivo;
                archivo_prueba = "C:\\Proyectos\\U\\EDD\\practica\\archivos_prueba\\pasajeros.json";
                cargarPasajeros(archivo_prueba);
                break;
            case 3:
                std::cout << "Aviones disponibles:" << std::endl;
                listaDisponibles.mostrarAviones();
                std::cout << "Aviones en mantenimiento:" << std::endl;
                listaMantenimiento.mostrarAviones();
                break;
            case 4:
                std::cout << "Pasajeros registrados:" << std::endl;
                listaPasajerosRegistrados.mostrarPasajeros();
                break;
            case 5:
                ejecutarMovimientos("C:\\Proyectos\\U\\EDD\\practica\\archivos_prueba\\movimientos.txt");
                break;
            case 6:
                return 0;
            default:
                std::cout << "Opción no válida." << std::endl;
        }
    }

    return 0;
}
