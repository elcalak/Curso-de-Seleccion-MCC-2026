# Problema C
## Catering by GoogleAI (Flujo Máximo de Costo Mínimo)
### Time limit: 2 Segundos

# 1. Enfoque de Modelado con Red de Flujo:

Para construir la red de flujo, se realiza una división de nodos (Node Splitting) para cada ubicación con el fin de garantizar que cada cliente reciba exactamente el servicio necesario:

## 1.1. Estructura de los Nodos

• Fuente $S$ y Sumidero $T$.

• La Sede Central (Compañía): Representada por un nodo de origen (o un par de llegada/salida).
• Los Clientes ($1 \ a \ N$): Cada cliente $i$ se divide en dos nodos distintos para representar el flujo de entrada y salida:
	
• $In_{i}$: Nodo de llegada (recepción del equipo).
	
• $Out_{i}$: Nodo de salida (despacho del equipo hacia el siguiente servicio).

## 1.2. Definición de las Aristas (Capacidades y Costos)

• Desde la Sede:
	
• Se conecta un enlace especial desde la sede hacia los clientes o desde $S$ hacia la salida de la sede con capacidad $K$ (el límite de equipos disponibles) y costo $0$.

• Flujo por Cliente:

• Para asegurar que cada cliente reciba servicio exactamente una vez, las aristas desde la entrada del cliente hacia el sumidero ($In_i \rightarrow T$) tendrán capacidad $1$ y costo $0$.

• De igual manera, las aristas que inician un despacho ($S \rightarrow Out_i$) tendrán capacidad $1$ y costo $0$.

• Transiciones entre Servicios ($i \rightarrow j$) donde ($i < j$):

• Se conecta el nodo de salida de un cliente con el nodo de entrada de un cliente posterior ($Out_i \rightarrow In_j$) con capacidad $1$ y un costo equivalente al precio del traslado proporcionado por la matriz del problema.

• También se incluyen las aristas desde la salida de la Sede hacia la entrada de cualquier cliente ($Out_{Sede} \rightarrow In_j$) con el costo correspondiente de enviar un equipo desde la central.

## 1.3. Resolución

Una vez montado el grafo, el objetivo es inyectar un flujo máximo total de $N$ (o ($N+K$)) dependiendo de las variantes exactas del sumidero) empleando algoritmos tradicionales de costo mínimo como Successive Shortest Path (usando SPFA o Dijkstra con potenciales). El costo acumulado por el algoritmo al alcanzar dicho flujo será la solución óptima.

# 2. Justificacion:


## 2.1. Robustez ante los límites del problema

El problema cuenta con límites de $N \le 100$ (clientes) y $K \le 100$ (equipos). Un diseño basado en MCMF procesará este grafo de tamaño moderado de manera sumamente holgada. Aunque DP es teóricamente viable, modelar el estado de múltiples equipos independientes moviéndose de forma asíncrona sobre una línea temporal eleva drásticamente la complejidad del estado y la probabilidad de cometer errores de lógica en las transiciones (especialmente al registrar qué equipos quedan libres).

## 2.2. Simplicidad en el Código y Reducción de Bugs

• Problema B ya absorbió una gran cantidad de tiempo en el desarrollo del equipo debido a su complejidad geométrica y manejo de fracciones/intersecciones.
• Problema C (MCMF): Si tu equipo ya cuenta con una plantilla estándar de un algoritmo de flujo como Successive Shortest Path con Dijkstra o SPFA, la resolución de este problema se reduce únicamente a la construcción del grafo. No hay que inventar lógica desde cero: solo se lee la matriz de costos y se crean las aristas correspondientes.

## 2.3. Facilidad de Mapeo frente a Restricciones Complejas

La restricción crítica de que dos equipos no pueden atender al mismo cliente simultáneamente (cada cliente se visita exactamente una vez) es un dolor de cabeza para modelar en DP, pero se resuelve de forma nativa en Flujo limitando la capacidad de las aristas internas del nodo del cliente a exactamente 1. De igual manera, el límite de $K$ equipos globales se mapea directamente poniendo una capacidad de K en la salida de la sede central.

# 3. Implementacion

Usamos C++ para hacer esta implementacion:


## 3.1. Las Estructuras en Memoria (struct Edge y class MinCostMaxFlow)

Antes de que empiece a correr el algoritmo, el programa define cómo se va a guardar el grafo en la RAM.

La Arista (struct Edge)

Cada conexión (tubería) entre dos nodos no es solo un número; es un objeto que guarda 5 datos críticos:

• `to`: A qué nodo llega.

• `capacity`: Cuánto flujo máximo soporta.

• `flow`: Cuánto flujo está pasando actualmente por ella (empieza en 0).

• `cost`: Cuánto cuesta mandar 1 unidad de flujo por aquí.

• `rev`: La posición (índice) donde está su "hermana gemela inversa" en el vector del nodo vecino. Esto sirve para poder "deshacer" caminos instantáneamente en tiempo $O(1)$.

```C++
struct Edge {
    int to;
    int capacity;
    int flow;
    long long cost;
    int rev; // Índice de la arista inversa en el vector del nodo 'to'
};
```

### El Constructor de la Clase:

Cuando en el main escribes `MinCostMaxFlow mcmf(total_nodes);`, la computadora reserva espacio en la memoria para los siguientes vectores dinámicos:

• `adj`: Es un vector de vectores `(vector<vector<Edge>>)`. Si tienes 203 nodos, crea 203 listas vacías. Cada lista guardará las aristas que salen de ese nodo.

• `potential`, `dist`, `parent_node`, `parent_edge`: Vectores de tamaño `total_nodes`. Se inicializan vacíos o en 0. Su función es servir de "pizarrón temporal" para que Dijkstra anote sus cálculos en cada iteración.

