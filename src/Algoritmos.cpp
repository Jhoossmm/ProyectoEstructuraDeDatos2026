#include "Algoritmos.h"

#include <cmath>
#include <functional>
#include <limits>
#include <queue>

BrandesBusqueda Algoritmos::brandes_bfs(
    const unordered_map<int, vector<Arista>>& ady,
    const vector<int>& nodos,
    const unordered_map<int, int>& idx,
    int s_idx
) {
    const int n = static_cast<int>(nodos.size());
    const double inf = numeric_limits<double>::infinity();
    const double eps = 1e-12;

    BrandesBusqueda res;
    res.pred.assign(n, vector<int>());
    res.sigma.assign(n, 0.0);
    res.orden.reserve(n);

    vector<double> dist(n, inf);
    dist[s_idx] = 0.0;
    res.sigma[s_idx] = 1.0;

    queue<int> q;
    q.push(s_idx);

    while (!q.empty()) {
        int v_idx = q.front();
        q.pop();
        res.orden.push_back(v_idx);

        int v_id = nodos[v_idx];
        auto it = ady.find(v_id);
        if (it == ady.end()) {
            continue;
        }

        for (const Arista& a : it->second) {
            auto it_w = idx.find(a.destino);
            if (it_w == idx.end()) {
                continue;
            }
            int w_idx = it_w->second;

            if (dist[w_idx] == inf) {
                dist[w_idx] = dist[v_idx] + 1.0;
                q.push(w_idx);
            }

            if (abs(dist[w_idx] - (dist[v_idx] + 1.0)) <= eps) {
                res.sigma[w_idx] += res.sigma[v_idx];
                res.pred[w_idx].push_back(v_idx);
            }
        }
    }

    return res;
}

BrandesBusqueda Algoritmos::brandes_dijkstra(
    const unordered_map<int, vector<Arista>>& ady,
    const vector<int>& nodos,
    const unordered_map<int, int>& idx,
    int s_idx
) {
    const int n = static_cast<int>(nodos.size());
    const double inf = numeric_limits<double>::infinity();
    const double eps = 1e-12;

    BrandesBusqueda res;
    res.pred.assign(n, vector<int>());
    res.sigma.assign(n, 0.0);
    res.orden.reserve(n);

    vector<double> dist(n, inf);
    dist[s_idx] = 0.0;
    res.sigma[s_idx] = 1.0;

    using Estado = pair<double, int>;
    priority_queue<Estado, vector<Estado>, greater<Estado>> pq;
    vector<bool> cerrado(n, false);
    pq.push({0.0, s_idx});

    while (!pq.empty()) {
        double d = pq.top().first;
        int v_idx = pq.top().second;
        pq.pop();

        if (cerrado[v_idx] || d > dist[v_idx] + eps) {
            continue;
        }
        cerrado[v_idx] = true;
        res.orden.push_back(v_idx);

        int v_id = nodos[v_idx];
        auto it = ady.find(v_id);
        if (it == ady.end()) {
            continue;
        }

        for (const Arista& a : it->second) {
            auto it_w = idx.find(a.destino);
            if (it_w == idx.end()) {
                continue;
            }
            int w_idx = it_w->second;
            double nuevo = dist[v_idx] + a.peso;

            if (nuevo + eps < dist[w_idx]) {
                dist[w_idx] = nuevo;
                res.sigma[w_idx] = res.sigma[v_idx];
                res.pred[w_idx].clear();
                res.pred[w_idx].push_back(v_idx);
                pq.push({dist[w_idx], w_idx});
            } else if (abs(nuevo - dist[w_idx]) <= eps) {
                res.sigma[w_idx] += res.sigma[v_idx];
                res.pred[w_idx].push_back(v_idx);
            }
        }
    }

    return res;
}