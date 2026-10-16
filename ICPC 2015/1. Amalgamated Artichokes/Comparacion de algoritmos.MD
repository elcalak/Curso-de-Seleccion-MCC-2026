# Problema A
## Amalgamated Artichokes by Calak de Astora (Comparacion del algoritmos)
### Time limit: 5 Segundos

# 1. Resumen de los paradigmas

## Algoritmo del maximo acumulado (Greedy):

El problema se reduce a hacer una simple pregunta por dia:

"Si el precio más alto de la historia de esta acción ocurrió en el pasado, y yo vendiera hoy que el precio bajó... ¿esta sería la peor caída que hemos registrado?"

Esta pregunta se reponde recordando solo un dato del pasado: el precio mas alto visto hasta el momento (`max_so_far`).

## Algoritmo de Kadane (Programacion Dinamica):

La programacion dinamica piensa de manera incremental, de un paz a lo vez, se enfoca en la tendencia inmediata. La pregunta que se hace es:

"Si yo sé cuál era la mayor caída acumulada que venía sufriendo el precio justo hasta el día de ayer... ¿el cambio de precio de hoy ayuda a que esa caída sea aún peor, o el precio subió tanto que la racha de caídas se ha roto por completo?"

Esto lo respondemos usando una adaptacion del `Algoritmo de Kadane`, es un metodo para encontrar el subarreglo de suma maxima, es decir, en lugar de sumar las ganancias, nostros acumulamos las perdidas consecutivas.

## Fuerza Bruta:

Este algoritmo no es muy util que digamos, debido a su alto costo computacional y alto tiempo de procesamiento, teniendo $O(n^2)$ y una cantidad de $\frac{10^{12}}{2}$ operaciones, por lo que solo sera tomando en cuenta este metodo de manera comparativa.

# 1.1. Profundizando en Greedy:

