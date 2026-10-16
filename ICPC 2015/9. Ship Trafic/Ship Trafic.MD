# Problema I
## Pipe Stream by GoogleAI (Por barrido)
### Time limit: 3 Segundos

# 1. Enfoque del problema:

Consiste en transformar las restricciones espaciales y de movimiento en intervalos de tiempo "prohibidos", para luego encontrar la ventana de tiempo permitida más larga mediante la fusión de intervalos (Interval Merging) y un enfoque de barrido (Sweep-line algorithm).
A continuación se detalla la estrategia paso a paso para solucionar este problema de forma óptima:

## 1.1. Transformación a Intervalos Prohibidos por Carril

Dado que el ferry se mueve a una velocidad constante $v$ perpendicular a los carriles, el tiempo exacto en el que el ferry cruzará el carril \(i\) (que se encuentra a una distancia fija del origen) depende únicamente de su tiempo de salida $t_{s}$.

• Si la distancia desde el inicio hasta el carril $i$ es $Y_{i}$, el ferry estará cruzando ese carril en el instante exacto $t = t_s + \frac{Y_i}{v}$.

• Cada barco en ese carril se mueve horizontalmente a una velocidad constante $u$. Un barco te obstruirá el paso si, en ese instante exacto $t$, alguna parte de su longitud intersecta la línea de cruce del ferry ($X = 0$).

Para cada barco $j$ en el carril $i$, calculamos el rango de tiempo exacto $[t_{min}, t_{max}]$ en el que su cuerpo bloquea el cruce. Utilizando álgebra básica (y considerando la dirección del barco, Este u Oeste), puedes despejar cuándo el barco entra y sale de la trayectoria.
Al restarle $\frac{Y_{i}}{v}$ a estos tiempos, mapeas ese bloqueo directamente al tiempo de salida del ferry ($t_{s}$). Esto te dará un intervalo prohibido de partida para el ferry $[l, r]$ por cada barco.

## 1.2. Unificación de los Intervalos (Interval Merging)

Una vez que has procesado todos los barcos de todos los carriles, tendrás una lista con miles de intervalos prohibidos \([l_k, r_k]\). El objetivo cambia a: encontrar el espacio libre más grande entre los límites globales $[t_1, t_2]$ que no esté cubierto por ningún intervalo.
1. Recorte y filtrado: Clampa todos los intervalos para que queden dentro del rango permitido global $[t_1, t_2]$. Si un intervalo prohibido queda completamente fuera, se descarta.
2. Ordenamiento: Ordena todos los intervalos prohibidos basándote en su punto de inicio ($l_{k}$). Si empatan, ordénalos por su punto de finalización ($r_{k}$).
3. Fusión: Utiliza una técnica clásica de puntero/barrido para combinar los intervalos superpuestos en bloques continuos disjuntos de tiempo prohibido.

## 1.3. Búsqueda de la Ventana Máxima

Al terminar la fusión, te quedará una lista ordenada de bloques de tiempo bloqueados que no se intersectan entre sí. Los espacios "vacíos" entre estos bloques (así como entre $t_{1}$ y el primer bloque, y el último bloque y $t_{2}$) representan los intervalos seguros de salida para el ferry.

• Resta el final de un bloque bloqueado con el inicio del siguiente bloque bloqueado para obtener la duración disponible.

• Lleva un registro del máximo de estas diferencias.

• Como la geometría y las velocidades son lineales, el resultado es continuo y no requiere búsquedas binarias sobre el tiempo; basta con imprimir este valor máximo utilizando variables de punto flotante (double) para evitar problemas de precisión.

## 1.4 Complejidad Óptima

• Tiempo: $O(M \log M)$, donde $M$ es el número total de barcos (hasta $10^{5}$). El factor dominante es el ordenamiento de los intervalos.

• Espacio: $O(M)$ para almacenar los intervalos generados.

# 2. Implementacion:

## Pseudo Codigo:

```
ALGORITMO ShipTrafficMaxWindow
ENTRADA:
    n : Entero (Número de carriles)
    w : Real (Ancho de cada carril)
    u : Real (Velocidad de los barcos)
    v : Real (Velocidad del ferry)
    t1, t2 : Real (Ventana global de tiempo permitida)

ESTRUCTURA Intervalo:
    l : Real (Tiempo de inicio)
    r : Real (Tiempo de fin)

INICIO
    // Bucle para procesar múltiples casos de prueba mientras existan datos
    MIENTRAS haya datos de entrada para (n, w, u, v, t1, t2) HACER:
        
        Crear Lista vacía de Intervalos: intervalos_bloqueados

        // FASE 1: PROCESAMIENTO CINEMÁTICO DE LOS BARCOS
        PARA i DESDE 0 HASTA n - 1 HACER:
            Leer dir (Dirección: 'E' o 'W')
            Leer m (Número de barcos en este carril)

            // Tiempos absolutos en los que el ferry entra y sale físicamente del carril i
            t_entra_carril ← (i * w) / v
            t_sale_carril ← ((i + 1) * w) / v

            PARA j DESDE 0 HASTA m - 1 HACER:
                Leer longitud, pos

                // Calcular ventana absoluta [t_entrada, t_salida] donde el barco invade X = 0
                SI dir == 'E' ENTONCES
                    t_entrada ← -pos / u
                    t_salida  ← (longitud - pos) / u
                SINO // dir == 'W'
                    t_entrada ← pos / u
                    t_salida  ← (pos + longitud) / u
                FIN_SI

                // Proyectar la restricción al tiempo de salida en el origen (t_s) del ferry
                l ← t_entrada - t_sale_carril
                r ← t_salida - t_entra_carril

                // Acotar estrictamente dentro del rango global [t1, t2]
                l ← MAX(l, t1)
                r ← MIN(r, t2)

                // Si hay una superposición real de peligro, se registra el intervalo
                SI l < r - 1e-9 ENTONCES
                    Añadir Intervalo(l, r) a intervalos_bloqueados
                FIN_SI
            FIN_PARA
        FIN_PARA

        // CASO BORDE: Si no hay bloqueos, toda la ventana está libre
        SI intervalos_bloqueados está vacía ENTONCES
            IMPRIMIR (t2 - t1)
            CONTINUAR con el siguiente caso de prueba
        FIN_SI

        // FASE 2: ORDENAMIENTO CRONOLÓGICO
        ORDENAR intervalos_bloqueados de menor a mayor usando:
            - Primero por el inicio (l)
            - En caso de empate, por el fin (r)

        // FASE 3: FUSIÓN DE INTERVALOS (ALGORITMO DE BARRIDO / SWEEP-LINE)
        Crear Lista vacía de Intervalos: intervalos_fusionados
        Añadir intervalos_bloqueados[0] a intervalos_fusionados

        PARA i DESDE 1 HASTA LONGITUD(intervalos_bloqueados) - 1 HACER:
            ultimo_bloque ← Obtener el último elemento de intervalos_fusionados
            bloque_actual ← intervalos_bloqueados[i]

            // Si se solapan o son adyacentes, se expande el bloque
            SI bloque_actual.l <= ultimo_bloque.r + 1e-9 ENTONCES
                ultimo_bloque.r ← MAX(ultimo_bloque.r, bloque_actual.r)
            SINO
                // Si hay espacio libre, se agrega como un nuevo bloqueo separado
                Añadir bloque_actual a intervalos_fusionados
            FIN_SI
        FIN_PARA

        // FASE 4: EVALUACIÓN DE LAS BRECHAS LIBRES (GAPS)
        max_brecha ← 0.0

        // Brecha 1: Desde t1 hasta el inicio del primer bloque prohibido
        max_brecha ← MAX(max_brecha, intervalos_fusionados[0].l - t1)

        // Brechas intermedias: Entre bloques prohibidos consecutivos
        PARA i DESDE 1 HASTA LONGITUD(intervalos_fusionados) - 1 HACER:
            brecha_actual ← intervalos_fusionados[i].l - intervalos_fusionados[i-1].r
            max_brecha ← MAX(max_brecha, brecha_actual)
        FIN_PARA

        // Brecha final: Desde el término del último bloque prohibido hasta t2
        ultimo_bloque ← Obtener el último elemento de intervalos_fusionados
        max_brecha ← MAX(max_brecha, t2 - ultimo_bloque.r)

        // SALIDA
        IMPRIMIR max_brecha (con precisión decimal alta)

    FIN_MIENTRAS
FIN
```

