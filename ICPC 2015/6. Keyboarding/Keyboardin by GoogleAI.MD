# Problema F
## Keyboarding by GoogleAI (Busqueda de anchura)
### Time limit: 4 Segundos

# 1. Enfoque:

Para resolver este problema de forma eficiente, transformamos el teclado en un tablero de juego por el que nos movemos usando una Búsqueda en Anchura (BFS) modificada.
Aquí te explico el enfoque paso a paso:

## 1.1. El Espacio de Estados (¿Qué necesitamos recordar?)

En un BFS estándar en una cuadrícula, solo necesitas saber tu posición actual (fila, columna). Sin embargo, aquí también te interesa saber qué parte del texto objetivo ya has escrito.
Por lo tanto, cada estado en nuestra cola de BFS será una tupla con tres valores:

• r: La fila actual del cursor.

• c: La columna actual del cursor.

• idx: El índice del próximo carácter que necesitamos escribir del texto objetivo.

## 1.2. Precomputación de Movimientos (El truco de la velocidad)

Si desde cada tecla intentamos simular el movimiento celda por celda hacia arriba, abajo, izquierda o derecha hasta encontrar una letra diferente, el algoritmo será demasiado lento y excederá el tiempo límite (Time Limit Exceeded).
La solución: Antes de empezar el BFS, calcula a dónde te lleva presionar cada una de las 4 flechas desde cada posición (r, c) del teclado. Guardas esto en una matriz o tabla de consulta directa. Así, cuando el BFS esté corriendo, calcular el siguiente salto tomará un tiempo constante de $O(1)$.

## 1.3. El Algoritmo BFS paso a paso

### 1.3.1 Inicialización:

• Empiezas en la esquina superior izquierda (0, 0).

• Tu índice de texto objetivo comienza en 0.

• Si la tecla en (0, 0) coincide con el primer carácter que buscas, incrementas el índice inmediatamente y cuentas 1 "pulsación" (*).

• Añades este estado inicial a la cola y creas una matriz de visitas de tres dimensiones visitado[filas][columnas][longitud_del_texto] para evitar procesar el mismo estado dos veces.

### 1.3.2. Procesamiento de la Cola: Mientras la cola no esté vacía, sacas el estado actual (r, c, idx):

• Condición de victoria: Si idx es igual a la longitud del texto objetivo (más el carácter de fin de línea *), has terminado. Los pasos que te tomó llegar ahí son la respuesta óptima.

• Acción 1 (Escribir): Si la tecla en la posición actual (r, c) coincide exactamente con el carácter texto[idx], significa que debes presionar *. Añades a la cola el estado (r, c, idx + 1) con el costo actual más 1.

• Acción 2 (Moverse): Usas la precomputación del paso 2 para ver a qué posiciones te llevan las 4 flechas (arriba, abajo, izquierda, derecha) desde (r, c). Para cada uno de esos 4 destinos, si el estado (nuevo_r, nuevo_c, idx) no ha sido visitado, lo marcas como visitado y lo añades a la cola con el costo actual más 1.

# 2. Implementacion:

## 2.1. Modelado de Datos (Estructuras y Direcciones)

En lugar de manejar variables sueltas, agrupamos la información en componentes limpios:

```c++
struct Position { int r, c; };
struct State { int r, c, idx, dist; };
const int dr[] = {-1, 1, 0, 0};
const int dc[] = {0, 0, -1, 1};
```

• Position: Representa un punto geométrico (fila, columna) en el teclado. Lo usamos exclusivamente para almacenar el destino de los saltos.

• State: Modela un nodo dentro de nuestro grafo BFS. Mantiene el rastro de la posición del cursor (r, c), cuántos caracteres del objetivo hemos escrito (idx) y el número de botones presionados hasta el momento (dist).

• dr y dc: Vectores de dirección estándar que permiten movernos en cruz (Arriba, Abajo, Izquierda, Derecha) de forma cíclica mediante un bucle for.

## 2.2 Lectura y Preparación del Objetivo

```C++
int R, C;
cin >> R >> C;
vector<string> grid(R);
for (int i = 0; i < R; ++i) cin >> grid[i];

string target;
cin >> target;
target += "*"; // Botón de "Enter" obligatorio
int target_len = target.length();
```
Leemos las dimensiones y el diseño del teclado en un vector de cadenas de texto. Al texto objetivo (target) le concatenamos un asterisco (*) al final. Esto es vital porque el problema exige que, tras escribir la palabra, el cursor navegue hasta la tecla * y la presione una última vez para enviar el mensaje.