Un algoritmo Greedy (Tambien conocido como Voraz o Codicioso), es una estrategia de busqueda el cual consiste en elegir la opcion optima en cada paso local, con la esperanza de llegar a la situacion general mas optima. [`Wikipedia`](https://es.wikipedia.org/wiki/Algoritmo_voraz)

## Esquema:

Dado un conjunto finito de entradas $C$, un algoritmo voraz devuelve un conjunto $S$ (seleccionados), tal que $S \subseteq C$ y que ademas cumple con las restricciones del problema inicial. A cada conjunto $S$ que satisfaga las restricciones se le denomina como prometedor, y si este ademas logra la funcion objetivo se minimice o maximice (Segun sea el caso), diremos que $S$ es una $Solucion\ optima$

Elementos del algoritmo:

• El conjunto $C$ de candidatos (Entradas del problema).

• Funcion solucion, la cual comprueba, en cada paso. si el subconjunto de candidatos forma una solucion (No importa si es optimo o no).

• Funcion de seleccion. Informa cual es el elemento mas prometedor para completar la solucion. Este no puede haber sido escogido con anterioridad. Cada elemento es considerado una sola vez. Luego, puede ser rechazado o aceptado y pertenecera a $C$ \ $S$.

• Funcion de factibilidad. Informa si a aprtir de un conjunto se puede llegar a una solucion. Se le aplica al conjunto de seleccionados unido con el elemento mas prometedor.

• Funcion objetivo. Es aquella que queremos maximizar o minimizar, el nucleo del problema.

## Funcionamiento:

El algoritmo esoge cada paso al mejor elemento $x ∈ C$, conocido como `Elemento mas prometedor` se elimina ese elemento del conjunto de candidatos $(C ← C$ ∖ {$x$}) y acto seguido, comprueba si la inclusion de este elemento es el conjunto de elementos seleccionados ($S \ ∪$ {$x$}) produce una solucion factible.

En caso de que asi sea, se incluye ese elemento en $S$. Si la inclusion no fuera factible, se descarta el elemento. Iteramos el bucle, comprobamios si el conjunto de seleccionados es una solucion y, si no es asi, pasando al siguiente elemento de conjunto de candidatos.

Algortimo en pseudocodigo:

```C++
Greedy(conjunto de candidatos C): solución S
    S = ∅
    while (S no sea una solución y C ≠ ∅) {
        x = selección(C)
        C = C – {x}
        if (S ∪ {x} es factible)
            S = S ∪ {x}
    }
    if (S es una solución)
        return S
    else
        return "No se encontró una solución"
```

# 1.1.1. Profundizando en el algoritmo del maximo acumulado (Running Maximum):

Un algoritmo de Running Maximum (también conocido como máximo acumulado o cumulative maximum) es un proceso que recorre una secuencia de datos (como una lista o un arreglo) y calcula el valor máximo encontrado hasta la posición actual.

Concepto basico:

Si tienes una lista de números, el "Running Maximum" en la posición i es el mayor elemento entre el inicio de la lista y esa posición i. El resultado final suele ser otra lista del mismo tamaño.

```C++
RunningMaximum(secuencia A): secuencia M
    M = ∅
    if (A == ∅)
        return M
        
    maxActual = A[0]
    M = M ∪ {maxActual}
    i = 1
    
    while (i < longitud(A)) {
        x = A[i]
        if (x > maxActual)
            maxActual = x
            
        M = M ∪ {maxActual}
        i = i + 1
    }
    return M
```

# 1.1.2. Implementacion del Algortimo de Maximo Acumulado (Greedy):

El problema nos pide escribir un programa para determinar la mayor caida de precios en una secuencia de precios. Ademas, Fatima ha realizado analisis previos y llego a la conclusion de que la ecuacion:

$price(k)= p \ · (sin(a \ · k + b) + cos(c \ · k + d) + 2)$

donde p, a, b, c y d son constantes. Puede modelar con precision las acciones en cualquier punto de tiempo

Usaremos C++ para implementar este algoritmo, por su velocidad y manejo manual de la memoria. Iniciaremos declarando las librearias que usaremos:

```C++
#include <iostream>     // Libreria standart de salida de datos de C++
#include <cmath>       // Libreria de Matematicas de C++
#include <iomanip>    // Libreria de formato de entrada y salida de datos de C++
#include <algorithm> // Libreria de algortimos de C++

using namespace std; // Asignacion de namespace standart
```

Declaramos las variables de nuestra ecuacion:

```C++
    double p, a, b, c, d;
    long long n;
```
Los tipos de datos son los adecuados segun como los pide el probelma:

$p \ (1 ≤ p ≤ 1 000), \ a, \ b, \ c, \ d \ (0 ≤ a, b, c, d ≤ 1 000)$ y $n \ (1 ≤ n ≤ 10⁶)$.

Los primeros 5 numeros ($p, \ a, \ b, \ c, \ d$) como se explico antes representan las constantes bursatiles de la ecuacion y $n$ el numero maximo de cotizaciones a tener en cuenta.

Lectura optimizada del caso:

Usamos un condicional, que dentro de este hace la lectura de datos:

```C++
if (cin >> p >> a >> b >> c >> d >> n) {
```

Al colocar el cin (Funcion para la lectura de datos) dentro del if, hacemos un solo proceso, deteniendo el programa en cada respuesta.

Una vez entramos en este condicial, inicializamos las variables de control:

```C++
double max_decline = 0.0;
double max_so_far = -1e9; // Un número muy pequeño como valor inicial
```

`max_decline` guarda la caida mas grande y se inicializa en 0.0, debido a que si no tenemos caida, la maxima caida es 0.

`max_so_far` guarda el pico mas alto del precio registrado, se inicializa en un numero muy pequeno, debido a que para inicializar el programa necesitamos tener un termino menor a los datos obtenidos.

• NOTA: Si no se logra entrar al condicional, es decir existe algun error en la captura de datos, el programa responde con el caso contrario:

```C++
    } else{ // Entra si hay un error en la lectura de datos

        cout << "Error en la lectura de datos";

    }
```

Una vez inicializadas nuestras variables de control, inicializamos el bucle unico ($O(n)$):

```C++
for (int i = 1; i <= n; ++i) {
    double current_price = p * (sin(a * i + b) + cos(c * i + d) + 2);
```
El bucle inicializa una variable de control que sera comparada con n (Que, como se menciono antes: es el numero maximo de cotizaciones a tener en cuenta).

Una vez dentro del bucle, calculamos el precio obtenido con los valores iniciales y comenzamos con nuestro Algortimo de Maximo Acumulado:

```C++
if (i == 1) {
    max_so_far = current_price; // El primer día, el precio actual es el máximo visto
} else {
    // Si el precio de hoy es menor que nuestro pico histórico, hay una caída
    if (current_price < max_so_far) {
        max_decline = max(max_decline, max_so_far - current_price);
    }
    // Si el precio de hoy supera nuestro pico histórico, actualizamos el récord
    if (current_price > max_so_far) {
        max_so_far = current_price;
    }
}
```

Iniciamos con el caso base, cuando $i = 1$ estamos en el primer dia, por lo que el precio actual es el maximo: `max_so_far = current_price` y asi terminamos el bucle para el dia 1.

Inicializamos el dia 2 calculando el precio del dia, como estamos en el dia 2 entonces $i = 2$ y con esto el caso base (el primer dia) ya no se cumple, por lo que hacemos dos compraciones: `if (current_price < max_so_far)` y `if (current_price > max_so_far)`.

En el primer caso si nuestro precio de hoy es mas chico que el $Maximo \ Historico$ encontes tenemos una caida, esta la vamos a calcular como el valor maximo entre la $Maxima \ Caida \ Historica$ y la resta del valor $Maximo \ Historico \ - \ Precio \ de \ hoy $ y esto lo guardaremos en `max_decline`, actualizando la $Maxima \ Caida \ Historica$.

En el segundo caso si nuestro precio de hoy es mas grande que el $Maximo \ Historico$, entonces actualizamos el $Maximo \ Historico$: `max_so_far = current_price`

Una vez terminado el dia dos hace lo mismo para los $n$ dias restantes.

Imprimimos el resultado ($Maxima \ Caida \ Historica$) en el formato requerido por la ICPC y limpiamos el buffer de la pantalla: `cout << fixed << setprecision(12) << max_decline << endl`

# 1.1.3 Justificacion del algoritmo de Maximo Acumulado:

# 1.2. Profundizando en Programacion Dinamica:

programación dinámica es un método para reducir el tiempo de ejecución de un algoritmo mediante la utilización de subproblemas superpuestos y subestructuras óptimas.

Una $«subestructura \ óptima»$ implica que la solución óptima de un problema puede obtenerse a partir de las soluciones óptimas de sus subproblemas. Por ejemplo, el camino más corto entre dos vértices de un grafo se puede encontrar calculando primero el camino más corto al objetivo desde todos los vértices adyacentes al de partida, y después usando estas soluciones para elegir el mejor camino de todos ellos. En general, se pueden resolver problemas con subestructuras óptimas siguiendo estos tres pasos:

    Dividir el problema en subproblemas más pequeños.
    Resolver estos problemas de manera óptima usando este proceso de tres pasos recursivamente.
    Usar estas soluciones óptimas para construir una solución óptima al problema original.

Los subproblemas se resuelven a su vez dividiéndolos en subproblemas más pequeños hasta que se alcance el caso fácil, donde la solución al problema es trivial.

Decir que un problema tiene subproblemas superpuestos es decir que se usa un mismo subproblema para resolver diferentes problemas mayores. Por ejemplo, en la sucesión de Fibonacci (F3 = F1 + F2 y F4 = F2 + F3) calcular cada término supone calcular F2. Como para calcular F5 hacen falta tanto F3 como F4, una mala implementación para calcular F5 acabará calculando F2 dos o más veces. Esto sucede siempre que haya subproblemas superpuestos: una mala implementación puede acabar desperdiciando tiempo recalculando las soluciones óptimas a problemas que ya han sido resueltos anteriormente.

Esto se puede evitar guardando las soluciones que ya hemos calculado. Entonces, si necesitamos resolver el mismo problema más tarde, podemos obtener la solución de la lista de soluciones calculadas y reutilizarla. Este acercamiento al problema se llama memoización (no confundir con memorización; en inglés es llamado memoization, véase en). Si estamos seguros de que no volveremos a necesitar una solución en concreto, la podemos descartar para ahorrar espacio. En algunos casos, podemos calcular las soluciones a problemas que de antemano sabemos que vamos a necesitar. [Wikipedia](https://es.wikipedia.org/wiki/Programaci%C3%B3n_din%C3%A1mica)

En resumen esta consta de:

• Subproblemas superpuestos

• Subestructuras óptimas

• Memoización

## Enfoques:

• Top-down: El problema se divide en subproblemas, y estos se resuelven recordando las soluciones por si fueran necesarias nuevamente. Es una combinación de memoización y recursión.

• Bottom-up: Todos los problemas que puedan ser necesarios se resuelven de antemano y después se usan para resolver las soluciones a problemas mayores. Este enfoque es ligeramente mejor en consumo de espacio y llamadas a funciones, pero a veces resulta poco intuitivo encontrar todos los subproblemas necesarios para resolver un problema dado.

## Caracteristicas:

El problema se puede dividir por etapas, y cada una de las etapas requiere de una política de decisión.

Cada etapa tiene cierto número de estados, los estados son las distintas condiciones posibles en las que se puede encontrar el sistema en cada etapa.

El efecto de la política de decisión en cada etapa es transformar el estado actual en un estado asociado con el inicio de la siguiente etapa, entonces podemos decir que un estado es una columna de nodos, cada nodo representa un estado y cada rama una política de decisión.

El procedimiento de resolución de la programación dinámica está diseñado para encontrar una política óptima para cada etapa logrando así la política óptima general.

Dado el estado actual, una política óptima para las etapas restantes es independiente de la política adoptada en etapas anteriores, por ende, la decisión inmediata óptima solo depende del estado actual y no de cómo se llegó ahí, esto es el principio de optimalidad de la programación dinámica.

Se dispone de una relación recursiva que identifica la política óptima para la etapa $n$, dada la política óptima para la etapa $n+1$.

Cuando se hace uso de la relación recursiva, el procedimiento de solución comienza al final se mueve hacia atrás etapa por etapa hasta encontrar la política óptima en cada etapa hasta la etapa inicial.

## Tipos de enfoque:

1) Programación Dinámica Determinística: El enfoque determinístico consiste en que el estado de la siguiente etapa se encuentra determinado por completo con respecto al estado y la decisión que posee la etapa actual. En la etapa n el proceso se encontrará en algún estado $S(n)$. Al tomar la decisión $x(n)$ se mueve a algún estado $S(n+1)$ en la etapa $n+1$. El valor de la función objetiva para la política óptima de ese punto en adelante se calculó previamente como $f*n+1(S(n)+1)$. La política de decisión también hace una contribución a la función objetivo. Al combinar estas dos cantidades en la forma apropiada se proporciona a la función objetivo $fn(S(n),x(n))$ la contribución de la etapa $n$ en adelante. La optimización respecto a $x(n)$ proporciona entonces $f \ * \ n(S(n))=fn(S(n),x(n))$. Una vez encontrados $x(n)$ y $fn*(S(n))$ para cada valor posible de $S(n)$, el procedimiento de solución se mueve hacia atrás una etapa.

    Los problemas de programación dinámica determinística se pueden clasificar según su función objetivo (minimizar la suma de las contribuciones de cada una de las etapas individuales, maximizar esa suma, minimizar el producto de los términos, etc.) y según la naturaleza del conjunto de estados (variables discretas o variables continuas).

2) Programación Dinámica Probabilística: En este enfoque, el valor del estado de la siguiente etapa y política de decisión queda completamente determinado mediante una distribución probabilística. Sea $S$ el número de estados posibles en la etapa $n+1$ y etiquete estos estados al lado derecho por $1,2…,S$. El sistema cambia al estado $i$ con probabilidad $pi$ $(i=1,2…,S)$ dados el estado $S(n)$ y la decisión $x(n)$ en la etapa $n$. Si el sistema cambia al estado $i$, $Ci$ es la contribución de la etapa $n$ a la función objetivo.