## 2.1. Cabeceras y Estructura Base (Configuración)

```C++
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cmath>

using namespace std;
```
• `#include <iomanip>`: Fundamental para usar setprecision, que nos permite controlar cuántos decimales imprimimos en la salida.

• `#include <cmath>`: Lo necesitamos para abs(), que nos ayuda a comparar números flotantes con un margen de tolerancia.

```C++
struct Interval {
    double l, r;
    bool operator<(const Interval& other) const {
        if (abs(l - other.l) > 1e-9) return l < other.l;
        return r < other.r;
    }
};
```
• `double l, r`: Almacenan el inicio y el fin de un intervalo de tiempo prohibido para el ferry.

• `operator<`: Sobrecargamos este operador para que la función sort() de C++ ordene automáticamente de menor a mayor según el inicio del intervalo (l). Si dos intervalos empiezan casi al mismo tiempo (diferencia menor a 1e-9), los ordena según su final (r).

## 2.2. Control de Entrada y Lectura Segura

```C++
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
```

• Optimización de I/O: Desactivamos la sincronización de las consolas de C y C++ para que la lectura de datos (cin) sea extremadamente rápida, algo vital cuando el juez en línea tiene más de $10^{5}$ líneas de entrada.

```C++
    int n;
    double w, u, v, t1, t2;

    while (cin >> n >> w >> u >> v >> t1 >> t2) {
        vector<Interval> blocked_intervals;
```
• `while(cin >> ...)`: Esta es la estructura estándar de programación competitiva. El programa intentará leer un caso de prueba completo. Si tiene éxito, entra al bucle; si el archivo se queda sin datos (EOF), el bucle se rompe limpiamente sin colgar la computadora.

• `vector<Interval> blocked_intervals`: Aquí guardaremos todos los intervalos de tiempo en los que está prohibido salir a causa de los barcos de todos los carriles.

## 2.3. Procesamiento y Proyección Temporal (El Núcleo Matemático)

```C++
        for (int i = 0; i < n; ++i) {
            char dir;
            int m;
            cin >> dir >> m;

            double t_start_lane = (i * w) / v;
            double t_end_lane = ((i + 1) * w) / v;
```
• `t_start_lane` y `t_end_lane`: Calculamos el momento exacto en el que el ferry toca el carril i y el momento en el que lo abandona por completo. Dado que el ferry avanza verticalmente, esto depende únicamente de su velocidad v y del ancho w de los carriles que ya cruzó.

```C++
            for (int j = 0; j < m; ++j) {
                double length, pos;
                cin >> length >> pos;

                double t_entry, t_exit;

                if (dir == 'E') {
                    t_entry = -pos / u;
                    t_exit = (length - pos) / u;
                } else {
                    t_entry = pos / u;
                    t_exit = (pos + length) / u;
                }
```

• Cinemática del barco: Despejamos el tiempo en el que el barco invade la línea $X=0$.

• Hacia el Este ('E'): Avanza hacia valores de X positivos. El frente choca en $t = -pos/u$. La parte trasera (que mide length) termina de pasar en $t = (length - pos)/u$.

• Hacia el Oeste ('W'): Avanza hacia valores de X negativos. Los signos cambian porque el barco viene del lado positivo y se mueve hacia el origen.

