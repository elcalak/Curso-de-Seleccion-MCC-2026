/*

Solucion al Problema A Amalgamed Artichokes by: GoogleAI

Paradigma: Programacion Dinamica

Comentarios by: Calak de Astora

Input: 42 1 23 4 8 10

*/

#include <iostream>
#include <cmath>     
#include <iomanip>
#include <algorithm> 

using namespace std;

int main() {
    double p, a, b, c, d;
    long long n;

    // El programa lee los datos una sola vez y se cierra al terminar
    if (cin >> p >> a >> b >> c >> d >> n) {
        
        // Casos base de la Programación Dinámica
        double precio_anterior = p * (sin(a * 1 + b) + cos(c * 1 + d) + 2.0);
        double dp_caida_actual = 0.0; // Representa la máxima caída que termina en el día i
        double max_caida_global = 0.0; // Registra la respuesta máxima encontrada

        // Llenamos nuestra "tabla" de DP paso a paso desde el día 2 hasta n
        for (long long i = 2; i <= n; ++i) {
            double precio_actual = p * (sin(a * i + b) + cos(c * i + d) + 2.0);
            
            // Cambio de precio de ayer a hoy
            double cambio = precio_anterior - precio_actual; 

            // ECUACIÓN DE RECURRENCIA DP:
            // La máxima caída acumulada que termina hoy es:
            // Lo que ya venía cayendo más el cambio de hoy, O cero (si el precio subió demasiado)
            dp_caida_actual = max(0.0, dp_caida_actual + cambio);

            // Actualizamos la ganancia máxima global vista hasta ahora
            max_caida_global = max(max_caida_global, dp_caida_actual);

            // El precio de hoy se convierte en el de ayer para el próximo paso
            precio_anterior = precio_actual;
        }

        // Imprime el resultado con los 12 decimales requeridos
        cout << fixed << setprecision(12) << max_caida_global << endl;
    }

    return 0;
}