## Principio de optimalidad:

Cuando hablamos de optimizar nos referimos a buscar la mejor solución de entre muchas alternativas posibles. Dicho proceso de optimización puede ser visto como una secuencia de decisiones que nos proporcionan la solución correcta. Si, dada una subsecuencia de decisiones, siempre se conoce cuál es la decisión que debe tomarse a continuación para obtener la secuencia óptima, el problema es elemental y se resuelve trivialmente tomando una decisión detrás de otra, lo que se conoce como estrategia voraz. En otros casos, aunque no sea posible aplicar la estrategia voraz, se cumple el principio de optimalidad de Bellman que dicta que «dada una secuencia óptima de decisiones, toda subsecuencia de ella es, a su vez, óptima». En este caso sigue siendo posible el ir tomando decisiones elementales, en la confianza de que la combinación de ellas seguirá siendo óptima, pero será entonces necesario explorar muchas secuencias de decisiones para dar con la correcta, siendo aquí donde interviene la programación dinámica.

Contemplar un problema como una secuencia de decisiones equivale a dividirlo en problemas más pequeños y por lo tanto más fáciles de resolver como hacemos en Divide y Vencerás, técnica similar a la de programación dinámica. La programación dinámica se aplica cuando la subdivisión de un problema conduce a:

    Una enorme cantidad de problemas.
    Problemas cuyas soluciones parciales se solapan.
    Grupos de problemas de muy distinta complejidad.