```C++
                double l = t_entry - t_end_lane;
                double r = t_exit - t_start_lane;

                l = max(l, t1);
                r = min(r, t2);

                if (l < r - 1e-9) {
                    blocked_intervals.push_back({l, r});
                }
            }
        }
```
• Mapeo al Ferry: Para que el ferry no choque, su tiempo de salida no debe hacer que coincida con el barco mientras cruza el carril. Restando los tiempos del carril (`t_end_lane` y `t_start_lane`), transformamos los tiempos del barco en límites aplicables directamente a la hora de salida del muelle del ferry.

• Ajuste de Ventana: Usamos max y min con t1 y t2 para ignorar cualquier peligro que ocurra fuera de las horas permitidas por el problema. Si el intervalo sigue siendo real (l < r), se guarda en la lista.

## 2.4. Algoritmo de Barrido (Fusión de Zonas Rojas)

```C++
        if (blocked_intervals.empty()) {
            cout << fixed << setprecision(8) << (t2 - t1) << "\n";
            continue;
        }

        sort(blocked_intervals.begin(), blocked_intervals.end());
```

• Caso Base: Si ningún barco estorba en la simulación, el ferry puede salir en cualquier momento, así que la respuesta es simplemente la duración total (t2 - t1).

• sort: Ordenamos cronológicamente todas las amenazas calculadas.

```C++
        vector<Interval> merged;
        merged.push_back(blocked_intervals[0]); 

        for (size_t i = 1; i < blocked_intervals.size(); ++i) {
            if (blocked_intervals[i].l <= merged.back().r + 1e-9) {
                merged.back().r = max(merged.back().r, blocked_intervals[i].r);
            } else {
                merged.push_back(blocked_intervals[i]);
            }
        }
```

• Fusión (Interval Merging): Creamos un nuevo vector merged y metemos la primera amenaza. Luego recorremos las demás:
	
• Si el siguiente intervalo de peligro empieza antes (o justo cuando) termina el anterior (`<= merged.back().r`), significa que se enciman. Extendemos el fin del peligro actual usando max.
	
• Si no se enciman, encontramos un periodo de tiempo libre intermedio. Guardamos la nueva amenaza por separado.

## 2.5. Medición de Brechas y Salida

```C++
        double max_gap = 0.0;

        max_gap = max(max_gap, merged[0].l - t1);

        for (size_t i = 1; i < merged.size(); ++i) {
            max_gap = max(max_gap, merged[i].l - merged[i-1].r);
        }

        max_gap = max(max_gap, t2 - merged.back().r);

        cout << fixed << setprecision(8) << max_gap << "\n";
    }
    return 0;
}
```
• Medición de Gaps: Las ventanas seguras de salida son los espacios en blanco entre los bloques prohibidos de merged.

1. Calculamos el tiempo libre antes del primer bloqueo (`merged[0].l - t1`).
2. Calculamos los tiempos libres entre bloqueos consecutivos (`merged[i].l - merged[i-1].r`).
3. Calculamos el tiempo libre que queda después del último bloqueo hasta el cierre (`t2 - merged.back().r`).

• Con la función `max()`, nos quedamos con el espacio más grande posible y lo imprimimos usando `setprecision(8)` con un salto de línea (`\n`) para que el formato de salida sea impecable.

# 3. Justificacion:

La justificación matemática y algorítmica de por qué este enfoque es el correcto (y por qué garantiza pasar los límites del problema) se basa en tres pilares fundamentales: reducción de dimensionalidad, continuidad geométrica y eficiencia asintótica.
Aquí tienes el sustento formal de la solución:

## 3.1. Reducción de Dimensionalidad (De 2D + Tiempo a 1D)

Un enfoque ingenuo intentaría simular el movimiento del ferry en el plano bidimensional $(X, Y)$ a lo largo del tiempo $t$. Esto es extremadamente complejo porque el espacio de búsqueda es continuo e infinito.

