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
        listaPasajerosRegistrados.reordenarPorVuelo();
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

template<typename T>
void generarReporteAviones(const T& lista, const std::string& filename) {
    std::string dotFileName = filename + ".dot";
    std::ofstream file(dotFileName);
    
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    file << "digraph ListaCircularDoble {\n";
    file << "    rankdir=LR;\n";
    file << "    Primero [shape=plaintext, label=\"Primero\"];";
    file << "    Ultimo [shape=plaintext, label=\"Último\"];";

    if (!lista.estaVacia()) {
        Nodo* actual = lista.getCabeza();
        Nodo* primero = actual;
        Nodo* siguiente = static_cast<Nodo*>(primero->getSiguiente());
        Nodo* ultimo = static_cast<Nodo*>(actual->getAnterior());
        do {
            Avion* avionData = static_cast<Avion*>(actual->getDato());

            if (avionData) {
                file << "    \"" << actual << "\" [label=\"Avion: " << avionData->numero_de_registro << "\\n Estado:" << avionData->estado << "\"];\n";
            } 
            if (actual->getSiguiente() != primero) {
                file << "    \"" << actual << "\" -> \"" << actual->getSiguiente() << "\";\n";
                file << "    \"" << actual->getSiguiente() << "\" -> \"" << actual << "\" [constraint=false];\n";
            }

            actual = siguiente;
            siguiente = static_cast<Nodo*>(actual->getSiguiente());
        } while (actual != primero);

        file << "    \"" << primero << "\" -> \"" << ultimo << "\" [constraint=false];\n";
        file << "    \"" << ultimo << "\" -> \"" << primero << "\" [constraint=false];\n";
        file << "    Primero -> \"" << primero << "\" [constraint=false];\n";
        file << "    Ultimo -> \"" << ultimo << "\" [constraint=false];\n";
    }

    file << "}\n";
    file.close();

    std::string command = "dot -Tpng " + dotFileName + " -o " + filename + ".png";
    system(command.c_str());
}

template<typename T>
void generarReportePasajerosDoble(const T& container, const std::string& filename) {
    std::string dotFileName = filename + ".dot";
    std::ofstream file(dotFileName);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    file << "digraph G {\n";
    file << "    rankdir=LR;\n";

    if (!container.estaVacia()) {
        auto current = container.getCabeza();
        do {
            Pasajero* pasajeroData = static_cast<Pasajero*>(current->getDato());
            if (pasajeroData) {
                file << "    \"" << current << "\" [label=\"Pasajero: " << pasajeroData->nombre 
                     << "\\nPasaporte: " << pasajeroData->numero_de_pasaporte << "\"];\n";
            }

            if (current->getSiguiente() != nullptr) {
                file << "    \"" << current << "\" -> \"" << current->getSiguiente() << "\";\n";
            }

            current = current->getSiguiente();
        } while (current != nullptr && current != container.getCabeza());
    }

    file << "}\n";
    file.close();

    std::string command = "dot -Tpng " + dotFileName + " -o " + filename + ".png";
    system(command.c_str());
}

void generarReporteCola(const Cola& cola, const std::string& filename) {
    std::string dotFileName = filename + ".dot";
    std::ofstream file(dotFileName);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    if(cola.estaVacia()){
        std::cout << "La cola esta vacia" << std::endl;
        return;
    }

    file << "digraph G {\n";
    file << "    rankdir=LR;\n";
    file << "    head;\n";
    file << "    tail;\n";

    auto current = cola.getPrimero();
    int index = 0;
    while (current != nullptr) {
        Pasajero* pasajeroData = static_cast<Pasajero*>(current->getDato());
        if(index == 0){
            file << index << "-> head \"" << "\";\n";
        }
        if (pasajeroData) {
            file << "    \"" << index << "\" [label=\"Pasajero: " << pasajeroData->nombre 
                 << "\\nPasaporte: " << pasajeroData->numero_de_pasaporte << "\"];\n";
        }

        if (current->getSiguiente() != nullptr) {
            file << "    \"" << index + 1 << "\" -> \"" << index << "\";\n";
        } else {
            file << "    \"" << "\" tail ->" << index << ";\n";
        }

        current = current->getSiguiente();
        index++;
    }

    file << "}\n";
    file.close();

    std::string command = "dot -Tpng " + dotFileName + " -o " + filename + ".png";
    system(command.c_str());
}