# 1.2.1. Profundizando en Algoritmo de Kadane:

La idea detrás del algoritmo es encontrar la suma máxima de la submatriz cuyo último índice sea la posición en la que nos encontramos. Hay dos opciones: que la suma máxima hasta el índice en el que nos encontramos sea tal cual el elemento contenido en el índice donde nos encontramos, o que la suma máxima hasta el índice en el que nos encontramos sea el elemento contenido en el índice donde nos encontramos más la suma máxima de la submatriz cuyo último índice es el índice anterior. Damos por hecho que la máxima suma al inicio siempre será el valor del primer elemento de la matriz:

```C++
int kadane(int A[], int N) {
    int maximo_total = A[0];
    int maximo_actual = A[0];

    for (int i = 1; i < N; i++) {
        maximo_actual = max(A[i], maximo_actual + A[i]);
        if (maximo_actual > maximo_total) {
            maximo_total = maximo_actual;
        }
    }

    return maximo_total;
}
```

Este algoritmo es que se ejecuta en tiempo lineal, es decir, el número de operaciones depende directamente del tamaño del problema $O(n)$.

# 1.2.2. Implementacion del Algortimo de Kadane (Programacion Dinamica):

### Pseudo Codigo del algoritmo:

```
ALGORITMO AmalgamatedArtichokesDP
ENTRADA: p, a, b, c, d, n (parámetros de la serie de tiempo)
SALIDA: max_caida_global (el mayor declive de precio encontrado)

INICIO
    // Calcular el precio base del Día 1
    precio_anterior ← p * ( sin(a * 1 + b) + cos(c * 1 + d) + 2.0 )
    
    // Inicializar los estados de la Programación Dinámica
    dp_caida_actual ← 0.0   // Caída acumulada que termina exactamente en el día actual
    max_caida_global ← 0.0  // Almacena el récord de la peor caída encontrada

    // Llenar la "tabla" de estados de forma incremental (Día 2 hasta n)
    PARA i DESDE 2 HASTA n CON PASO 1 HACER
        // Calcular precio del día actual
        precio_actual ← p * ( sin(a * i + b) + cos(c * i + d) + 2.0 )
        
        // Medir el cambio de precio de ayer a hoy (pérdida de valor)
        cambio ← precio_anterior - precio_actual
        
        // Ecuación de recurrencia DP:
        // Acumula la pérdida de hoy a la caída anterior. Si el precio sube 
        // demasiado y el resultado se vuelve negativo, la caída se reinicia a 0.
        dp_caida_actual ← MAXIMO(0.0, dp_caida_actual + cambio)
        
        // Mantener registro de la caída más grande vista en todo el viaje
        max_caida_global ← MAXIMO(max_caida_global, dp_caida_actual)
        
        // El precio de hoy pasa a ser el de ayer para el siguiente paso
        precio_anterior ← precio_actual
    FIN_PARA

    DEVOLVER max_caida_global
FIN
```