## 2.3. Precomputación de Saltos en tiempo constante $O(1)$

Para evitar recalcular rutas celda por celda durante el BFS (lo que daría Time Limit Exceeded), creamos una matriz tridimensional llamada next_pos[R][C][4] que guarda el destino directo de presionar cualquier flecha.

```C++
for (int r = 0; r < R; ++r) {
    for (int c = 0; c < C; ++c) {
        for (int d = 0; d < 4; ++d) {
            int nr = r, nc = c;
            bool found_different = false;
            while (true) {
                int next_r = nr + dr[d], next_c = nc + dc[d];
                if (next_r < 0 || next_r >= R || next_c < 0 || next_c >= C) break;
                nr = next_r; nc = next_c;
                if (grid[nr][nc] != grid[r][c]) { found_different = true; break; }
            }
            next_pos[r][c][d] = found_different ? Position{nr, nc} : Position{r, c};
        }
    }
}
```
• Lógica del bucle: Desde cada celda (r, c), simulamos avanzar en línea recta en la dirección d.

• La regla corregida: Si el cursor encuentra una letra distinta, frena y guardamos esa nueva coordenada. Si el bucle termina porque chocamos contra el borde del teclado sin haber cambiado de letra, la regla especial dicta que el cursor no se mueve, por lo que reiniciamos su destino a la posición original (r, c).

## 2.4. Inicialización del Grafo BFS

```C++
vector<vector<vector<int>>> dist(R, vector<vector<int>>(C, vector<int>(target_len + 1, -1)));
queue<State> q;

q.push({0, 0, 0, 0});
dist[0][0][0] = 0;
```

Creamos una matriz de visitados/distancias de 3 dimensiones: dist[Fila][Columna][ProgresoTexto]. Su tamaño en la tercera dimensión es target_len + 1 para poder registrar el estado final cuando el texto se complete. Inicializamos la cola insertando el estado de partida: cursor en la esquina superior izquierda (0,0), con 0 caracteres escritos y 0 movimientos totales.

## 2.5. Motor del BFS (Toma de Decisiones)

El ciclo procesa los estados capa por capa. Al usar un BFS clásico sobre transiciones de peso uniforme (cada pulsación vale 1), el primer estado que logre escribir todo el texto será la solución óptima.

```C++
while (!q.empty()) {
    State current = q.front(); q.pop();

    if (current.idx == target_len) {
        cout << current.dist << "\n";
        return 0;
    }
    if (current.dist > dist[current.r][current.c][current.idx]) continue;
```

• Condición de Victoria: Si current.idx == target_len, imprimimos la distancia y cerramos el programa.

• Poda: Si el estado actual de la cola ofrece una distancia peor que la ya registrada en la matriz dist, lo descartamos con continue.
A partir de aquí, el algoritmo expande el nodo actual evaluando dos caminos posibles:

### Opción A: Presionar la tecla actual (*)

```C++
if (grid[current.r][current.c] == target[current.idx]) {
    int n_idx = current.idx + 1;
    int n_dist = current.dist + 1;
    if (dist[current.r][current.c][n_idx] == -1 || n_dist < dist[current.r][current.c][n_idx]) {
        dist[current.r][current.c][n_idx] = n_dist;
        q.push({current.r, current.c, n_idx, n_dist});
    }
}
```

Si la letra sobre la que estamos parados es exactamente la que necesitamos escribir, simulamos "hacer clic". El progreso de la palabra avanza en uno (n_idx), el costo aumenta en uno (n_dist), y si este nuevo estado no ha sido visitado, lo añadimos a la cola.

### Opción B: Moverse usando las flechas

```C++
for (int d = 0; d < 4; ++d) {
    Position next_cell = next_pos[current.r][current.c][d];
    if (next_cell.r == current.r && next_cell.c == current.c) continue;

    int n_dist = current.dist + 1;
    if (dist[next_cell.r][next_cell.c][current.idx] == -1 || n_dist < dist[next_cell.r][next_cell.c][current.idx]) {
        dist[next_cell.r][next_cell.c][current.idx] = n_dist;
        q.push({next_cell.r, next_cell.c, current.idx, n_dist});
    }
}
```
Iteramos por las 4 direcciones saltando instantáneamente gracias a next_pos. Si el destino nos deja en el mismo sitio (por culpa de la regla del borde), ignoramos el movimiento con continue. De lo contrario, incrementamos el costo en 1 y, si la casilla de destino no ha sido explorada para nuestro índice actual de escritura, la registramos y la metemos a la cola.

