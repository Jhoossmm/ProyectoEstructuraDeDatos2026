#include "Grafo.h"

//Constructor
Grafo::Grafo() {}

//Insertar arista en el "grafo"
void Grafo::insertar_arista(int u, int v, double peso, bool es_dirigida) {
    lista_adyacencia[u].push_back({v, peso});
    
    //Si el grafo no es dirigido, también agregar la conexión inversa
    if (!es_dirigido) {
        lista_adyacencia[v].push_back({u, peso});
    }
    
    //Asegurarse que el vertice destino también exista en la lista de adyacencia
    if (lista_adyacencia.find(v) == lista_adyacencia.end()) {
        lista_adyacencia[v] = vector<Arista>();
    }

    //Aumenta el grado de entrada al insertar la arista, en caso de no ser dirigido aumenta el grado de entrada de ambos nodos
    grado_entrada[v]++; 
    if (grado_entrada.find(u) == grado_entrada.end())
        grado_entrada[u] = 0;
    if (!es_dirigida)
        grado_entrada[u]++;
}


//Funcion de prueba para mostrar algunos nodos y su grado de salida
void Grafo::imprimir_muestra_nodos(unordered_map<int, string>& id_a_nombre) {
    cout << "-> Total de vertices en el grafo: " << lista_adyacencia.size() << "\n";
    cout << "-> Muestra de los primeros 5 nodos y su grado de salida:\n";
    
    int contador = 0;
    for (auto const& par : lista_adyacencia) {
        if (contador >= 5) break;
        
        int id_nodo = par.first;
        cout << "   - ID [" << id_nodo << "] (" << id_a_nombre[id_nodo] << ") "
             << "tiene " << par.second.size() << " conexiones de salida.\n";
        contador++;
    }
    cout << "   (...)\n";
}

vector<int> Grafo::get_nodos() const {
    vector<int> nodos;
    nodos.reserve(lista_adyacencia.size());
    for (auto const& par : lista_adyacencia) {
        nodos.push_back(par.first);
    }
    return nodos;
}

int Grafo::get_grado_salida(int id) const {
    return (int)lista_adyacencia.at(id).size();
}

int Grafo::get_grado_entrada(int id) const {
    return grado_entrada.count(id) ? grado_entrada.at(id) : 0;
}

int Grafo::get_num_vertices() const {
    return (int)lista_adyacencia.size();
}