El problema nos pide escribir un programa para determinar la mayor caida de precios en una secuencia de precios. Ademas, Fatima ha realizado analisis previos y llego a la conclusion de que la ecuacion:

$price(k)= p \ · (sin(a \ · k + b) + cos(c \ · k + d) + 2)$

donde p, a, b, c y d son constantes. Puede modelar con precision las acciones en cualquier punto de tiempo

Usaremos C++ para implementar este algoritmo, por su velocidad y manejo manual de la memoria. Iniciaremos declarando las librearias que usaremos:

```C++
#include <iostream>     // Libreria standart de salida de datos de C++
#include <cmath>       // Libreria de Matematicas de C++
#include <iomanip>    // Libreria de formato de entrada y salida de datos de C++
#include <algorithm> // Libreria de algortimos de C++

using namespace std; // Asignacion de namespace standart
```

Declaramos las variables de nuestra ecuacion:

```C++
double p, a, b, c, d;
long long n;
```
Los tipos de datos son los adecuados segun como los pide el probelma:

$p \ (1 ≤ p ≤ 1 000), \ a, \ b, \ c, \ d \ (0 ≤ a, b, c, d ≤ 1 000)$ y $n \ (1 ≤ n ≤ 10⁶)$.

Los primeros 5 numeros ($p, \ a, \ b, \ c, \ d$) como se explico antes representan las constantes bursatiles de la ecuacion y $n$ el numero maximo de cotizaciones a tener en cuenta.

