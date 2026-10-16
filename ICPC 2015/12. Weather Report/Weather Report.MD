# Problema L
## Tours by GoogleAI (Huffman)
### Time limit: 2 Segundos

# 1. Enfoque del problema:

El problema pide calcular la longitud esperada del código Huffman para todas las \(4^{n}\) cadenas de longitud \(n\) posibles usando un alfabeto de 4 estados del clima (\(S, C, R, F\)), donde cada estado tiene una probabilidad independiente dada (\(p_S, p_C, p_R, p_F\)). Para un \(n \le 20\), el número total de estados es \(4^{20} \approx 1.1 \times 10^{12}\), lo cual descarta por completo una simulación directa con una cola de prioridad normal.

Enfoque de Solución Paso a Paso


## 1.1. Agrupación por Combinaciones Frecuenciales (Estados Iniciales)

La probabilidad de cualquier cadena depende únicamente de cuántas veces aparece cada clima, no de su orden específico. Por lo tanto, podemos agrupar las hojas iniciales según el número de veces $f_S, f_C, f_R, f_F$ que aparece cada letra, cumpliendo que $f_S + f_C + f_R + f_F = n$.

• El número de combinaciones únicas de este tipo es el número multinomial, que para $n = 20$ es solo $\binom{20+4-1}{3} = \binom{23}{3} = 1771$ grupos.

• Para cada grupo con frecuencias fijas, la probabilidad de una cadena es $P = p_S^{f_S} \cdot p_C^{f_C} \cdot p_R^{f_R} \cdot p_F^{f_F}$.

• La cantidad de cadenas (su conteo inicial) que comparten exactamente esa misma probabilidad viene dada por el coeficiente multinomial:

$\text{Conteo}=\frac{n!}{f_{S}!\cdot f_{C}!\cdot f_{R}!\cdot f_{F}!}$

## 1.2. Cola de Prioridad Agrupada (Compresión de Estados)

En lugar de insertar cada cadena individualmente en la cola de prioridad, insertamos pares de la forma (probabilidad, conteo).
Para procesar la cola de manera óptima de acuerdo con las reglas de Huffman, extraemos el elemento con la menor probabilidad disponible:

• Si el conteo del elemento con menor probabilidad es par (ej. $2k$): Significa que estos nodos se pueden emparejar perfectamente entre sí. Formarán $k$ nuevos nodos con probabilidad $2 \cdot P$ y sumamos $$2k \cdot P$ al costo total acumulado. Insertamos (2 * P, k) de regreso en la cola.

• Si el conteo es impar (ej. $2k + 1$): Emparejamos $2k$ de ellos formando $k$ nuevos nodos de probabilidad $2 \cdot P$, sumamos $2k \cdot P$ al costo total, e insertamos (2 * P, k). El nodo restante con probabilidad $P$ e inicialmente un conteo de $1$ se debe emparejar con el siguiente elemento de menor probabilidad en la cola (el cual incrementará su probabilidad a $P + P_{\text{sig}}$).

## 1.3. Tipos de Datos y Precisión

• double / long double: Dado que las probabilidades de los nodos individuales pueden volverse sumamente pequeñas a medida que $n$ crece, es vital usar double o long double para mitigar errores de precisión numérica.

• long long o __int128: El conteo de las cadenas supera el límite de los enteros de 32 bits, por lo que es mandatorio utilizar tipos de datos de 64 bits o superiores para evitar desbordamientos.

Complejidad

Dado que en cada paso combinamos de manera masiva los nodos repetidos, el tamaño de la cola de prioridad se mantiene sumamente pequeño durante toda la ejecución del programa. El tiempo de ejecución final se reduce a una fracción de segundo (unos pocos milisegundos), cumpliendo con creces el límite de tiempo de 2 segundos del problema.

# 2. Implementacion:


## 2.1. Cabeceras, Configuración y Estructura del Nodo

Esta primera sección incluye las librerías necesarias para el manejo de flujos de entrada/salida de alta velocidad y la optimización de precisión decimal requerida por el ICPC.

```C++
#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
#include <algorithm>

using namespace std;

// Estructura para almacenar los grupos de nodos en la cola de prioridad
struct Node {
    long double prob; // Probabilidad acumulada de este tipo de nodo
    long long count;  // Cuántos nodos idénticos comparten exactamente esta probabilidad

    // Sobrecarga de operadores para que la cola de prioridad (Min-Heap) 
    // coloque arriba siempre el lote que tenga la menor probabilidad.
    bool operator>(const Node& other) const {
        return prob > other.prob;
    }
};
```

## 2.2. Precomputación de Factoriales