La justificación de nuestro algoritmo radica en que el ferry tiene una trayectoria fija y obligatoria. El ferry no puede esquivar barcos moviéndose hacia los lados; solo se desplaza verticalmente a velocidad constante $v$.

• Al fijar la trayectoria del ferry, su posición vertical es una función directa del tiempo de salida: $Y_{ferry}(t) = v \cdot (t - t_s)$.

• Esto nos permite proyectar todas las colisiones espaciales de los barcos en un solo eje: el tiempo de salida $t_{s}$. Al transformar cada barco en un intervalo cerrado de "salidas prohibidas" $[l, r]$, convertimos un problema de geometría dinámica en 2D en un problema estático de cobertura de intervalos en 1D.

## 3.2. El Principio de Continuidad (Por qué basta con medir las brechas)

Dado que todas las ecuaciones de movimiento tanto de los barcos como del ferry son lineales (velocidad constante, sin aceleración), los límites de los intervalos prohibidos calculados son exactos y continuos.

• Si un barco $j$ bloquea al ferry para cualquier salida en el rango $[l_j, r_j]$, significa que cualquier valor infinitesimal intermedio $t_s \in [l_j, r_j]$ resultará en una colisión física.

• Al fusionar todos los intervalos superpuestos mediante el algoritmo de barrido (sweep-line), el complemento de estos conjuntos (los espacios vacíos o gaps) representa todas las ventanas de tiempo en las que el ferry tiene un éxito del 100% para cruzar sin colisiones.

• Como las funciones son lineales, la ventana de tiempo permitida más larga tiene que ser necesariamente la distancia máxima entre el final de un bloqueo y el inicio del siguiente (o los límites globales $t_{1}$ y $t_{2}$). No existen "trampas" o colisiones espontáneas a mitad de una brecha libre.

## 3.3. Consideración del Ancho del Ferry y del Carril

Una de las sutilezas del problema es que el ferry tarda un tiempo no despreciable en cruzar cada carril de ancho $W$. El algoritmo justifica esto extendiendo los intervalos prohibidos:

• En lugar de asumir que el ferry interactúa con el carril en un solo instante, calculamos la ventana completa $[t_{start\_lane}, t_{end\_lane}]$.

• Para que haya una colisión, el intervalo de ocupación del barco $[t_{entry}, t_{exit}]$ debe solaparse con el intervalo de ocupación del ferry. La condición de solapamiento de dos intervalos $[A, B]$ y $[C, D]$ es $A < D$ y $C < B$.

• Al despejar $t_{s}$ de esta inecuación de solapamiento, la matemática nos dicta exactamente que el peligro ocurre cuando $t_s \in [t_{entry} - t_{end\_lane}, t_{exit} - t_{start\_lane}]$. Esto demuestra rigurosamente que la extensión de los límites $l$ y $r$ en el código cubre perfectamente el tamaño del carril.

## 3.4. Eficiencia y Correctitud del Ordenamiento (Sweep-Line)

El problema impone un límite de hasta $10^{5}$ barcos. Una solución por fuerza bruta que intente verificar colisiones en intervalos discretos de tiempo (por ejemplo, cada $0.0001$ segundos) fallaría por dos motivos: Tiempo de Ejecución Excesivo (TLE) e Imprecisión Numérica (WA).

• Correctitud: Ordenar los intervalos por su punto de inicio garantiza que el algoritmo de barrido procese la línea de tiempo de manera cronológica estricta. Al mantener el final del intervalo actual (`merged.back().r`), podemos decidir de manera unívoca si el siguiente intervalo extiende el bloqueo actual o abre una nueva ventana libre.

• Complejidad Asintótica: El ordenamiento toma $\mathcal{O}(M \log M)$ y la fusión toma $\mathcal{O}(M)$ pasos lineales. Para $M = 10^5$, el número de operaciones es de aproximadamente $1.7 \times 10^6$, lo cual se ejecuta en apenas unos milidisegundos en C++, cumpliendo sobradamente con el límite estándar de 1.0 segundo del ICPC.