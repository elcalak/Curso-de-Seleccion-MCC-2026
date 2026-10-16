/*

Solucion al Problema G Pipe Stream by: GoogleAI

Paradigma: 

Comentarios by: Calak de Astora

Input:

3
1000 1 30 1 1
60 2 10 2 5
59 2 10 2 5

*/

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

// Función de validación: Comprueba si K golpes son suficientes
bool es_posible(int K, double L, double v1, double v2, double t, double s) {
    // Si la tubería infinita ya requeriría más de K golpes, es imposible.
    if ((v2 - v1) <= t) return true;
    if (K == 0) return false;

    // dp[i] guardará la velocidad MÁXIMA que podemos resolver partiendo de v1
    // si nos quedan 'i' golpes por realizar en el futuro.
    vector<double> dp(K + 1, v1);

    // Caso base: Con 0 golpes restantes, el intervalo final mide t.
    // Partiendo de v1, lo máximo que podemos cubrir es v1 + t.
    dp[0] = min(v2, v1 + t);

    // Iteramos inductivamente incrementando el número de golpes restantes
    for (int i = 1; i <= K; ++i) {
        // Tiempo absoluto de este golpe medido desde el inicio.
        // Dado que restan 'i' golpes y el total es 'K', el golpe actual es el (K - i + 1)
        double tiempo_actual = (K - i + 1) * s;
        double v_limite = L / tiempo_actual;

        // Si la cota que podíamos resolver con (i-1) golpes ya superó v_limite, 
        // significa que el fin de la tubería no nos limita en este paso y duplicamos el alcance.
        if (dp[i - 1] >= v_limite) {
            // El golpe óptimo se hace en v_limite. Las velocidades altas están cubiertas 
            // por la DP anterior y las bajas ganan el mismo tamaño.
            dp[i] = dp[i - 1] + (dp[i - 1] - v1);
        } else {
            // Si dp[i-1] es menor que v_limite, la tubería limita el extremo derecho.
            // El golpe óptimo se ubica en v_limite, expandiendo el rango de forma proporcional.
            dp[i] = dp[i - 1] + (v_limite - v1);
        }

        // Si logramos cubrir o superar v2, esta cantidad de golpes es válida.
        if (dp[i] >= v2) return true;
    }

    return dp[K] >= v2;
}

int main() {
    // Optimización de flujos de consola
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int num_casos;
    if (cin >> num_casos) {
        while (num_casos--) {
            double L, v1, v2, t, s;
            cin >> L >> v1 >> v2 >> t >> s;

            // Validación física inicial: el primer golpe ocurre en el segundo 's'.
            // Si el líquido más rápido (v2) sale antes de s, es imposible medirlo.
            if (v2 > L / s) {
                cout << "impossible\n";
                continue;
            }

            // Búsqueda binaria sobre el número de golpes K
            int low = max(0, (int)ceil(log2((v2 - v1) / t)));
            int high = 30000; // Cota superior segura para las restricciones del World Finals
            int respuesta = -1;

            while (low <= high) {
                int mid = low + (high - low) / 2;
                if (es_posible(mid, L, v1, v2, t, s)) {
                    respuesta = mid;
                    high = mid - 1; // Intentamos con menos golpes
                } else {
                    low = mid + 1;  // Requerimos más golpes
                }
            }

            if (respuesta != -1) {
                cout << respuesta << "\n";
            } else {
                cout << "impossible\n";
            }
        }
    }

    return 0;
}