Lectura optimizada del caso:

Usamos un condicional, que dentro de este hace la lectura de datos:

```C++
if (cin >> p >> a >> b >> c >> d >> n) {
```

Al colocar el cin (Funcion para la lectura de datos) dentro del if, hacemos un solo proceso, deteniendo el programa en cada respuesta.

Una vez entramos en este condicial, inicializamos los casos base:

```C++
 // Casos base de la Programación Dinámica
double precio_anterior = p * (sin(a * 1 + b) + cos(c * 1 + d) + 2.0);
double dp_caida_actual = 0.0; // Representa la máxima caída que termina en el día i
double max_caida_global = 0.0; // Registra la respuesta máxima encontrada
```

Aqui podemos ver que `precio_anterior` es `p * (sin(a * 1 + b) + cos(c * 1 + d) + 2.0)` que es el calculo del primer dia.

Inicializamos nuestra matriz de DP desde el dia 2:

```C++
for (long long i = 2; i <= n; ++i) {
    double precio_actual = p * (sin(a * i + b) + cos(c * i + d) + 2.0);
```

Claramente el dia se inicia calculando el precio del dia y y luego calcula el cambio en el precio:

```C++
// Cambio de precio de ayer a hoy
double cambio = precio_anterior - precio_actual; 
```

Con esto procedemos a la ecuacion de recurrencia para calcular la $Caida \ Actual$:

```C++
dp_caida_actual = max(0.0, dp_caida_actual + cambio);
```

La ecuacion de recurrencia calcula el maximo entre 0 y la $Caida \ Actual$ mas el cambio previamente calculado, de esta manera actualizamos $Caida \ Actual$ y calculamos la $Maxima \ Caida \ Historica$:

```C++
max_caida_global = max(max_caida_global, dp_caida_actual);
```

La $Maxima \ Caida \ Historica$ se actualiza buscando el maximo entre la $Maxima \ Caida \ Historica$ y la $Caida \ Actual$ (Ecuacion de recursividad), una vez encontrada la $Maxima \ Caida \ Historica$, procedemos a actualizar el prencio del dia anterior:

```C++
// El precio de hoy se convierte en el de ayer para el próximo paso
precio_anterior = precio_actual;
```
Se actualiza el `precio_anterior` con `precio_actual` ya que el precio calculado al inicio del dia se convierte en el del dia anterior para la sigueinte iteracion.

Y una vez acabado el segundo dia, el bucle se repite para los $n$ casos restantes.

Por ultimo imprimimos el resultado en el formato requerido por el ICPC:

```C++
// Imprime el resultado con los 12 decimales requeridos
cout << fixed << setprecision(12) << max_caida_global << endl;
```
# 1.2.3. Justificacion del Algoritmo de Kadane: