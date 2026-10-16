/*

Solucion al Problema H Qanat by: GoogleAI

Paradigma: Programacion dinamica y busqueda ternaria

Comentarios by: Calak de Astora

Input:

8 4 1

195 65 2

10000 1 1000

*/

#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

int main() {
    // Optimizar la entrada/salida estándar
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    double w, h;
    int n;
    
    if (!(cin >> w >> h >> n)) return 0;

    double m = h / w; // Pendiente de la montaña
    double a = 2.0 / (m * m - 1.0); // Constante del sistema tridiagonal

    // c[i] guardará la relación x_i / x_{i+1}
    vector<double> c(n + 1, 0.0);
    c[0] = 0.0; // Caso base: x_0 = 0

    // Calcular las proporciones de izquierda a derecha
    for (int i = 1; i <= n; ++i) {
        c[i] = -1.0 / (a + c[i - 1]);
    }

    // Reconstruir las posiciones absolutas de los pozos de derecha a izquierda
    vector<double> x(n + 2, 0.0);
    x[n + 1] = w; // El límite derecho es el ancho total w
    for (int i = n; i >= 1; --i) {
        x[i] = c[i] * x[i + 1];
    }

    // Calcular la función de costo total minimizada usando la ecuación del problema
    double total_cost = 0.0;
    for (int i = 0; i <= n; ++i) {
        double t1 = (x[i + 1] * x[i + 1]) * (m * m + m) / 2.0;
        double t2 = (x[i] * x[i]) * m / 2.0;
        double t3 = (1.0 - m * m) * ((x[i + 1] - x[i]) * (x[i + 1] - x[i])) / 4.0;
        total_cost += (t1 - t2 + t3);
    }

    // Imprimir el costo total mínimo (6 decimales)
    cout << fixed << setprecision(6) << total_cost << "\n";

    // Imprimir las posiciones de los primeros pozos (máximo 10)
    int items_to_print = min(n, 10);
    for (int i = 1; i <= items_to_print; ++i) {
        cout << fixed << setprecision(6) << x[i] << (i == items_to_print ? "" : " ");
    }
    cout << "\n";

    return 0;
}
