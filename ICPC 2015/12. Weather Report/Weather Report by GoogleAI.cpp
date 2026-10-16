/*

Solucion al Problema L Weather Report by: GoogleAI

Paradigma: Algoritmo de Huffman

Comentarios by: Calak de Astora

Input:

2
0.9 0.049999 0.05 0.000001

20
0.25 0.25 0.25 0.25

*/

#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>

using namespace std;

// Estructura para almacenar los grupos de nodos en la cola de prioridad
struct Node {
    long double prob;
    long long count;

    // El nodo con MENOR probabilidad debe ir arriba en la cola de prioridad
    bool operator>(const Node& other) const {
        return prob > other.prob;
    }
};

// Función para calcular los factoriales necesarios para el coeficiente multinomial
vector<long long> get_factorials(int n) {
    vector<long long> fact(n + 1, 1);
    for (int i = 2; i <= n; ++i) {
        fact[i] = fact[i - 1] * i;
    }
    return fact;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    vector<long double> p(4);
    for (int i = 0; i < 4; ++i) {
        cin >> p[i];
    }

    auto fact = get_factorials(n);
    priority_queue<Node, vector<Node>, greater<Node>> pq;

    // Generar todas las combinaciones de frecuencias (fS, fC, fR, fF) tales que sumen n
    for (int fS = 0; fS <= n; ++fS) {
        for (int fC = 0; fC <= n - fS; ++fC) {
            for (int fR = 0; fR <= n - fS - fC; ++fR) {
                int fF = n - fS - fC - fR;

                // Calcular la probabilidad individual de este tipo de cadena
                long double current_prob = 1.0;
                for (int i = 0; i < fS; ++i) current_prob *= p[0];
                for (int i = 0; i < fC; ++i) current_prob *= p[1];
                for (int i = 0; i < fR; ++i) current_prob *= p[2];
                for (int i = 0; i < fF; ++i) current_prob *= p[3];

                // Calcular cuántas cadenas comparten esta misma distribución (Coeficiente Multinomial)
                long long ways = fact[n] / (fact[fS] * fact[fC] * fact[fR] * fact[fF]);

                if (ways > 0) {
                    pq.push({current_prob, ways});
                }
            }
        }
    }

    long double total_expected_length = 0.0;

    // Simulación del algoritmo de Huffman con Run-Length Compression
    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();

        // Si es el último grupo de nodos y solo queda 1, terminamos
        if (pq.empty() && current.count == 1) {
            break;
        }

        // 1. Procesar parejas internas del mismo grupo (si hay más de 1 nodo con esta probabilidad)
        if (current.count > 1) {
            long long pairs = current.count / 2;
            long double new_prob = current.prob * 2.0;
            
            // Cada combinación suma el costo de sus dos ramas al total esperado
            total_expected_length += new_prob * pairs;
            pq.push({new_prob, pairs});
            
            current.count %= 2; // Nos quedamos con el residuo (0 o 1)
        }

        // 2. Si quedó un nodo huérfano (impar), se tiene que fusionar con el siguiente más pequeño
        if (current.count == 1) {
            if (pq.empty()) break; // No hay con quién fusionarlo

            Node next_node = pq.top();
            pq.pop();

            // Combinar el nodo actual con 1 solo nodo del siguiente grupo
            long double combined_prob = current.prob + next_node.prob;
            total_expected_length += combined_prob;
            pq.push({combined_prob, 1});

            // Devolver los nodos restantes del siguiente grupo a la cola
            if (next_node.count > 1) {
                pq.push({next_node.prob, next_node.count - 1});
            }
        }
    }

    // Imprimir el resultado con la precisión requerida por el ICPC (6 decimales recomendados)
    cout << fixed << setprecision(6) << total_expected_length << "\n";

    return 0;
}
