/*

Solucion al Problema D Cuttin Cheese by: GoogleAI

Paradigma: Flujo Máximo de Costo Mínimo (MCMF)

Comentarios by: Calak de Astora

Input:

0 4

2 5
10000 10000 20000 20000
40000 40000 50000 60000

*/

#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

// Constante matemática para Pi
const double PI = acos(-1.0);

// Estructura para almacenar los datos de cada esfera (agujero de aire)
struct Sphere {
    double r, x, y, z;
};

// Función para calcular el volumen total de aire de una esfera que está por debajo de la altura Z
double air_volume_below(const Sphere& s, double z) {
    // Caso 1: La esfera está completamente por encima de z
    if (s.z - s.r >= z) {
        return 0.0;
    }
    // Caso 2: La esfera está completamente por debajo de z
    if (s.z + s.r <= z) {
        return (4.0 / 3.0) * PI * s.r * s.r * s.r;
    }
    // Caso 3: El plano z corta la esfera (calculamos el casquete esférico inferior)
    double h = z - (s.z - s.r);
    return (1.0 / 3.0) * PI * h * h * (3.0 * s.r - h);
}

// Función que calcula el volumen neto de queso por debajo de la altura Z
double cheese_volume_below(double z, const vector<Sphere>& spheres) {
    // Volumen del contenedor rectangular de 100 x 100 x z
    double total_box_vol = 100.0 * 100.0 * z;
    
    // Restamos el aire acumulado de todas las esferas hasta la altura z
    double air_vol = 0.0;
    for (const auto& s : spheres) {
        air_vol += air_volume_below(s, z);
    }
    
    return total_box_vol - air_vol;
}

int main() {
    // Optimización de Entrada/Salida
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, s;
    if (!(cin >> n >> s)) return 0;

    vector<Sphere> spheres(n);
    for (int i = 0; i < n; ++i) {
        // El problema lee el radio en micrómetros (o milímetros dependiendo de la versión),
        // pero la escala suele ser directamente transferible si todo se maneja en milímetros.
        // Dividimos entre 1000 si la entrada viene en micrómetros para pasar a milímetros (0 a 100).
        cin >> spheres[i].r >> spheres[i].x >> spheres[i].y >> spheres[i].z;
        spheres[i].r /= 1000.0;
        spheres[i].x /= 1000.0;
        spheres[i].y /= 1000.0;
        spheres[i].z /= 1000.0;
    }

    // Calcular el volumen total de queso neto a la altura máxima z = 100
    double total_cheese_vol = cheese_volume_below(100.0, spheres);
    
    // El volumen objetivo de cada una de las rebanadas
    double target_slice_vol = total_cheese_vol / s;

    double previous_z = 0.0;
    
    // Configurar precisión decimal para la respuesta de salida
    cout << fixed << setprecision(9);

    // Encontrar los s-1 cortes
    for (int i = 1; i <= s; ++i) {
        double target_accumulated_vol = i * target_slice_vol;
        
        double low = previous_z;
        double high = 100.0;
        
        // 60 iteraciones de búsqueda binaria garantizan precisión de sobra (2^-60 es infinitesimal)
        for (int iter = 0; iter < 60; ++iter) {
            double mid = low + (high - low) / 2.0;
            if (cheese_volume_below(mid, spheres) < target_accumulated_vol) {
                low = mid;
            } else {
                high = mid;
            }
        }
        
        double current_z = low;
        
        // El problema pide imprimir el grosor de cada rebanada individual (distancia entre cortes consecutivos)
        cout << (current_z - previous_z) << "\n";
        
        previous_z = current_z;
    }

    return 0;
}
