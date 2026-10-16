# Problema D
## Cuttin Cheese by GoogleAI (Busqueda Binaria)
### Time limit: 3 Segundos

# 1. Comprensión del problema:
Tenemos un bloque de queso con forma de cubo de ($100 \times 100 \times 100$) milímetros. Dentro del queso hay $n$ esferas huecas (agujeros de aire) que no se intersectan entre sí (pero pueden estar parcialmente cortadas por las paredes del cubo). Queremos realizar ($s-1$) cortes paralelos al plano ($XY$) (es decir, cortes en coordenadas $z$) de tal manera que dividamos el queso en $s$ rebanadas que tengan exactamente el mismo volumen neto de queso.
El volumen total neto del queso es:

$\text{Volumen\ Total}=100^{3}-\sum _{i=1}^{n}\text{Volumen\ de\ la\ esfera}_{i}$

Por ende, cada rebanada individual debe tener un volumen de $V_{\text{target}} = \frac{\text{Volumen Total}}{s}$. Si estamos buscando el $i-ésimo$ corte, el volumen acumulado de queso desde la base $z=0$ hasta la altura $z$ del corte debe ser exactamente:

$\text{Volumen\ Acumulado}(z)=i\times V_{\text{target}}$

## 1.1. Estrategia de la solucion:


### Paso A: Búsqueda Binaria por cada Corte

Dado que el volumen acumulado de queso aumenta de manera estrictamente monótona a medida que subimos en la coordenada $z$ (desde $0$ hasta $100$), podemos encontrar la posición exacta $z$ de cada corte utilizando búsqueda binaria.

Para cada corte $i$ (desde $1$ hasta $s-1$):

1. Inicializas el rango de búsqueda binaria: `low = 0` y `high = 100` (o el $z$ del corte anterior para optimizar).
2. Calculas el punto medio: `mid = (low + high) / 2`.
3. Calculas el volumen neto de queso por debajo de `mid`.
4. Si el volumen es menor que el objetivo ($i \times V_{\text{target}}$), ajustas `low = mid`. De lo contrario, `high = mid`.
5. Repites el proceso un número fijo de iteraciones (ej. 60 o 100 iteraciones) para asegurar la precisión decimal requerida.

### Paso B: Cálculo del Volumen por debajo de una altura \(z\)

Para saber cuánto queso hay hasta una altura $z$, calculas el volumen total del contenedor geométrico (un prisma rectangular de ($100 \times 100 \times z$)) y le restas el volumen de las secciones de las esferas que caen por debajo de esa altura $z$.

$\text{Volumen\ Neto}(z)=(100\times 100\times z)-\sum _{i=1}^{n}\text{Volumen\ de\ la\ esfera}_{i}\text{\ debajo\ de\ }z$

Para cada esfera con centro en ($x_i, y_i, z_i$) y radio ($r_{i}$), su intersección con el plano $z$ depende enteramente de su posición respecto a la altura actual:

• Si la esfera está completamente arriba de $z$ ($z_i - r_i \ge z$): No aporta volumen a restar (0).

• Si la esfera está completamente abajo de $z$ ($z_i + r_i \le z$): Se resta su volumen completo: $\frac{4}{3}\pi r_i^3$.

• Si el plano $z$ intersecta la esfera ($z_i - r_i < z < z_i + r_i$): Se genera un casquete esférico.

### Paso C: Fórmula del Casquete Esférico

La fórmula para calcular el volumen de un casquete esférico de altura \(h\) en una esfera de radio $r$ se obtiene fácilmente por cálculo integral:

$V_{\text{casquete}}=\frac{1}{3}\pi h^{2}(3r-h)$

Para aplicarlo al problema de manera directa según la porción que queda abajo:

• Determinas la altura del casquete que queda por debajo del plano: ($h = z - (z_i - r_i$).

• Aplicas la fórmula anterior usando esa $h$ y el radio de la esfera.

## 1.2. Consideraciones de la implementacion:

• Complejidad Temporal: Realizar la búsqueda binaria toma \(O(I \times n)\) por corte, donde $I$ es el número de iteraciones (ej. 60) y $n$ es la cantidad de esferas. Para los $s$ cortes, la complejidad total será $O(s \times I \times n)$. Dado que $n, s \le 10,000$, esta solución corre holgadamente dentro del límite de tiempo (toma apenas unas fracciones de segundo en C++ o Java).

• Precisión: Utiliza siempre tipos de datos de punto flotante de doble precisión (double en C++) para evitar errores acumulativos. El problema no requiere analizar si las esferas sobresalen de las paredes laterales del cubo de queso, ya que matemáticamente el volumen de aire restado sigue siendo el mismo al desplazarse el plano horizontal.

# 2. Implementacion:

Se implemento en C++ por optimizacion y velocidad

## Pseudo Codigo:

```
ALGORITMO CuttingCheese
ENTRADA:
    n: Número de esferas
    s: Número de rebanadas deseadas
    esferas: Lista de estructuras con (radio, x, y, z)

// 1. Convertir datos a milímetros para trabajar en un rango cómodo de 0 a 100
PARA CADA esfera EN esferas:
    esfera.radio = esfera.radio / 1000
    esfera.x = esfera.x / 1000
    esfera.y = esfera.y / 1000
    esfera.z = esfera.z / 1000

// 2. Calcular volúmenes base
volumen_total_queso = CHEESE_VOLUME_BELOW(100.0, esferas)
volumen_rebanada_objetivo = volumen_total_queso / s

previous_z = 0.0

// 3. Encontrar la posición de cada uno de los cortes
PARA i DESDE 1 HASTA s:
    volumen_acumulado_requerido = i * volumen_rebanada_objetivo
    
    // Búsqueda binaria sobre el eje Z
    low = previous_z
    high = 100.0
    
    REPETIR 60 VECES:
        mid = low + (high - low) / 2
        
        SI CHEESE_VOLUME_BELOW(mid, esferas) < volumen_acumulado_requerido ENTONCES:
            low = mid   // Falta queso, el corte debe ser más arriba
        SINO:
            high = mid  // Sobra queso o es exacto, el corte puede ser más abajo
    
    current_z = low
    
    // El grosor de la rebanada actual es la distancia con el corte previo
    IMPRIMIR (current_z - previous_z)
    
    previous_z = current_z // Actualizar la base para la siguiente rebanada

FIN ALGORITMO


// ====================================================================
// FUNCIÓN AUXILIAR: Calcula volumen neto de queso hasta una altura Z
// ====================================================================
FUNCIÓN CHEESE_VOLUME_BELOW(z, esferas):
    volumen_prisma_bruto = 100.0 * 100.0 * z
    volumen_aire_total = 0.0
    
    PARA CADA esfera EN esferas:
        volumen_aire_total = volumen_aire_total + AIR_VOLUME_BELOW(esfera, z)
        
    REGRESAR (volumen_prisma_bruto - volumen_aire_total)


// ====================================================================
// FUNCIÓN AUXILIAR: Calcula el aire de una esfera por debajo de Z
// ====================================================================
FUNCIÓN AIR_VOLUME_BELOW(esfera, z):
    // Caso 1: La esfera está completamente arriba del plano z
    SI (esfera.z - esfera.radio) >= z ENTONCES:
        REGRESAR 0.0
        
    // Caso 2: La esfera está completamente abajo del plano z
    SI (esfera.z + esfera.radio) <= z ENTONCES:
        REGRESAR (4.0 / 3.0) * PI * (esfera.radio^3)
        
    // Caso 3: El plano z corta la esfera de forma transversal
    altura_casquete = z - (esfera.z - esfera.radio)
    volumen_casquete = (1.0 / 3.0) * PI * (altura_casquete^2) * (3.0 * esfera.radio - altura_casquete)
    
    REGRESAR volumen_casquete
```

## 2.1. Inclusion de librerias y Definiciones globales:

```C++
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

// Constante matemática para Pi
const double PI = acos(-1.0);
```

• `#include <iostream>`: Permite usar los flujos de entrada y salida estándar (cin y cout).

• `#include <vector>`: Permite usar la estructura de datos dinámica std::vector para almacenar las esferas.

• `#include <cmath>`: Incluye funciones matemáticas como funciones trigonométricas e integrales simples.

• `#include <iomanip>`: Proporciona manipuladores de flujo como setprecision para formatear los decimales de la salida.

• `const double PI = acos(-1.0);`: Calcula de forma exacta el valor de $\pi$ utilizando el arco coseno de $-1$, garantizando la máxima precisión permitida por el tipo double.

## 2.1. Estructura de la esfera:

```C++
struct Sphere {
    double r, x, y, z;
};
```

Define un tipo de dato personalizado llamado Sphere para empaquetar de manera limpia los atributos de cada agujero del queso: el radio `r` y sus coordenadas espaciales `x, y, z`.

## 2.3 Función del Volumen de Aire por debajo de una Altura $Z$:

```C++
double air_volume_below(const Sphere& s, double z) {
    // Caso 1: La esfera está completamente por encima de z
    if (s.z - s.r >= z) {
        return 0.0;
    }
```
• Recibe una referencia constante a una esfera (para evitar copiar memoria innecesariamente) y la altura actual de corte `z`.

• `s.z - s.r >= z`: El punto más bajo de la esfera está por encima o justo en el plano de corte `z`. Por lo tanto, no hay nada de aire acumulado debajo de ese plano, devolviendo 0.0.

```C++
    // Caso 2: La esfera está completamente por debajo de z
    if (s.z + s.r <= z) {
        return (4.0 / 3.0) * PI * s.r * s.r * s.r;
    }
```
`s.z + s.r <= z`: El punto más alto de la esfera queda por debajo o justo en el plano de corte z. Esto significa que toda la esfera está contenida en la parte inferior, así que sumamos su volumen completo usando la fórmula geométrica clásica: $\frac{4}{3}\pi r^3$.

```C++
    // Caso 3: El plano z corta la esfera (calculamos el casquete esférico inferior)
    double h = z - (s.z - s.r);
    return (1.0 / 3.0) * PI * h * h * (3.0 * s.r - h);
}
```
• Si no entra en los casos anteriores, el plano corta transversalmente la esfera.

• `double h = z - (s.z - s.r);`: Calcula la altura del casquete esférico sumergido por debajo de z (restando la posición inicial más baja de la esfera de la altura del plano).

• `return ...`: Aplica la fórmula del volumen de un casquete esférico derivada por cálculo integral: $V = \frac{1}{3}\pi h^2 (3r - h)$.

## 2.4. Función del Volumen Neto de Queso por debajo de $Z$:

```C++
double cheese_volume_below(double z, const vector<Sphere>& spheres) {
    // Volumen del contenedor rectangular de 100 x 100 x z
    double total_box_vol = 100.0 * 100.0 * z;
```

`100.0 * 100.0 * z`: Calcula el volumen bruto del prisma rectangular que abarca desde la base del queso $z=0$ hasta el plano de corte actual z.

```C++
    // Restamos el aire acumulado de todas las esferas hasta la altura z
    double air_vol = 0.0;
    for (const auto& s : spheres) {
        air_vol += air_volume_below(s, z);
    }
    
    return total_box_vol - air_vol;
}
```
• `for (const auto& s : spheres)`: Recorre eficientemente cada esfera de la lista.

• `air_vol += ...`: Acumula el volumen de aire que cada una de las esferas aporta por debajo del plano z.

• Finalmente, resta el aire total acumulado del volumen bruto del contenedor para obtener el volumen neto de queso.

## 2.5. Funcion Main (Configuracion y lectura):

```C++
int main() {
    // Optimización de Entrada/Salida
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
```

Desvincula los flujos de C (stdio) de los de C++ (iostream) y vacía de manera automática los búferes de salida. Esto optimiza dramáticamente el tiempo de ejecución en problemas de jueces en línea con muchos datos.

```C++
    int n, s;
    if (!(cin >> n >> s)) return 0;

    vector<Sphere> spheres(n);
    for (int i = 0; i < n; ++i) {
        cin >> spheres[i].r >> spheres[i].x >> spheres[i].y >> spheres[i].z;
        spheres[i].r /= 1000.0;
        spheres[i].x /= 1000.0;
        spheres[i].y /= 1000.0;
        spheres[i].z /= 1000.0;
    }
```
• Lee el número de esferas n y rebanadas s.

• Inicializa un arreglo dinámico (vector) de tamaño n.

• Lee cada dimensión que originalmente viene expresada en micrómetros (enteros de $0$ a $100,000$) y las divide entre 1000.0 para mapearlas a una escala cómoda en milímetros flotantes (de $0.0$ a $100.0$).

### 2.5.1 Inicio del algortimo de corte:

```C++
    // Calcular el volumen total de queso neto a la altura máxima z = 100
    double total_cheese_vol = cheese_volume_below(100.0, spheres);
    
    // El volumen objetivo de cada una de las rebanadas
    double target_slice_vol = total_cheese_vol / s;

    double previous_z = 0.0;
    
    // Configurar precisión decimal para la respuesta de salida
    cout << fixed << setprecision(9);
```

• `cheese_volume_below(100.0, spheres)`: Evalúa todo el bloque para saber exactamente cuánto queso sólido (neto) hay en total.

• `total_cheese_vol / s`: Determina cuánta masa/volumen exacto de queso debe tener cada rebanada individual.

• `previous_z = 0.0`: Una variable de control para almacenar la altura del último corte realizado (comenzando desde el fondo).

• `setprecision(9)`: Le indica a cout que imprima los números flotantes con exactamente 9 cifras decimales.

### 2.5.2. Busqueda Binaria de los cortes:

```C++
    // Encontrar los s-1 cortes
    for (int i = 1; i <= s; ++i) {
        double target_accumulated_vol = i * target_slice_vol;
```
Este ciclo calcula la altura acumulada necesaria para llegar al corte i. Para la rebanada $1$, buscamos $1 \times V_{\text{target}}$, para la rebanada $2$ buscamos $2 \times V_{\text{target}}$, y así sucesivamente.

```C++
        double low = previous_z;
        double high = 100.0;
        
        // 60 iteraciones de búsqueda binaria garantizan precisión de sobra
        for (int iter = 0; iter < 60; ++iter) {
            double mid = low + (high - low) / 2.0;
            if (cheese_volume_below(mid, spheres) < target_accumulated_vol) {
                low = mid;
            } else {
                high = mid;
            }
        }
```
• `low = previous_z;`: Optimización matemática. Como los cortes son ascendentes, el siguiente plano obligatoriamente tiene que estar por encima de la altura del plano anterior.

• `for (int iter = 0; iter < 60; ++iter)`: En lugar de usar un límite condicional variable (while), se ejecutan exactamente 60 iteraciones. En cada paso el espacio de búsqueda se reduce a la mitad. Tras 60 ciclos, el rango se reduce por un factor de $2^{-60}$ (aproximadamente $8.6 \times 10^{-19}$), lo cual supera masivamente la precisión exigida por cualquier problema de geometría flotante.

• `mid = low + ...`: Encuentra el punto medio.

• `if (cheese_volume_below(...) < target_accumulated_vol)`: Si el queso por debajo de mid no es suficiente para alcanzar la meta acumulada, significa que el corte real está más arriba, por lo que movemos el límite inferior (low = mid). De lo contrario, nos pasamos o es el punto, reduciendo el límite superior (high = mid).

### 2.5.3. Impresion de resultados:

```C++
        double current_z = low;
        
        // El problema pide imprimir el grosor de cada rebanada individual
        cout << (current_z - previous_z) << "\n";
        
        previous_z = current_z;
    }

    return 0;
}
```

• `current_z = low;`: Tras terminar las iteraciones de la búsqueda binaria, low y high convergen de manera exacta en la coordenada del corte ideal.

• `current_z - previous_z`: La salida oficial del problema no pide la coordenada del corte, sino el grosor de la rebanada (la distancia entre el corte actual y el corte anterior).

• `previous_z = current_z;`: Actualiza el límite del último plano procesado para servir de base para la siguiente rebanada.

• `return 0;`: Finaliza el programa correctamente.

# 3. Justificacion:

La justificación de este enfoque se basa en tres pilares fundamentales: propiedades matemáticas, complejidad computacional y geometría tridimensional.

## 3.1. Monotonía: La clave para usar Búsqueda Binaria

La búsqueda binaria no solo sirve para arreglos ordenados; se puede aplicar sobre cualquier función matemática que sea estrictamente creciente (monótona).

• Si definimos la función $V(z)$ como el volumen de queso acumulado desde el suelo ($z=0$) hasta una altura $z$, sabemos con total certeza que si aumentamos $z$, el volumen $V(z)$ siempre aumentará. No hay forma de que el volumen disminuya al subir el plano de corte.

• Como la función es monótona, tiene una única solución exacta para cada rebanada objetivo. La búsqueda binaria aprovecha esto para descartar la mitad del espacio de búsqueda en cada paso, logrando una precisión microscópica en apenas 60 iteraciones.

## 3.2. Independencia de las coordenadas X e Y (Reducción de dimensiones)

El problema nos da las coordenadas ($x, y, z$) de cada esfera, lo que podría hacerte pensar que necesitas cálculo multivariable complejo. Sin embargo:
• Los cortes son estrictamente horizontales (paralelos al plano $XY$).

• Esto significa que no importa si una esfera está muy a la izquierda, a la derecha, en el centro o si se sale un poco por las paredes del cubo: el volumen de aire que intercepta un plano horizontal a la altura $z$ depende única y exclusivamente de la coordenada $z$ del centro de la esfera y de su radio $r$.

• Gracias a esto, el problema tridimensional se reduce a un análisis unidimensional sobre el eje $Z$.

## 3.3 Principio de Inclusión-Exclusión y Geometría Limpia

El enunciado del problema nos asegura explícitamente que las esferas no se intersectan entre sí. Esta es la justificación matemática para poder hacer una simple resta:

$\text{Volumen\ Neto}=\text{Volumen\ del\ Prisma\ Rectangular}-\sum \text{Volumen\ de\ cada\ Esfera}$

Si las esferas se interceptaran, calcular el volumen del solapamiento entre ellas sumergidas a una altura $z$ sería extremadamente complejo y requeriría integrales múltiples. Al ser disjuntas, podemos evaluar cada esfera de forma totalmente independiente utilizando la fórmula exacta del casquete esférico.

## 3.4. Eficiencia de cara a los límites del ICPC (Complejidad Temporal)

En competencias como el ICPC, tu solución debe correr en menos de 1 o 2 segundos. Analicemos el costo de este enfoque:

• Para cada uno de los $s$ cortes, hacemos $I$ iteraciones de búsqueda binaria (donde $I = 60$).

• En cada iteración, revisamos las $n$ esferas para calcular el volumen actual.

• La complejidad total es $\mathcal{O}(s \times I \times n)$.

Con los límites máximos del problema ($s \le 10,000$ y $n \le 10,000$), una búsqueda binaria ingenua desde cero para cada corte tardaría demasiadas operaciones. Sin embargo, aplicando la optimización de empezar la búsqueda desde el corte anterior `(low = previous_z)`, el espacio de búsqueda se reduce drásticamente en cada rebanada, permitiendo que el programa se ejecute en menos de 0.1 segundos, pasando con holgura el límite de tiempo del juez en línea.