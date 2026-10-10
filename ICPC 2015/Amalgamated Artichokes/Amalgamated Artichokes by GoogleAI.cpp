/*

Solucion al Problema A Amalgamed Artichokes by: GoogleAI

Paradigma: Greedy

Comentarios by: Calak de Astora

Input: 42 1 23 4 8 10

*/

#include <iostream>
#include <cmath>     
#include <iomanip>
#include <algorithm> 

using namespace std;

int main() {

    // Eliminamos las líneas de optimización de sincronización para que la consola responda en tiempo real
    double p, a, b, c, d;
    long long n;
    
    // 1. El programa lee los datos una sola vez (sin bucles repetitivos)
    if (cin >> p >> a >> b >> c >> d >> n) {
        
        double max_decline = 0.0;
        double max_so_far = -1e9;

        // i es long long para evitar bucles infinitos por desbordamiento de enteros
        for (long long i = 1; i <= n; ++i) {

            // Aseguramos precisión flotante nativa en senos y cosenos
            double current_price = p * (sin(a * i + b) + cos(c * i + d) + 2.0);

            if (i == 1) {
            
                max_so_far = current_price;
            
            } else {
                
                if (current_price < max_so_far) {
            
                    max_decline = max(max_decline, max_so_far - current_price);
            
                }
                if (current_price > max_so_far) {
            
                    max_so_far = current_price;
            
                }
            }
        }

        // Imprime el formato exacto del ICPC y limpia el búfer de pantalla con endl
        cout << fixed << setprecision(12) << max_decline << endl;
    }

    return 0;
}