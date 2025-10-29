#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

// ============================================================
// ESTRUCTURAS DE DATOS
// ============================================================

// Nodo para Lista Enlazada (Procesos Activos)
struct NodoProceso {
    int ID;
    string nombre;
    int prioridad;
    int bloqueMemoria;
    NodoProceso* siguiente;
};

// Nodo para Cola de Prioridad
struct NodoCola {
    NodoProceso* proceso;
    NodoCola* siguiente;
};

// Nodo para Pila (Memoria)
struct NodoMemoria {
    int IDProceso;
    int bloqueMemoria;
    NodoMemoria* siguiente;
};

// Nodo para Árbol Binario de Búsqueda
struct NodoArbol {
    int ID;
    string nombre;
    int prioridad;
    int bloqueMemoria;
    NodoArbol* izq;
    NodoArbol* der;
};

// ============================================================
// VARIABLES GLOBALES
// ============================================================
NodoProceso* listaProcesos = NULL;
NodoCola* colaPrioridad = NULL;
NodoMemoria* pilaMemoria = NULL;
NodoArbol* arbolProcesos = NULL;

int contadorProcesos = 0;
int contadorCola = 0;
int contadorMemoria = 0;

// ============================================================
// FUNCIONES AUXILIARES
// ============================================================

void pausar() {
    cout << "\nPresione ENTER para continuar...";
    cin.ignore();
    cin.get();
}

void limpiarPantalla() {
    system("cls");
}

// ============================================================
// FUNCIONES DE LISTA ENLAZADA
// ============================================================