Para calcular cuántas cadenas idénticas existen de forma masiva, requerimos coeficientes multinomiales. Esta función calcula los factoriales desde $0!$ hasta $n!$ en tiempo lineal para evitar desbordamientos y cálculos repetitivos.

```C++
// Función para calcular los factoriales necesarios para el coeficiente multinomial
vector<long long> get_factorials(int n) {
    vector<long long> fact(n + 1, 1);
    for (int i = 2; i <= n; ++i) {
        fact[i] = fact[i - 1] * i;
    }
    return fact;
}
```

## 2.3. Lectura de Datos y Generación Combinatoria (De $4^{20}$ a 1,771 estados)

En lugar de generar individualmente cada una de las más de $1.1 \times 10^{12}$ cadenas, utilizamos tres bucles anidados para iterar sobre la cantidad de veces que aparece cada clima ($f_S, f_C, f_R, f_F$).

```C++
int main() {
    // Optimización de entrada/salida estándar para Programación Competitiva
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

    // Generar todas las combinaciones de frecuencias tales que sumen exactamente 'n'
    for (int fS = 0; fS <= n; ++fS) {
        for (int fC = 0; fC <= n - fS; ++fC) {
            for (int fR = 0; fR <= n - fS - fC; ++fR) {
                int fF = n - fS - fC - fR;

                // 1. Calculamos la probabilidad individual de UNA sola cadena con esta distribución
                long double current_prob = 1.0;
                for (int i = 0; i < fS; ++i) current_prob *= p[0];
                for (int i = 0; i < fC; ++i) current_prob *= p[1];
                for (int i = 0; i < fR; ++i) current_prob *= p[2];
                for (int i = 0; i < fF; ++i) current_prob *= p[3];

                // 2. Coeficiente Multinomial: Averigua cuántas cadenas distintas tienen esta misma cantidad de climas
                long long ways = fact[n] / (fact[fS] * fact[fC] * fact[fR] * fact[fF]);

                // 3. Si la combinación es válida, metemos el "lote" completo a nuestro Min-Heap
                if (ways > 0) {
                    pq.push({current_prob, ways});
                }
            }
        }
    }
```

## 2.4. Simulación del Algoritmo de Huffman (Run-Length Compression)

Esta sección contiene el motor principal del algoritmo. Extrae los lotes de la cola y procesa las uniones de ramas del árbol de Huffman de forma masiva mediante operaciones aritméticas en lugar de hacerlo uno por uno.

```C++
    long double total_expected_length = 0.0;

    // Simulación del algoritmo de Huffman agrupando nodos repetidos
    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();

        // Condición de parada: Si es el último grupo de nodos y solo queda 1, el árbol está completo
        if (pq.empty() && current.count == 1) {
            break;
        }

        // PASO A: Combinar parejas idénticas dentro del mismo lote
        if (current.count > 1) {
            long long pairs = current.count / 2;       // Cuántas parejas idénticas podemos formar de golpe
            long double new_prob = current.prob * 2.0; // Al unir dos nodos idénticos, su probabilidad se duplica
            
            // Cada combinación suma el costo de sus dos ramas al total esperado de Huffman
            total_expected_length += new_prob * pairs;
            
            // Insertamos el nuevo lote de nodos combinados de vuelta a la cola
            pq.push({new_prob, pairs});
            
            // Aplicamos módulo 2 para ver si quedó un nodo "huérfano" suelto (0 si era par, 1 si era impar)
            current.count %= 2; 
        }

        // PASO B: Resolver el nodo huérfano (Impar)
        if (current.count == 1) {
            if (pq.empty()) break; // Si no hay más elementos en la cola, no hay con quién fusionarlo

            Node next_node = pq.top(); // Tomamos el siguiente lote disponible con menor probabilidad
            pq.pop();

            // Fusionamos nuestro nodo huérfano con EXACTAMENTE 1 elemento extraído del lote vecino
            long double combined_prob = current.prob + next_node.prob;
            total_expected_length += combined_prob;
            
            // Insertamos el nuevo nodo único resultante de la mezcla híbrida
            pq.push({combined_prob, 1});

            // Como le "robamos" un nodo al lote vecino, restamos 1 a su contador.
            // Si al lote vecino todavía le quedan nodos disponibles, lo devolvemos ordenadamente a la cola.
            if (next_node.count > 1) {
                pq.push({next_node.prob, next_node.count - 1});
            }
        }
    }

    // Imprimir el resultado con la precisión exacta de 6 decimales requerida por el problema
    cout << fixed << setprecision(6) << total_expected_length << "\n";

    return 0;
}
```

# 3. Justificacion

La justificación teórica y matemática de por qué este algoritmo funciona (y por qué es la única manera de obtener un veredicto de Accepted) se basa en tres pilares fundamentales: la naturaleza del algoritmo de Huffman, la simetría combinatoria del problema y la técnica de compresión de estados.

