/*

Solucion al Problema C Catering by: GoogleAI

Paradigma: Flujo Máximo de Costo Mínimo (MCMF)

Comentarios by: Calak de Astora

Input:

3 2
40 30 40
50 10
50

3 2
10 10 10
20 21
21

*/

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

const long long INF = 1e18;

struct Edge {
    int to;
    int capacity;
    int flow;
    long long cost;
    int rev; // Índice de la arista inversa en el vector del nodo 'to'
};

class MinCostMaxFlow {
    int n;
    vector<vector<Edge>> adj;
    vector<long long> potential;
    vector<long long> dist;
    vector<int> parent_node;
    vector<int> parent_edge;

public:
    MinCostMaxFlow(int n) : n(n), adj(n), potential(n, 0), dist(n), parent_node(n), parent_edge(n) {}

    void add_edge(int from, int to, int capacity, long long cost) {
        adj[from].push_back({to, capacity, 0, cost, (int)adj[to].size()});
        adj[to].push_back({from, 0, 0, -cost, (int)adj[from].size() - 1});
    }

    bool dijkstra(int source, int sink) {
        fill(dist.begin(), dist.end(), INF);
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;

        dist[source] = 0;
        pq.push({0, source});

        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();

            if (d > dist[u]) continue;

            for (int i = 0; i < adj[u].size(); ++i) {
                Edge &e = adj[u][i];
                if (e.capacity - e.flow > 0) {
                    // Costo reducido usando los potenciales para garantizar valores >= 0
                    long long reduced_cost = e.cost + potential[u] - potential[e.to];
                    if (dist[e.to] > dist[u] + reduced_cost) {
                        dist[e.to] = dist[u] + reduced_cost;
                        parent_node[e.to] = u;
                        parent_edge[e.to] = i;
                        pq.push({dist[e.to], e.to});
                    }
                }
            }
        }
        return dist[sink] != INF;
    }

    pair<int, long long> solve(int source, int sink) {
        int max_flow = 0;
        long long min_cost = 0;

        // Inicializamos los potenciales (opcional si ya sabemos que los costos base son >= 0)
        fill(potential.begin(), potential.end(), 0);

        while (dijkstra(source, sink)) {
            // Actualizamos los potenciales para la siguiente iteración
            for (int i = 0; i < n; ++i) {
                if (dist[i] != INF) {
                    potential[i] += dist[i];
                }
            }

            // Encontrando el flujo cuello de botella en el camino
            int path_flow = 1e9; // Para este problema usualmente empujamos de 1 en 1
            for (int v = sink; v != source; v = parent_node[v]) {
                int u = parent_node[v];
                int idx = parent_edge[v];
                path_flow = min(path_flow, adj[u][idx].capacity - adj[u][idx].flow);
            }

            // Aplicando el flujo y acumulando el costo real
            for (int v = sink; v != source; v = parent_node[v]) {
                int u = parent_node[v];
                int idx = parent_edge[v];
                adj[u][idx].flow += path_flow;
                adj[v][adj[u][idx].rev].flow -= path_flow;
                min_cost += (long long)path_flow * adj[u][idx].cost;
            }

            max_flow += path_flow;
        }

        return {max_flow, min_cost};
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, k;
    if (!(cin >> n >> k)) return 0;

    // Mapeo de Nodos:
    // S (Fuente)      = 0
    // Sede (Salida)   = 1
    // In_i (Clientes) = 2 a N + 1
    // Out_i (Clientes)= N + 2 a 2N + 1
    // T (Sumidero)    = 2N + 2
    int source = 0;
    int sede_out = 1;
    int total_nodes = 2 * n + 3;
    int sink = total_nodes - 1;

    MinCostMaxFlow mcmf(total_nodes);

    // Limitamos la salida de la sede central a máximo K equipos disponibles
    mcmf.add_edge(source, sede_out, k, 0);

    // Lectura de los costos desde la sede hacia cada cliente i
    for (int i = 1; i <= n; ++i) {
        long long cost;
        cin >> cost;
        // La sede puede enviar un equipo al nodo de entrada del cliente i
        mcmf.add_edge(sede_out, 1 + i, 1, cost);
    }

    // Lectura de la matriz de transiciones entre clientes
    for (int i = 1; i < n; ++i) {
        for (int j = i + 1; j <= n; ++j) {
            long long cost;
            cin >> cost;
            // Del nodo de salida del cliente i al nodo de entrada del cliente j
            mcmf.add_edge(n + 1 + i, 1 + j, 1, cost);
        }
    }

    // Conexiones estructurales para garantizar el flujo correcto
    for (int i = 1; i <= n; ++i) {
        int in_node = 1 + i;
        int out_node = n + 1 + i;

        // Cada cliente i debe ser servido exactamente una vez
        mcmf.add_edge(in_node, sink, 1, 0);

        // Opción de "reiniciar" o despachar un camino que termina (aristas de demanda/cobertura)
        mcmf.add_edge(source, out_node, 1, 0);
    }

    // Resolvemos el grafo
    auto [flow, total_cost] = mcmf.solve(source, sink);

    cout << total_cost << "\n";

    return 0;
}
