#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include <unordered_map>
#include <vector>

#include "Grafo.h"

using namespace std;

struct BrandesBusqueda {
    vector<vector<int>> pred;
    vector<double> sigma;
    vector<int> orden;
};

class Algoritmos {
public:
    // caminos minimos para brandes en no ponderado
    static BrandesBusqueda brandes_bfs(
        const unordered_map<int, vector<Arista>>& ady,
        const vector<int>& nodos,
        const unordered_map<int, int>& idx,
        int s_idx
    );

    // caminos minimos para brandes en ponderado
    static BrandesBusqueda brandes_dijkstra(
        const unordered_map<int, vector<Arista>>& ady,
        const vector<int>& nodos,
        const unordered_map<int, int>& idx,
        int s_idx
    );
};

#endif