```C++
class MinCostMaxFlow {
    int n;
    vector<vector<Edge>> adj;
    vector<long long> potential;
    vector<long long> dist;
    vector<int> parent_node;
    vector<int> parent_edge;

public:
    MinCostMaxFlow(int n) : n(n), adj(n), potential(n, 0), dist(n), parent_node(n), parent_edge(n) {}
```

## 3.2. Funcion add_edge:

Cada vez que agregas una conexión en el programa, ocurren dos operaciones simultáneas:

```C++
void add_edge(int from, int to, int capacity, long long cost) {
    adj[from].push_back({to, capacity, 0, cost, (int)adj[to].size()});
    adj[to].push_back({from, 0, 0, -cost, (int)adj[from].size() - 1});
}
```
1. Línea 1 (Arista Real): En la lista del nodo from, mete una arista que va hacia `to`, con la capacidad y costo originales. Su parámetro `rev` guarda la posición actual de la lista de `to` (es decir, dónde va a caer su hermana).
2. Línea 2 (Arista Inversa/Residual): En la lista del nodo `to`, mete automáticamente una arista de regreso hacia `from`. Ojo a los valores: su capacidad inicial es `0` (no puedes regresar agua si no ha pasado primero) y su costo es negativo (`-cost`). Su parámetro `rev` apunta a la posición de la arista real.

## 3.3. Ejecucion paso a paso del bucle principal:

Cuando llamas a mcmf.solve(source, sink), el programa entra en un ciclo coordinado de tres fases que se repite hasta solucionar el catering completo.

### Fase A: El Rastreo con Dijkstra

`dijkstra(source, sink)` se encarga de explorar el grafo.
1. Llena el vector dist con un número gigantesco (`INF`), excepto en `dist[source] = 0`.
2. Mete el origen a una cola de prioridad (`priority_queue`). La cola siempre te da el nodo con la distancia acumulada más barata.
3. Saca un nodo u de la cola y revisa todas sus aristas en `adj[u]`.
4. Para cada arista, verifica una condición: `if (e.capacity - e.flow > 0)`. Esto significa: "¿Todavía queda espacio libre en esta tubería?". Si la arista está llena, la ignora.
5. Calcula el costo usando el potencial: `long long reduced_cost = e.cost + potential[u] - potential[e.to];`.
6. Si la distancia para llegar a `e.to` pasando por `u` es más barata que la que ya conocía, actualiza los vectores:
	
    • `dist[e.to] = dist[u] + reduced_cost;` (Guarda el nuevo costo mínimo).
	
    • `parent_node[e.to] = u;` (Anota en el pizarrón: "Para llegar a este nodo, el camino óptimo viene desde u").
	
    • `parent_edge[e.to] = i;` (Anota: "Y se usó la arista número i de la lista de u").
	
    • Mete el nodo modificado a la cola de prioridad para seguir explorando desde ahí.


### Fase B: Reajuste del Tablero (Potenciales)

Si Dijkstra llegó al `sink`, el `while` continúa. Lo primero que hace es esto:

```C++
for (int i = 0; i < n; ++i) {
    if (dist[i] != INF) potential[i] += dist[i];
}
```

• Qué hace físicamente: Modifica el arreglo `potential` sumándole los resultados de `dist`. Esto altera el "voltaje" del grafo para que en la siguiente vuelta de Dijkstra, las aristas inversas (esas que tienen `-cost`) nunca tengan un valor menor a cero.

### Fase C: El Viaje en Reversa (Empujar Flujo y Cobrar)

Ahora el programa necesita aplicar los cambios en el flujo y calcular el dinero.

```C++
int path_flow = 1e9; 
for (int v = sink; v != source; v = parent_node[v]) {
    int u = parent_node[v];
    int idx = parent_edge[v];
    path_flow = min(path_flow, adj[u][idx].capacity - adj[u][idx].flow);
}
```

Qué hace: Empieza parado en el nodo `sink`. Mira su `parent_node` para ver quién lo llamó (por ejemplo, el Cliente 3). Luego mira el `parent_node` del Cliente 3 para ver quién lo llamó a él (por ejemplo, `sede_out`), y así sucesivamente hasta llegar al `source`. En este viaje de regreso, encuentra cuál era la tubería con menos espacio disponible (`path_flow`). En este problema, casi siempre dará exactamente `1`.

```C++
for (int v = sink; v != source; v = parent_node[v]) {
    int u = parent_node[v];
    int idx = parent_edge[v];
    adj[u][idx].flow += path_flow;
    adj[v][adj[u][idx].rev].flow -= path_flow;
    min_cost += (long long)path_flow * adj[u][idx].cost;
}
```

Qué hace: Vuelve a hacer el mismo viaje físico del sink al source, pero ahora modificando los valores reales:

• `adj[u][idx].flow += path_flow;`: Suma `1` al flujo de la arista real (la tubería se va llenando).

• `adj[v][adj[u][idx].rev].flow -= path_flow;`: Al usar el índice `.rev`, el programa salta instantáneamente al vector del nodo vecino y le resta `1` al flujo de la arista inversa. Al restarle flujo a una arista inversa cuya capacidad era `0`, su capacidad residual (`capacity - flow`) se convierte en `0 - (-1) = 1`. ¡Físicamente abriste una tubería de retorno por si necesitas cancelar esta ruta en el futuro!

• `min_cost += ...`: Multiplica el `path_flow (1)` por el costo original de la arista y lo suma a la alcancía.

Al terminar el recorrido de reversa, el programa vuelve a la Fase A para intentar buscar otro camino. Cuando Dijkstra ya no encuentra ninguna ruta libre hacia el `sink`, el bucle se rompe y el programa simplemente imprime el valor final acumulado en `min_cost`.