bool buscarProcesoID(int ID) {
    NodoProceso* actual = listaProcesos;
    while (actual != NULL) {
        if (actual->ID == ID) {
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}

bool buscarBloqueMemoria(int bloque) {
    NodoMemoria* actual = pilaMemoria;
    while (actual != NULL) {
        if (actual->bloqueMemoria == bloque) {
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}

NodoProceso* obtenerProceso(int ID) {
    NodoProceso* actual = listaProcesos;
    while (actual != NULL) {
        if (actual->ID == ID) {
            return actual;
        }
        actual = actual->siguiente;
    }
    return NULL;
}

void insertarProceso(int ID, string nombre, int prioridad, int bloque) {
    NodoProceso* nuevo = new NodoProceso;
    nuevo->ID = ID;
    nuevo->nombre = nombre;
    nuevo->prioridad = prioridad;
    nuevo->bloqueMemoria = bloque;
    nuevo->siguiente = NULL;
    
    if (listaProcesos == NULL) {
        listaProcesos = nuevo;
    } else {
        NodoProceso* actual = listaProcesos;
        while (actual->siguiente != NULL) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevo;
    }
    contadorProcesos++;
}

void eliminarProcesoLista(int ID) {
    if (listaProcesos == NULL) return;
    
    if (listaProcesos->ID == ID) {
        NodoProceso* temp = listaProcesos;
        listaProcesos = listaProcesos->siguiente;
        delete temp;
        contadorProcesos--;
        return;
    }
    
    NodoProceso* actual = listaProcesos;
    while (actual->siguiente != NULL && actual->siguiente->ID != ID) {
        actual = actual->siguiente;
    }
    
    if (actual->siguiente != NULL) {
        NodoProceso* temp = actual->siguiente;
        actual->siguiente = actual->siguiente->siguiente;
        delete temp;
        contadorProcesos--;
    }
}

void mostrarListaProcesos() {
    limpiarPantalla();
    cout << "==========================================================" << endl;
    cout << "   LISTA DE PROCESOS ACTIVOS" << endl;
    cout << "==========================================================" << endl;
    cout << endl;
    
    if (listaProcesos == NULL) {
        cout << "No hay procesos registrados en el sistema." << endl;
        cout << endl;
        cout << "Use la opción 1 del menú para agregar procesos." << endl;
    } else {
        cout << "Total de procesos registrados: " << contadorProcesos << endl;
        cout << endl;
        cout << "ID\tNOMBRE\t\t\tPRIORIDAD\tMEMORIA" << endl;
        cout << "----------------------------------------------------------" << endl;
        
        NodoProceso* actual = listaProcesos;
        while (actual != NULL) {
            cout << actual->ID << "\t" << actual->nombre << "\t\t\t" 
                 << actual->prioridad << "\t\tBloque " << actual->bloqueMemoria << endl;
            actual = actual->siguiente;
        }
        cout << "==========================================================" << endl;
    }
    pausar();
}

// ============================================================
// FUNCIONES DE COLA DE PRIORIDAD
// ============================================================

void encolarProceso(NodoProceso* proceso) {
    NodoCola* nuevo = new NodoCola;
    nuevo->proceso = proceso;
    nuevo->siguiente = NULL;
    
    if (colaPrioridad == NULL || proceso->prioridad < colaPrioridad->proceso->prioridad) {
        nuevo->siguiente = colaPrioridad;
        colaPrioridad = nuevo;
        cout << "[OK] Proceso encolado en primera posición (prioridad " << proceso->prioridad << ")" << endl;
    } else {
        NodoCola* actual = colaPrioridad;
        while (actual->siguiente != NULL && 
               actual->siguiente->proceso->prioridad <= proceso->prioridad) {
            actual = actual->siguiente;
        }
        nuevo->siguiente = actual->siguiente;
        actual->siguiente = nuevo;
        cout << "[OK] Proceso encolado según prioridad " << proceso->prioridad << endl;
    }
    contadorCola++;
}

void desencolarProceso() {
    if (colaPrioridad == NULL) return;
    
    NodoCola* temp = colaPrioridad;
    colaPrioridad = colaPrioridad->siguiente;
    delete temp;
    contadorCola--;
}

void eliminarDeCola(int ID) {
    if (colaPrioridad == NULL) return;
    
    if (colaPrioridad->proceso->ID == ID) {
        NodoCola* temp = colaPrioridad;
        colaPrioridad = colaPrioridad->siguiente;
        delete temp;
        contadorCola--;
        return;
    }
    
    NodoCola* actual = colaPrioridad;
    while (actual->siguiente != NULL && actual->siguiente->proceso->ID != ID) {
        actual = actual->siguiente;
    }
    
    if (actual->siguiente != NULL) {
        NodoCola* temp = actual->siguiente;
        actual->siguiente = actual->siguiente->siguiente;
        delete temp;
        contadorCola--;
    }
}

void mostrarColaEjecucion() {
    limpiarPantalla();
    cout << "==========================================================" << endl;
    cout << "   COLA DE EJECUCIÓN (Planificador CPU)" << endl;
    cout << "==========================================================" << endl;
    cout << endl;
    
    if (colaPrioridad == NULL) {
        cout << "La cola de ejecución esta vacía." << endl;
        cout << endl;
        cout << "Agregue procesos usando la opción 1 del menú." << endl;
    } else {
        cout << "Procesos ordenados por prioridad (1 = máxima prioridad):" << endl;
        cout << "El primer proceso se ejecutará primero." << endl;
        cout << endl;
        cout << "POS.\tID\tNOMBRE\t\t\tPRIORIDAD" << endl;
        cout << "----------------------------------------------------------" << endl;
        
        NodoCola* actual = colaPrioridad;
        int pos = 1;
        while (actual != NULL) {
            cout << pos << "\t" << actual->proceso->ID << "\t" 
                 << actual->proceso->nombre << "\t\t\t" 
                 << actual->proceso->prioridad << endl;
            actual = actual->siguiente;
            pos++;
        }
        cout << "==========================================================" << endl;
        cout << "Total de procesos en cola: " << contadorCola << endl;
    }
    pausar();
}

// ============================================================
// FUNCIONES DE PILA (MEMORIA)
// ============================================================

void pushMemoria(int ID, int bloque) {
    NodoMemoria* nuevo = new NodoMemoria;
    nuevo->IDProceso = ID;
    nuevo->bloqueMemoria = bloque;
    nuevo->siguiente = pilaMemoria;
    pilaMemoria = nuevo;
    contadorMemoria++;
}

void eliminarMemoria(int ID) {
    if (pilaMemoria == NULL) return;
    
    if (pilaMemoria->IDProceso == ID) {
        NodoMemoria* temp = pilaMemoria;
        pilaMemoria = pilaMemoria->siguiente;
        delete temp;
        contadorMemoria--;
        return;
    }
    
    NodoMemoria* actual = pilaMemoria;
    while (actual->siguiente != NULL && actual->siguiente->IDProceso != ID) {
        actual = actual->siguiente;
    }
    
    if (actual->siguiente != NULL) {
        NodoMemoria* temp = actual->siguiente;
        actual->siguiente = actual->siguiente->siguiente;
        delete temp;
        contadorMemoria--;
    }
}

void mostrarEstadoMemoria() {
    limpiarPantalla();
    cout << "==========================================================" << endl;
    cout << "   ESTADO DE MEMORIA (Pila LIFO)" << endl;
    cout << "==========================================================" << endl;
    cout << endl;
    
    if (pilaMemoria == NULL) {
        cout << "No hay bloques de memoria asignados." << endl;
        cout << endl;
        cout << "Los bloques se asignan automáticamente al agregar procesos." << endl;
    } else {
        cout << "Bloques de memoria ocupados (LIFO):" << endl;
        cout << "LIFO = Last In First Out (Ultimo en entrar, primero en salir)" << endl;
        cout << endl;
        cout << "ORDEN\tBLOQUE\t\tPROCESO ID" << endl;
        cout << "----------------------------------------------------------" << endl;
        
        NodoMemoria* actual = pilaMemoria;
        int orden = 1;
        while (actual != NULL) {
            cout << orden << "\t" << actual->bloqueMemoria << "\t\t" 
                 << actual->IDProceso << endl;
            actual = actual->siguiente;
            orden++;
        }
        cout << "==========================================================" << endl;
        cout << "Total de bloques ocupados: " << contadorMemoria << endl;
        cout << endl;
        cout << "NOTA: El bloque en la posición 1 (tope) se libero mas recientemente." << endl;
    }
    pausar();
}

// ============================================================
// FUNCIONES DE ÁRBOL BINARIO DE BÚSQUEDA
// ============================================================

NodoArbol* insertarNodoArbol(NodoArbol* raiz, int ID, string nombre, int prioridad, int bloque) {
    if (raiz == NULL) {
        NodoArbol* nuevo = new NodoArbol;
        nuevo->ID = ID;
        nuevo->nombre = nombre;
        nuevo->prioridad = prioridad;
        nuevo->bloqueMemoria = bloque;
        nuevo->izq = NULL;
        nuevo->der = NULL;
        return nuevo;
    }
    
    if (ID < raiz->ID) {
        raiz->izq = insertarNodoArbol(raiz->izq, ID, nombre, prioridad, bloque);
    } else if (ID > raiz->ID) {
        raiz->der = insertarNodoArbol(raiz->der, ID, nombre, prioridad, bloque);
    }
    
    return raiz;
}

NodoArbol* buscarNodoArbol(NodoArbol* raiz, int ID) {
    if (raiz == NULL || raiz->ID == ID) {
        return raiz;
    }
    
    if (ID < raiz->ID) {
        return buscarNodoArbol(raiz->izq, ID);
    }
    
    return buscarNodoArbol(raiz->der, ID);
}

void recorrerInorden(NodoArbol* raiz) {
    if (raiz != NULL) {
        recorrerInorden(raiz->izq);
        cout << raiz->ID << "\t" << raiz->nombre << "\t\t\t" 
             << raiz->prioridad << "\t\tBloque " << raiz->bloqueMemoria << endl;
        recorrerInorden(raiz->der);
    }
}

NodoArbol* encontrarMinimo(NodoArbol* nodo) {
    while (nodo->izq != NULL) {
        nodo = nodo->izq;
    }
    return nodo;
}

NodoArbol* eliminarNodoArbol(NodoArbol* raiz, int ID) {
    if (raiz == NULL) return raiz;
    
    if (ID < raiz->ID) {
        raiz->izq = eliminarNodoArbol(raiz->izq, ID);
    } else if (ID > raiz->ID) {
        raiz->der = eliminarNodoArbol(raiz->der, ID);
    } else {
        if (raiz->izq == NULL) {
            NodoArbol* temp = raiz->der;
            delete raiz;
            return temp;
        } else if (raiz->der == NULL) {
            NodoArbol* temp = raiz->izq;
            delete raiz;
            return temp;
        }
        
        NodoArbol* temp = encontrarMinimo(raiz->der);
        raiz->ID = temp->ID;
        raiz->nombre = temp->nombre;
        raiz->prioridad = temp->prioridad;
        raiz->bloqueMemoria = temp->bloqueMemoria;
        raiz->der = eliminarNodoArbol(raiz->der, temp->ID);
    }
    return raiz;
}

// ============================================================
// FUNCIÓN PRINCIPAL: AGREGAR PROCESO
// ============================================================

void agregarProceso() {
    limpiarPantalla();
    cout << "==========================================================" << endl;
    cout << "   AGREGAR NUEVO PROCESO" << endl;
    cout << "==========================================================" << endl;
    cout << endl;
    
    int ID, prioridad, bloqueMemoria;
    string nombre;
    
    cout << "Ingrese el ID del proceso (número entero positivo y único):" << endl;
    cout << "ID: ";
    cin >> ID;
    cin.ignore();
    
    cout << endl << "Ingrese el nombre del proceso:" << endl;
    cout << "Nombre: ";
    getline(cin, nombre);
    
    cout << endl << "Ingrese la prioridad (1 = maxima prioridad, 2, 3, 4...):" << endl;
    cout << "Prioridad: ";
    cin >> prioridad;
    
    cout << endl << "Ingrese el bloque de memoria a asignar (número entero positivo y único):" << endl;
    cout << "Bloque: ";
    cin >> bloqueMemoria;
    
    cout << endl;
    cout << "==========================================================" << endl;
    cout << "VALIDANDO DATOS..." << endl;
    cout << "==========================================================" << endl;
    
    bool procesoValido = true;
    
    if (ID <= 0) {
        cout << "[ERROR] El ID debe ser un número positivo mayor que 0" << endl;
        procesoValido = false;
    }
    
    if (prioridad <= 0) {
        cout << "[ERROR] La prioridad debe ser un número positivo (1 o mayor)" << endl;
        procesoValido = false;
    }
    
    if (bloqueMemoria <= 0) {
        cout << "[ERROR] El bloque de memoria debe ser un número positivo" << endl;
        procesoValido = false;
    }
    
    if (nombre.length() == 0) {
        cout << "[ERROR] El nombre del proceso no puede estar vacío" << endl;
        procesoValido = false;
    }
    
    if (procesoValido && buscarProcesoID(ID)) {
        cout << "[ERROR] Ya existe un proceso con el ID " << ID << endl;
        procesoValido = false;
    }
    
    if (procesoValido && buscarBloqueMemoria(bloqueMemoria)) {
        cout << "[ERROR] El bloque de memoria " << bloqueMemoria << " ya esta ocupado" << endl;
        procesoValido = false;
    }
    
    if (procesoValido) {
        cout << endl << "Validación exitosa. Registrando proceso..." << endl << endl;
        
        insertarProceso(ID, nombre, prioridad, bloqueMemoria);
        cout << "[OK] Proceso agregado a la lista de procesos activos" << endl;
        
        pushMemoria(ID, bloqueMemoria);
        cout << "[OK] Bloque de memoria " << bloqueMemoria << " asignado al proceso " << ID << endl;
        
        NodoProceso* proceso = obtenerProceso(ID);
        encolarProceso(proceso);
        
        arbolProcesos = insertarNodoArbol(arbolProcesos, ID, nombre, prioridad, bloqueMemoria);
        cout << "[OK] Proceso indexado en arbol binario de busqueda" << endl;
        
        cout << endl;
        cout << "==========================================================" << endl;
        cout << "   PROCESO REGISTRADO EXITOSAMENTE" << endl;
        cout << "==========================================================" << endl;
        cout << "  ID:              " << ID << endl;
        cout << "  Nombre:          " << nombre << endl;
        cout << "  Prioridad:       " << prioridad << " (1 = maxima)" << endl;
        cout << "  Memoria:         Bloque " << bloqueMemoria << endl;
        cout << "==========================================================" << endl;
    } else {
        cout << endl;
        cout << "==========================================================" << endl;
        cout << "[ERROR] No se pudo agregar el proceso" << endl;
        cout << "Revise los errores mostrados arriba" << endl;
        cout << "==========================================================" << endl;
    }
    
    pausar();
}

// ============================================================
// FUNCIÓN: EJECUTAR PROCESO
// ============================================================

void ejecutarProceso() {
    limpiarPantalla();
    cout << "==========================================================" << endl;
    cout << "   EJECUTAR PROCESO (Desencolar)" << endl;
    cout << "==========================================================" << endl;
    cout << endl;
    
    if (colaPrioridad == NULL) {
        cout << "[ERROR] No hay procesos en la cola de ejecucion" << endl;
        cout << endl;
        cout << "Agregue procesos usando la opción 1 del menú." << endl;
    } else {
        NodoProceso* proceso = colaPrioridad->proceso;
        
        cout << "Proceso a ejecutar:" << endl;
        cout << "  ID:        " << proceso->ID << endl;
        cout << "  Nombre:    " << proceso->nombre << endl;
        cout << "  Prioridad: " << proceso->prioridad << endl;
        cout << "  Memoria:   Bloque " << proceso->bloqueMemoria << endl;
        cout << endl;
        cout << "==========================================================" << endl;
        cout << "EJECUTANDO PROCESO..." << endl;
        cout << "==========================================================" << endl;
        cout << "[OK] Proceso ID " << proceso->ID << " en ejecución..." << endl;
        cout << "[OK] Proceso completado exitosamente" << endl;
        
        desencolarProceso();
        cout << "[OK] Proceso removido de la cola de ejecución" << endl;
        
        cout << endl;
        cout << "==========================================================" << endl;
        cout << "   PROCESO EJECUTADO EXITOSAMENTE" << endl;
        cout << "==========================================================" << endl;
        cout << "Procesos restantes en cola: " << contadorCola << endl;
    }
    
    pausar();
}

// ============================================================
// FUNCIÓN: ELIMINAR PROCESO
// ============================================================

void eliminarProceso() {
    limpiarPantalla();
    cout << "==========================================================" << endl;
    cout << "   ELIMINAR PROCESO" << endl;
    cout << "==========================================================" << endl;
    cout << endl;
    
    if (listaProcesos == NULL) {
        cout << "[ERROR] No hay procesos en el sistema" << endl;
        cout << endl;
        cout << "Agregue procesos usando la opción 1 del menú." << endl;
    } else {
        cout << "Procesos disponibles:" << endl;
        cout << endl;
        cout << "ID\tNOMBRE\t\t\tPRIORIDAD" << endl;
        cout << "----------------------------------------------" << endl;
        
        NodoProceso* actual = listaProcesos;
        while (actual != NULL) {
            cout << actual->ID << "\t" << actual->nombre << "\t\t\t" 
                 << actual->prioridad << endl;
            actual = actual->siguiente;
        }
        
        cout << endl;
        int ID;
        cout << "Ingrese el ID del proceso a eliminar: ";
        cin >> ID;
        
        NodoProceso* proceso = obtenerProceso(ID);
        
        if (proceso != NULL) {
            cout << endl;
            cout << "==========================================================" << endl;
            cout << "ELIMINANDO PROCESO: " << proceso->nombre << " (ID: " << ID << ")" << endl;
            cout << "==========================================================" << endl;
            
            int bloque = proceso->bloqueMemoria;
            
            eliminarProcesoLista(ID);
            cout << "[OK] Proceso removido de la lista" << endl;
            
            eliminarDeCola(ID);
            cout << "[OK] Proceso removido de la cola de ejecución" << endl;
            
            eliminarMemoria(ID);
            cout << "[OK] Bloque de memoria " << bloque << " liberado" << endl;
            
            arbolProcesos = eliminarNodoArbol(arbolProcesos, ID);
            cout << "[OK] Proceso removido del indice de busqueda" << endl;
            
            cout << endl;
            cout << "==========================================================" << endl;
            cout << "   PROCESO ELIMINADO EXITOSAMENTE" << endl;
            cout << "==========================================================" << endl;
            cout << "Procesos activos restantes: " << contadorProcesos << endl;
        } else {
            cout << endl << "[ERROR] No existe un proceso con el ID " << ID << endl;
        }
    }
    
    pausar();
}

// ============================================================
// FUNCIÓN: MOSTRAR ÁRBOL (ÍNDICE DE PROCESOS)
// ============================================================

void mostrarArbolProcesos() {
    limpiarPantalla();
    cout << "==========================================================" << endl;
    cout << "   INDICE DE PROCESOS (Arbol Binario de Busqueda)" << endl;
    cout << "==========================================================" << endl;
    cout << endl;
    
    if (arbolProcesos == NULL) {
        cout << "No hay procesos indexados en el sistema." << endl;
        cout << endl;
        cout << "Use la opción 1 del menu para agregar procesos." << endl;
    } else {
        cout << "Procesos ordenados por ID (recorrido inorden):" << endl;
        cout << "Optimiza las busquedas con complejidad O(log n)" << endl;
        cout << endl;
        cout << "ID\tNOMBRE\t\t\tPRIORIDAD\tMEMORIA" << endl;
        cout << "----------------------------------------------------------" << endl;
        recorrerInorden(arbolProcesos);
        cout << "==========================================================" << endl;
        cout << "Total de procesos indexados: " << contadorProcesos << endl;
    }
    pausar();
}

// ============================================================
// MENÚ PRINCIPAL
// ============================================================

int main() {
    int opcion;
    bool continuar = true;
    
    while (continuar) {
        limpiarPantalla();
        cout << "==========================================================" << endl;
        cout << "   SISTEMA DE GESTIÓN DE PROCESOS - GRUPO C" << endl;
        cout << "   Estructura de Datos - NRC: 62159" << endl;
        cout << "==========================================================" << endl;
        cout << endl;
        cout << "MENU PRINCIPAL:" << endl;
        cout << "  1. Agregar Nuevo Proceso" << endl;
        cout << "  2. Mostrar Lista de Procesos" << endl;
        cout << "  3. Mostrar Cola de Ejecución" << endl;
        cout << "  4. Mostrar Estado de Memoria" << endl;
        cout << "  5. Mostrar Índice de Procesos (Árbol BST)" << endl;
        cout << "  6. Ejecutar Proceso (Desencolar)" << endl;
        cout << "  7. Eliminar Proceso" << endl;
        cout << "  0. Salir del Sistema" << endl;
        cout << "==========================================================" << endl;
        cout << endl;
        cout << "Estado actual del sistema:" << endl;
        cout << "  - Procesos Activos: " << contadorProcesos << endl;
        cout << "  - Procesos en Cola: " << contadorCola << endl;
        cout << "  - Bloques de Memoria Ocupados: " << contadorMemoria << endl;
        cout << endl;
        cout << "Seleccione una opción: ";
        cin >> opcion;
        
        switch (opcion) {
            case 1:
                agregarProceso();
                break;
            case 2:
                mostrarListaProcesos();
                break;
            case 3:
                mostrarColaEjecucion();
                break;
            case 4:
                mostrarEstadoMemoria();
                break;
            case 5:
                mostrarArbolProcesos();
                break;
            case 6:
                ejecutarProceso();
                break;
            case 7:
                eliminarProceso();
                break;
            case 0:
                limpiarPantalla();
                cout << "==========================================================" << endl;
                cout << "   CERRANDO SISTEMA DE GESTIÓN DE PROCESOS" << endl;
                cout << "==========================================================" << endl;
                cout << endl;
                cout << "Gracias por usar el sistema." << endl;
                cout << "Grupo C - NRC 62159" << endl;
                cout << endl;
                cout << "==========================================================" << endl;
                continuar = false;
                break;
            default:
                limpiarPantalla();
                cout << "==========================================================" << endl;
                cout << "[ERROR] Opción no válida" << endl;
                cout << "==========================================================" << endl;
                cout << endl;
                cout << "Por favor, seleccione una opción vlida (0-7)." << endl;
                pausar();
        }
    }
    
    return 0;
}
