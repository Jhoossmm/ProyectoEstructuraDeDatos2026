#include "Metricas.h"
#include "Algoritmos.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

namespace {
// en caso de que el grafo de pesos tenga pesos unitarios para que use bfs
bool valid_tiene_peso(const unordered_map<int, vector<Arista>>& ady) {
    const double eps = 1e-12;
    for (const auto& par : ady) {
        for (const Arista& a : par.second) {
            if (abs(a.peso - 1.0) > eps) {
                return true;
            }
        }
    }
    return false;
}
}

unordered_map<int, double> Metricas::betweenness_centrality(const Grafo& grafo, int k_origenes) {
    const auto& ady = grafo.get_lista_adyacencia();
    vector<int> nodos = grafo.get_nodos();

    // inicializa la centralidad de cada nodo en 0
    unordered_map<int, double> centralidad;
    centralidad.reserve(nodos.size());
    for (int n : nodos) {
        centralidad[n] = 0.0;
    }

    // mapa para convertir el id del nodo a un indice compacto
    unordered_map<int, int> idx;
    idx.reserve(nodos.size());
    for (int i = 0; i < static_cast<int>(nodos.size()); ++i) {
        idx[nodos[i]] = i;
    }

    
    // si hay pesos no unitarios se usa dijkstra, si no se usa bfs
    bool tiene_pesos = valid_tiene_peso(ady);

    // define cuantos nodos se usaran como origen para el calculo
    int total_origenes = static_cast<int>(nodos.size());
    int usados = total_origenes;
    if (k_origenes > 0 && k_origenes < total_origenes) {
        usados = k_origenes;
    }

    // recorre cada origen y acumula la centralidad
    for (int origen = 0; origen < usados; ++origen) {
        int n = static_cast<int>(nodos.size());
        int s_idx = idx[nodos[origen]];

        // bfs o dijkstra segun pesos
        BrandesBusqueda busqueda = tiene_pesos
            ? Algoritmos::brandes_dijkstra(ady, nodos, idx, s_idx)
            : Algoritmos::brandes_bfs(ady, nodos, idx, s_idx);

        // delta guarda la dependencia acumulada de cada nodo
        vector<double> delta(n, 0.0);

        // acumula dependencia en orden inverso desde la pila de nodos que ya visitados
        for (int i = static_cast<int>(busqueda.orden.size()) - 1; i >= 0; --i) {
            int w_idx = busqueda.orden[i];

            for (int v_idx : busqueda.pred[w_idx]) {
                if (busqueda.sigma[w_idx] > 0.0) {
                    // formula de brandes para propagar dependencia la dependencia  
                    delta[v_idx] += (busqueda.sigma[v_idx] / busqueda.sigma[w_idx]) * (1.0 + delta[w_idx]);
                }
            }

            if (w_idx != s_idx) {
                int w_id = nodos[w_idx];
                
                // el origen no se suma a si mismo
                centralidad[w_id] += delta[w_idx];
            }
        }

    }

    // escala si se uso una muestra de origenes
    if (usados < total_origenes && usados > 0) {
        double factor = static_cast<double>(total_origenes) / static_cast<double>(usados);
        for (auto& p : centralidad) {
            // se ajusta el valor para aproximar el resultado
            p.second *= factor;
        }
    }

    cout << "-> betweenness centrality lista para " << centralidad.size()
         << " nodos, usando " << usados << " origenes"
         << (usados == total_origenes ? " (exacto).\n" : " (aproximado).\n");
    return centralidad;
}

void Metricas::print_betweenness(const Grafo& grafo, const unordered_map<int, string>& id_a_nombre, int k, int k_origenes) {
    // calcula centralidad y ordena de mayor a menor
    unordered_map<int, double> bc = betweenness_centrality(grafo, k_origenes);
    vector<pair<int, double>> pares;
    pares.reserve(bc.size());

    // copia el mapa a un vector para poder ordenarlo por valor
    for (const auto& p : bc) {
        pares.push_back(p);
    }

    sort(pares.begin(), pares.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    int limite = min(k, static_cast<int>(pares.size()));
    cout << "-> top " << limite << " por betweenness:\n";

    // imprime los k nodos con mayor centralidad
    for (int i = 0; i < limite; ++i) {
        int id = pares[i].first;
        double valor = pares[i].second;
        auto it = id_a_nombre.find(id);
        string nombre = (it != id_a_nombre.end()) ? it->second : "sin_nombre";

        cout << "   " << (i + 1) << ". ID [" << id << "] (" << nombre << ") -> " << valor << "\n";
    }
}