void generarReportePila(const Pila& pila, const std::string& filename) {
    std::string dotFileName = filename + ".dot";
    std::ofstream file(dotFileName);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    file << "digraph G {\n";
    file << "    rankdir=LR;\n";
    file << "    Top;\n";

    int index = 0;
    auto current = pila.getCabeza();
    while (current != nullptr) {
        Pasajero* pasajeroData = static_cast<Pasajero*>(current->getDato());
        if (pasajeroData) {
            file << "    \"" << index << "\" [label=\"Nombre: " << pasajeroData->nombre 
                 << "\\nEquipaje: " << pasajeroData->equipaje_facturado << "\"];\n";
        }

        if (current->getSiguiente() != nullptr) {
            file << "    \"" << index << "\" -> \"" << index + 1 << "\";\n";
        } else {
            file << "    \"" << "\" -> Top\"" << index << "\";\n";
        }

        current = current->getSiguiente();
        index++;
    }

    file << "}\n";
    file.close();

    std::string command = "dot -Tpng " + dotFileName + " -o " + filename + ".png";
    system(command.c_str());
}

void mostrarMenu() {
    std::cout << "1. Cargar aviones" << std::endl;
    std::cout << "2. Cargar pasajeros" << std::endl;
    std::cout << "3. Carga de movimientos" << std::endl;
    std::cout << "4. Consultar pasajeros" << std::endl;
    std::cout << "5. Generar reporte" << std::endl;
    std::cout << "6. Salir" << std::endl;
}

void mostrarMenuReportes() {
    std::cout << "1. Aviones Disponibles" << std::endl;
    std::cout << "2. Aviones en mantenimiento" << std::endl;
    std::cout << "3. Cola de Registro" << std::endl;
    std::cout << "4. Pila de Equipaje" << std::endl;
    std::cout << "5. Lista de pasajeros" << std::endl;
    std::cout << "6. Salir" << std::endl;
}

void menuReportes(){
    int opcion;
    while (true) {
        mostrarMenuReportes();
        std::cin >> opcion;
        switch (opcion) {
            case 1:
                generarReporteAviones(listaDisponibles, "listaAvionesDisponibles");
                break;
            case 2:
                generarReporteAviones(listaMantenimiento, "listaAvionesMantenimiento");
                break;
            case 3:
                generarReporteCola(colaPasajeros, "colaPasajeros");
                break;
            case 4:
                generarReportePila(pilaEquipajes, "pilaEquipajes");
                break;
            case 5:
                generarReportePasajerosDoble(listaPasajerosRegistrados, "listaPasajerosRegistrados");
                break;
            case 6:
                return;
            default:
                std::cout << "Opción no válida." << std::endl;
        }
    }
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
                cargarAviones(archivo);
                break;
            case 2:
                std::cout << "Ingrese el nombre del archivo de pasajeros: ";
                std::cin >> archivo;
                archivo_prueba = "C:\\Proyectos\\U\\EDD\\practica\\archivos_prueba\\pasajeros.json";
                cargarPasajeros(archivo);
                break;
            case 3:
                std::cout << "Ingrese el nombre del archivo de movimientos: ";
                std::cin >> archivo;
                ejecutarMovimientos(archivo);
                break;
            case 4:
                std::cout << "Pasajeros registrados:" << std::endl;
                listaPasajerosRegistrados.mostrarPasajeros();
                break;
            case 5:
                menuReportes();
                break;
            case 6:
                return 0;
            default:
                std::cout << "Opción no válida." << std::endl;
        }
    }

    return 0;
}