## 3.1. Optimización mediante la Propiedad de Huffman

El problema pide calcular la longitud esperada del código óptimo de prefijo para un conjunto de mensajes. El Teorema de Huffman demuestra que se puede construir este código óptimo fusionando repetidamente los dos elementos con las frecuencias (o probabilidades) más bajas de forma recursiva.

Tradicionalmente, la longitud esperada se calcula multiplicando la longitud del código de cada hoja por su probabilidad:

$\text{Costo}=\sum (l_{i}\times p_{i})$

Sin embargo, existe una propiedad matemática alternativa e idéntica para calcular el costo en Huffman: la suma de las probabilidades de todos los nodos internos (no hojas) del árbol. Cada vez que fusionamos dos nodos con probabilidades A y B, el nuevo nodo padre tiene una probabilidad de A + B. Sumar este valor al costo acumulado en cada fusión equivale exactamente a contabilizar un bit adicional en la longitud del código para todos los descendientes de ese nuevo subárbol. El código lo aprovecha directamente en las líneas:

```C++
total_expected_length += new_prob * pairs; // Para parejas idénticas
total_expected_length += combined_prob;    // Para fusiones mixtas
```

## 3.2. Reducción del Espacio de Estados por Isomorfismo (Simetría)

El enunciado indica que cada carácter de la cadena se elige de forma independiente e idénticamente distribuida (i.i.d.).
Si tenemos, por ejemplo, n=3 y evaluamos las cadenas SSR y SRS, sus probabilidades son:

• $P(\text{"SSR"}) = p_S \times p_S \times p_R = p_S^2 \cdot p_R$

• $P(\text{"SRS"}) = p_S \times p_R \times p_S = p_S^2 \cdot p_R$

Al algoritmo de Huffman no le importa el orden de los caracteres, solo el valor numérico de la probabilidad. Como ambas cadenas tienen exactamente la misma probabilidad, se comportarán de manera idéntica en el árbol.

En lugar de procesar las $4^{n}$ cadenas, las agrupamos en clases de equivalencia definidas por el vector de frecuencias ($f_S, f_C, f_R, f_F$) que sumen n. El número de formas distintas de organizar estos caracteres es el coeficiente multinomial:

$\text{ways}=\frac{n!}{f_{S}!\cdot f_{C}!\cdot f_{R}!\cdot f_{F}!}$

Para el peor caso (n = 20), el número de cadenas es 4²⁰ = 1,099,511,627,776 (un billón), pero el número de vectores de frecuencia únicos es un problema de combinaciones con repetición: $\binom{20 + 4 - 1}{4 - 1} = \binom{23}{3} = \mathbf{1,771}$. Hemos reducido el problema inicial en un 99.9999998%.

## 3.3. Run-Length Compression (Compresión por bloques) en la Cola de Prioridad

Aunque empecemos con solo 1,771 grupos en la cola, el coeficiente multinomial ways puede ser un número gigantesco (hasta 11,732,745,024). Si insertáramos cada nodo individualmente a la cola de prioridad, el programa colapsaría por falta de memoria y tiempo ($O(M \log M)$ donde $M = 4^n$).

La justificación del bucle de Huffman optimizado es que podemos procesar lotes de elementos idénticos en tiempo constante O(1):

• Caso Par: Si tenemos K nodos con exactamente la misma probabilidad P, el algoritmo de Huffman tradicional tomaría el primero y el segundo (padre 2P), luego el tercero y el cuarto (padre 2P), y así sucesivamente. Nosotros calculamos directamente que se formarán $\lfloor K/2 \rfloor$ padres de valor 2P usando una sola división elemental.

• Caso Impar: Si K es impar, tras emparejar todos los elementos internos posibles, quedará exactamente un nodo "huérfano". Por la propiedad del Min-Heap, este nodo es estrictamente el más pequeño de todo el sistema, por lo que matemáticamente está obligado a fusionarse con el elemento más pequeño del siguiente grupo disponible (next_node.prob), sin importar si ese grupo vecino tiene uno o millones de elementos.

## 3.4. Conclusión de Complejidad

Gracias a estas tres justificaciones, la cola de prioridad nunca crece de manera descontrolada. En cada iteración del bucle, o bien reducimos a la mitad el tamaño de un lote gigante, o bien eliminamos por completo un nodo huérfano combinándolo con el siguiente. La complejidad temporal del algoritmo pasa de ser exponencial $O(4^n \cdot n)$ a ser prácticamente instantánea (unos pocos milisegundos), garantizando la solución óptima bajo los límites estrictos del ICPC.