#include "Metricas.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <climits>
#include <chrono>
#include <cmath>

vector<MetricasGrado> calcular_degree_centrality(const Grafo& grafo) {
    vector<int> nodos = grafo.get_nodos();
    int N = grafo.get_num_vertices();
    double denom = (N > 1) ? (double)(N - 1) : 1.0;

    vector<MetricasGrado> resultados;
    resultados.reserve(N);

    for (int id : nodos) {
        MetricasGrado m;
        m.id             = id;
        m.grado_salida   = grafo.get_grado_salida(id);
        m.grado_entrada  = grafo.get_grado_entrada(id);
        m.grado_total    = m.grado_entrada + m.grado_salida;

        // Normaliza entre 0 y 1
        m.centralidad_entrada = m.grado_entrada / denom;
        m.centralidad_salida  = m.grado_salida  / denom;
        m.centralidad_total   = m.grado_total   / (2.0 * denom);

        resultados.push_back(m);
    }

    // Ordena de mayor a menor grado total
    sort(resultados.begin(), resultados.end(), [](const MetricasGrado& a, const MetricasGrado& b) {
        return a.grado_total > b.grado_total;
    });

    return resultados;
}


void imprimir_degree_centrality(
    const vector<MetricasGrado>& metricas,
    unordered_map<int, string>& id_a_nombre,
    int top_n
) {
    int N     = (int)metricas.size();
    int limite = min(top_n, N);

    // Estadísticas generales
    double suma = 0;
    int max_tot = 0, min_tot = INT_MAX;
    for (const auto& m : metricas) {
        suma    += m.grado_total;
        max_tot  = max(max_tot, m.grado_total);
        min_tot  = min(min_tot, m.grado_total);
    }

    cout << "\n Estadisticas generales Degree Centrality\n";
    cout << "Nodos totales   : " << N       << "\n";
    cout << "Grado máximo    : " << max_tot << "\n";
    cout << "Grado mínimo    : " << min_tot << "\n";
    cout << fixed << setprecision(6);
    cout << "Grado promedio  : " << suma / N << "\n";

    // Tabla Top-N
    cout << "\n Top " << limite << " nodos por Degree Centrality \n";
    cout << left
         << setw(15)  << "Rank"
         << setw(15) << "ID"
         << setw(15) << "Nombre"
         << setw(15)  << "Total"
         << setw(15)  << "In"
         << setw(15)  << "Out"
         << setw(15) << "Centralidad"
         << "\n";
    cout << string(82, '-') << "\n";

    for (int i = 0; i < limite; ++i) {
        const auto& m = metricas[i];
        string nombre = id_a_nombre.count(m.id) ? id_a_nombre.at(m.id) : "?";
        if (nombre.size() > 20) nombre = nombre.substr(0, 17) + "...";

        cout << left
             << setw(15)  << (i + 1)
             << setw(15) << m.id
             << setw(15) << nombre
             << setw(15)  << m.grado_total
             << setw(15)  << m.grado_entrada
             << setw(15)  << m.grado_salida
             << fixed << setprecision(6)
             << setw(15) << m.centralidad_total
             << "\n";
    }
}

void medir_tiempo_degree_centrality(const Grafo& grafo, int repeticiones) {
    vector<double> tiempos;

    for (int i = 0; i < repeticiones; i++) {
        auto inicio = chrono::high_resolution_clock::now();
        calcular_degree_centrality(grafo);
        auto fin = chrono::high_resolution_clock::now();

        double ms = chrono::duration<double, milli>(fin - inicio).count();
        tiempos.push_back(ms);
    }

    // Promedio
    double suma = 0;
    for (double t : tiempos) suma += t;
    double promedio = suma / repeticiones;

    // Varianza
    double varianza = 0;
    for (double t : tiempos) varianza += pow(t - promedio, 2);
    varianza /= repeticiones;

    cout << "\n--- Medicion de Tiempo (" << repeticiones << " repeticiones) ---\n";
    cout << fixed << setprecision(4);
    cout << "  Promedio : " << promedio << " ms\n";
    cout << "  Varianza : " << varianza << " ms^2\n";
    cout << "  Desv. Estandar: " << sqrt(varianza) << " ms\n";
}