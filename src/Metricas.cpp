#include "Metricas.h"
#include "Algoritmos.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <unordered_set>
#include <vector>
#include <limits>

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

unordered_map<int, double> Metricas::degree_centrality(const Grafo& grafo) {
    const auto& ady = grafo.get_lista_adyacencia();
    vector<int> nodos = grafo.get_nodos();

    unordered_map<int, double> centralidad;
    centralidad.reserve(nodos.size());

    if (nodos.empty()) {
        return centralidad;
    }

    unordered_map<int, unordered_set<int>> vecinos_incidentes;
    vecinos_incidentes.reserve(nodos.size());
    for (int id : nodos) {
        vecinos_incidentes[id] = unordered_set<int>();
    }

    for (const auto& par : ady) {
        int origen = par.first;
        for (const Arista& arista : par.second) {
            vecinos_incidentes[origen].insert(arista.destino);
            vecinos_incidentes[arista.destino].insert(origen);
        }
    }

    const double normalizador = nodos.size() > 1
        ? static_cast<double>(nodos.size() - 1)
        : 1.0;

    for (int id : nodos) {
        centralidad[id] = static_cast<double>(vecinos_incidentes[id].size()) / normalizador;
    }

    cout << "-> degree centrality lista para " << centralidad.size() << " nodos.\n";
    return centralidad;
}

unordered_map<int, double> Metricas::pagerank(const Grafo& grafo, double damping, int max_iter, double tolerancia, int k_nodos) {
    const auto& ady = grafo.get_lista_adyacencia();
    vector<int> nodos = grafo.get_nodos();

    unordered_map<int, double> rank_final;
    rank_final.reserve(nodos.size());

    if (nodos.empty()) {
        return rank_final;
    }

    // si k_nodos es valido usa solo los primeros k nodos
    int total_nodos = static_cast<int>(nodos.size());
    int usados = total_nodos;
    if (k_nodos > 0 && k_nodos < total_nodos) {
        usados = k_nodos;
        nodos.resize(usados);
    }

    const int n = static_cast<int>(nodos.size());
    const double inv_n = 1.0 / static_cast<double>(n);

    unordered_map<int, int> idx;
    idx.reserve(nodos.size());
    for (int i = 0; i < n; ++i) {
        idx[nodos[i]] = i;
    }

    vector<vector<pair<int, double>>> entrantes(n);
    vector<double> suma_pesos_salientes(n, 0.0);

    // guarda las aristas entrantes y la suma de pesos salientes
    for (const auto& par : ady) {
        unordered_map<int, int>::const_iterator it_origen = idx.find(par.first);
        if (it_origen == idx.end()) {
            continue;
        }

        int origen_idx = it_origen->second;
        for (const Arista& arista : par.second) {
            unordered_map<int, int>::const_iterator it_destino = idx.find(arista.destino);
            if (it_destino == idx.end()) {
                continue;
            }

            int destino_idx = it_destino->second;
            double peso = arista.peso > 0.0 ? arista.peso : 1.0;
            entrantes[destino_idx].push_back(make_pair(origen_idx, peso));
            suma_pesos_salientes[origen_idx] += peso;
        }
    }

    vector<double> rank_actual(n, inv_n);
    vector<double> rank_nuevo(n, inv_n);

    // repite el calculo hasta converger o llegar al limite de iteraciones
    for (int iter = 0; iter < max_iter; ++iter) {
        double masa_colgante = 0.0;
        for (int i = 0; i < n; ++i) {
            if (suma_pesos_salientes[i] <= 0.0) {
                masa_colgante += rank_actual[i];
            }
        }

        double base = (1.0 - damping) * inv_n;
        double redistribucion_colgante = damping * masa_colgante * inv_n;
        double diferencia_total = 0.0;

        for (int destino_idx = 0; destino_idx < n; ++destino_idx) {
            double contribucion = 0.0;
            for (const pair<int, double>& fuente : entrantes[destino_idx]) {
                int origen_idx = fuente.first;
                double peso = fuente.second;
                if (suma_pesos_salientes[origen_idx] > 0.0) {
                    contribucion += rank_actual[origen_idx] * (peso / suma_pesos_salientes[origen_idx]);
                }
            }

            rank_nuevo[destino_idx] = base + redistribucion_colgante + damping * contribucion;
            diferencia_total += abs(rank_nuevo[destino_idx] - rank_actual[destino_idx]);
        }

        rank_actual.swap(rank_nuevo);
        if (diferencia_total < tolerancia) {
            break;
        }
    }

    for (int i = 0; i < n; ++i) {
        rank_final[nodos[i]] = rank_actual[i];
    }

    cout << "-> pagerank listo para " << rank_final.size()
         << " nodos, usando " << usados
         << (usados == total_nodos ? " nodos del grafo (exacto).\n" : " nodos del grafo (aproximado).\n");
    return rank_final;
}

void Metricas::print_degree(const Grafo& grafo, const unordered_map<int, string>& id_a_nombre, int k) {
    unordered_map<int, double> dc = degree_centrality(grafo);
    vector<pair<int, double>> pares;
    pares.reserve(dc.size());

    for (const auto& par : dc) {
        pares.push_back(par);
    }

    sort(pares.begin(), pares.end(), [](const pair<int, double>& a, const pair<int, double>& b) {
        return a.second > b.second;
    });

    int limite = min(k, static_cast<int>(pares.size()));
    cout << "-> top " << limite << " por degree centrality:\n";

    for (int i = 0; i < limite; ++i) {
        int id = pares[i].first;
        double valor = pares[i].second;
        unordered_map<int, string>::const_iterator it = id_a_nombre.find(id);
        string nombre = (it != id_a_nombre.end()) ? it->second : "sin_nombre";

        cout << "   " << (i + 1) << ". ID [" << id << "] (" << nombre << ") -> " << valor << "\n";
    }
}

void Metricas::print_pagerank(const Grafo& grafo, const unordered_map<int, string>& id_a_nombre, int k, double damping, int max_iter, double tolerancia, int k_nodos) {
    unordered_map<int, double> pr = pagerank(grafo, damping, max_iter, tolerancia, k_nodos);
    vector<pair<int, double>> pares;
    pares.reserve(pr.size());

    for (const auto& par : pr) {
        pares.push_back(par);
    }

    sort(pares.begin(), pares.end(), [](const pair<int, double>& a, const pair<int, double>& b) {
        return a.second > b.second;
    });

    vector<int> nodos = grafo.get_nodos();
    int total_nodos = static_cast<int>(nodos.size());
    int usados = total_nodos;
    if (k_nodos > 0 && k_nodos < total_nodos) {
        usados = k_nodos;
    }

    int limite = min(k, static_cast<int>(pares.size()));
    cout << "-> top " << limite << " por pagerank"
         << " (usando " << usados << " nodos"
         << (usados == total_nodos ? ", exacto):\n" : ", aproximado):\n");

    for (int i = 0; i < limite; ++i) {
        int id = pares[i].first;
        double valor = pares[i].second;
        unordered_map<int, string>::const_iterator it = id_a_nombre.find(id);
        string nombre = (it != id_a_nombre.end()) ? it->second : "sin_nombre";

        cout << "   " << (i + 1) << ". ID [" << id << "] (" << nombre << ") -> " << valor << "\n";
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

double Metricas::excentricidad(const Grafo& grafo, int origen, bool es_ponderado) {
    const auto& ady = grafo.get_lista_adyacencia();
    vector<int> nodos = grafo.get_nodos();

    //Crear el mapa de índices para los algoritmos
    unordered_map<int, int> idx;
    for (int i = 0; i < static_cast<int>(nodos.size()); ++i) {
        idx[nodos[i]] = i;
    }

    //Si el nodo origen no existe en el grafo, devolvemos 0
    if (idx.find(origen) == idx.end()) {
        return 0.0; 
    }

    int s_idx = idx[origen];
    BrandesBusqueda busqueda;

    if (es_ponderado) {
        busqueda = Algoritmos::brandes_dijkstra(ady, nodos, idx, s_idx);
    } else {
        busqueda = Algoritmos::brandes_bfs(ady, nodos, idx, s_idx);
    }

    //Encontrar la máxima distancia (ignorando los nodos inalcanzables)
    double max_distancia = 0.0;
    const double inf = numeric_limits<double>::infinity();

    //Se itera sobre las distancias recién agregadas al struct
    for (double d : busqueda.dist) {
        if (d != inf) {
            max_distancia = max(max_distancia, d);
        }
    }

    return max_distancia;
}

double Metricas::radiality_centrality(const Grafo& grafo, int origen, bool es_ponderado, double diametro_grafo) {
    const auto& ady = grafo.get_lista_adyacencia();
    vector<int> nodos = grafo.get_nodos();

    if (nodos.size() <= 1) return 0.0;

    //Crear mapa de índices
    unordered_map<int, int> idx;
    for (int i = 0; i < static_cast<int>(nodos.size()); ++i) {
        idx[nodos[i]] = i;
    }

    if (idx.find(origen) == idx.end()) {
        return 0.0; 
    }

    int s_idx = idx[origen];
    BrandesBusqueda busqueda;

    //Delegar la búsqueda de caminos mínimos
    if (es_ponderado) {
        busqueda = Algoritmos::brandes_dijkstra(ady, nodos, idx, s_idx);
    } else {
        busqueda = Algoritmos::brandes_bfs(ady, nodos, idx, s_idx);
    }

    //Se aplica la formula: Sum( (diametro + 1 - distancia) ) / (V - 1)
    double suma_radialidad = 0.0;
    int nodos_alcanzables = 0; //Esto representara el (V - 1)
    const double inf = numeric_limits<double>::infinity();

    for (double d : busqueda.dist) {
        //Se ignoran los nodos inalcanzables y la distancia hacia sí mismo (que es 0)
        if (d != inf && d > 0.0) {
            suma_radialidad += (diametro_grafo + 1.0 - d);
            nodos_alcanzables++;
        }
    }

    if (nodos_alcanzables == 0) {
        return 0.0;
    }

    //Se divide por la cantidad de nodos de su componente conexo
    return suma_radialidad / static_cast<double>(nodos_alcanzables);
}

double Metricas::average_shortest_path(const Grafo& grafo, int k_origenes) {
    const auto& ady = grafo.get_lista_adyacencia();
    vector<int> nodos = grafo.get_nodos();

    // si es un arbol unitario no hay caminos :O
    if (nodos.size() < 2) {
        return 0.0;
    }

    // mapa de id real a indice 
    unordered_map<int, int> idx;
    idx.reserve(nodos.size());
    for (int i = 0; i < static_cast<int>(nodos.size()); ++i) {
        idx[nodos[i]] = i;
    }

    // usa bfs si todos los pesos son unitarios y dijkstra si no
    bool tiene_pesos = valid_tiene_peso(ady);

    // si k_origenes es valido lo usa
    int total_origenes = static_cast<int>(nodos.size());
    int usados = total_origenes;
    if (k_origenes > 0 && k_origenes < total_origenes) {
        usados = k_origenes;
    }

    const double inf = numeric_limits<double>::infinity();
    double suma_distancias = 0.0;
    long long cantidad_pares = 0;

    // recorre cada origen y acumula las distancias a los nodos alcanzables
    for (int origen = 0; origen < usados; ++origen) {
        int s_idx = idx[nodos[origen]];
        BrandesBusqueda busqueda = tiene_pesos
            ? Algoritmos::brandes_dijkstra(ady, nodos, idx, s_idx)
            : Algoritmos::brandes_bfs(ady, nodos, idx, s_idx);

        // se excluye la distancia hacia si mismo y los nodos inalcanzablrs
        for (int w_idx = 0; w_idx < static_cast<int>(busqueda.dist.size()); ++w_idx) {
            if (w_idx == s_idx) {
                continue;
            }
            double d = busqueda.dist[w_idx];
            if (d != inf) {
                suma_distancias += d;
                ++cantidad_pares;
            }
        }
    }

    if (cantidad_pares == 0) {
        return 0.0;
    }

    // promedio final de distancias minimas
    return suma_distancias / static_cast<double>(cantidad_pares);
}

void Metricas::print_average_shortest_path(const Grafo& grafo, int k_origenes) {
    vector<int> nodos = grafo.get_nodos();
    int total_origenes = static_cast<int>(nodos.size());
    int usados = total_origenes;
    if (k_origenes > 0 && k_origenes < total_origenes) {
        usados = k_origenes;
    }

    // imprime el valor
    double asp = average_shortest_path(grafo, k_origenes);
    cout << "-> average shortest path: " << asp
         << " (usando " << usados << " origenes"
         << (usados == total_origenes ? ", exacto).\n" : ", aproximado).\n");
}

unordered_map<int, double> Metricas::closeness_centrality(const Grafo& grafo, int k_origenes) {
    const auto& ady = grafo.get_lista_adyacencia();
    vector<int> nodos = grafo.get_nodos();

    // inicializa el closeness los nodos en 0
    unordered_map<int, double> closeness;
    closeness.reserve(nodos.size());
    for (int id : nodos) {
        closeness[id] = 0.0;
    }

    // si no hay nodos o solo hay uno no se puede calcular el closeness
    if (nodos.size() <= 1) {
        return closeness;
    }

    // convierte el id real del nodo a un indice 
    unordered_map<int, int> idx;
    idx.reserve(nodos.size());
    for (int i = 0; i < static_cast<int>(nodos.size()); ++i) {
        idx[nodos[i]] = i;
    }

    // bfs o dijkstra segun pesos del grafo
    bool tiene_pesos = valid_tiene_peso(ady);
    const double inf = numeric_limits<double>::infinity();
    const double eps = 1e-12;
    const double n_menos_uno = static_cast<double>(nodos.size() - 1);

    // define cuantos nodos se usan como origen para el calculo
    int total_origenes = static_cast<int>(nodos.size());
    int usados = total_origenes;
    if (k_origenes > 0 && k_origenes < total_origenes) {
        usados = k_origenes;
    }

    // calcula las distancias minimas desde cada origen
    for (int s_idx = 0; s_idx < usados; ++s_idx) {
        BrandesBusqueda busqueda = tiene_pesos
            ? Algoritmos::brandes_dijkstra(ady, nodos, idx, s_idx)
            : Algoritmos::brandes_bfs(ady, nodos, idx, s_idx);

        double suma_distancias = 0.0;
        int alcanzables = 0;

        // acumula solo nodos alcanzables, menos el propio origen
        for (int w_idx = 0; w_idx < static_cast<int>(busqueda.dist.size()); ++w_idx) {
            if (w_idx == s_idx) {
                continue;
            }
            double d = busqueda.dist[w_idx];
            if (d != inf) {
                suma_distancias += d;
                ++alcanzables;
            }
        }

        int id_origen = nodos[s_idx];
        if (alcanzables > 0 && suma_distancias > eps) {
            // normaliza por si los grafos estan desconectados
            double fraccion_alcanzable = static_cast<double>(alcanzables) / n_menos_uno;
            closeness[id_origen] = fraccion_alcanzable * (static_cast<double>(alcanzables) / suma_distancias);
        }
    }

    return closeness;
}

void Metricas::print_closeness(const Grafo& grafo, const unordered_map<int, string>& id_a_nombre, int k, int k_origenes) {
    // calcula closeness
    unordered_map<int, double> cc = closeness_centrality(grafo, k_origenes);
    // prepara vector para ordenar
    vector<pair<int, double>> pares;
    pares.reserve(cc.size());

    vector<int> nodos = grafo.get_nodos();
    int total_origenes = static_cast<int>(nodos.size());
    int usados = total_origenes;
    if (k_origenes > 0 && k_origenes < total_origenes) {
        usados = k_origenes;
    }

    for (unordered_map<int, double>::const_iterator it = cc.begin(); it != cc.end(); ++it) {
        pares.push_back(*it);
    }

    sort(pares.begin(), pares.end(), [](const pair<int, double>& a, const pair<int, double>& b) {
        return a.second > b.second;
    });

        int limite = min(k, static_cast<int>(pares.size()));
        cout << "-> top " << limite << " por closeness"
            << " (usando " << usados << " origenes"
            << (usados == total_origenes ? ", exacto):\n" : ", aproximado):\n");

    // imprime los k nodos con mayor closeness
    for (int i = 0; i < limite; ++i) {
        int id = pares[i].first;
        double valor = pares[i].second;
        unordered_map<int, string>::const_iterator it = id_a_nombre.find(id);
        string nombre = (it != id_a_nombre.end()) ? it->second : "sin_nombre";

        cout << "   " << (i + 1) << ". ID [" << id << "] (" << nombre << ") -> " << valor << "\n";
    }
}