# 3. Justificacion:

La justificación de este enfoque se basa en tres pilares fundamentales de la informática y el diseño de algoritmos: la equivalencia del problema con un grafo de pesos uniformes, la reducción radical de la complejidad temporal mediante precomputación y la correcta administración de la memoria.


## 3.1. Garantía de Optimalidad (Propiedad del BFS)

El problema nos pide encontrar la mínima cantidad de pulsaciones de teclas para escribir un texto.

• Cada acción permitida (moverse con una flecha o presionar el botón de seleccionar *) cuesta exactamente 1 pulsación.

• En teoría de grafos, cuando todas las transiciones entre estados tienen un costo uniforme (peso igual a 1), una Búsqueda en Anchura (BFS) garantiza encontrar el camino mínimo de manera óptima.

• El BFS explora el espacio de estados en forma de "olas" concéntricas: primero evalúa todo lo que se puede hacer con 1 paso, luego con 2, luego con 3, etc. Por lo tanto, la primera vez que el algoritmo encuentra un estado donde el texto se ha completado, se garantiza matemáticamente que se llegó usando el menor número de operaciones posibles. Un algoritmo como Dijkstra sería innecesario y añadiría una sobrecarga de \(O(\log V)\) por el uso de colas de prioridad.

## 3.2. Reducción de Complejidad por Precomputación (\(O(1)\) vs \(O(N)\))

Si intentáramos calcular a dónde nos lleva una flecha durante la ejecución del BFS, la complejidad se dispararía. En el peor de los casos, el cursor tendría que recorrer filas o columnas enteras una y otra vez por cada estado de la cola.

• Al calcular la matriz next_pos al inicio del programa (Precomputación), convertimos una operación de desplazamiento lineal en una consulta de tiempo constante \(O(1)\).

• La precomputación toma un tiempo insignificante de \(O(R \times C \times \max(R, C))\), pero salva al BFS de repetir millones de ciclos redundantes en teclados grandes.

## 3.3. Análisis de Complejidad Espacio-Tiempo (Demostración de Viabilidad)

Para justificar una solución en programación competitiva, esta debe entrar holgadamente en los límites del juez en línea (usualmente 1 a 4 segundos de tiempo y 256 a 512 MB de memoria).

Evaluemos el peor escenario posible del problema ICPC (Teclados de hasta $50 \times 50$ y textos de hasta $10,000$ caracteres):

• Complejidad de Tiempo: El BFS visita cada estado único a lo sumo una vez. El número total de estados posibles está definido por la tupla (fila, columna, idx).

$\text{Estados\ Máximos}=R\times C\times |{}T|{}=50\times 50\times 10,000=25,000,000\text{\ estados}$

Dado que desde cada estado hacemos 1 intento de escritura y 4 intentos de movimiento en tiempo $O(1)$, el total de operaciones del BFS ronda las $1.25 \times 10^8$ operaciones. En C++, un procesador moderno ejecuta alrededor de $10^{8}$ operaciones por segundo, por lo que la búsqueda toma aproximadamente entre 0.2 y 0.5 segundos, pasando el límite de tiempo sobradamente.

• Complejidad de Espacio: Nuestra memoria está dominada por la matriz 3D dist.
Cada celda almacena un entero (int de 4 bytes). El tamaño máximo requerido es:
$\text{Memoria}=50\times 50\times 10,001\times 4\text{\ bytes}\approx 100,010,000\text{\ bytes}\approx 95.3\text{\ MB}$
Considerando que los límites de memoria de las finales mundiales de ICPC suelen ser de 512 MB o superiores, usar menos de 100 MB es completamente seguro y eficiente.

# Conclusión

Este enfoque está plenamente justificado porque transforma un problema que parece de simulación geométrica en un problema puro de caminos mínimos sobre un espacio de estados finito. Cumple con la correctitud algorítmica gracias al BFS y respeta las restricciones de recursos gracias a la precomputación.