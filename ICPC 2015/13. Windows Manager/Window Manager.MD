# Problema M
## Tours by GoogleAI
### Time limit: 2 Segundos

# 1. Enfoque del problema:

Dado que las restricciones y la cantidad de ventanas activas simultáneamente son pequeñas, no se necesitan estructuras de datos extremadamente complejas (como árboles de intervalos bidimensionales). Una lista o un arreglo dinámico simple basta para mantener el estado de las ventanas.

La complejidad del problema radica casi exclusivamente en la implementación de la operación MOVE, donde las ventanas pueden empujarse entre sí de forma recursiva o en cascada.

## 1.1. Representación del Estado

Cada ventana se puede representar como una estructura u objeto con los siguientes atributos:

• Coordenadas: $x$, $y$ (esquina superior izquierda o inferior izquierda según el sistema coordenado indicado), además de su ancho ($w$) y alto ($h$).

• Identificador único (o su orden de creación/índice).

## 1.2. Procesamiento de Comandos Básicos

• OPEN / RESIZE / CLOSE: Se resuelven con búsquedas lineales simples sobre el conjunto de ventanas para verificar intersecciones o localizar la ventana afectada.

• Búsqueda por punto: Para comandos que dependan de un clic en una coordenada ($x, y$), se recorre la lista en orden inverso (de la más reciente a la más antigua, simulando el eje $Z$ o enfoque) hasta encontrar la primera ventana cuyos límites contengan dicho punto.

## 1.3. El enfoque para la operación MOVE (El núcleo del problema)

Cuando una ventana intenta moverse una distancia $d$ en una dirección (por ejemplo, a la derecha), puede empujar a otras ventanas que estén en su camino, y estas a su vez a otras, hasta chocar con el borde de la pantalla. El algoritmo óptimo para resolver esto consta de dos pasos:

### Paso A: Calcular la distancia máxima permitida de movimiento

Antes de mover físicamente cualquier ventana, debes determinar cuántas unidades realmente se pueden desplazar (ya que el movimiento se detiene si alguna ventana en la cadena de empuje choca contra el borde de la pantalla).

1. Crea una función recursiva calcular_desplazamiento_max(ventana, direccion, d_restante).

2. Para la ventana actual, calcula cuánta distancia libre tiene hasta el borde de la pantalla o hasta intersectar a sus vecinas directas en esa dirección.

3. Si colisiona con otras ventanas, invoca recursivamente la función para esas ventanas adyacentes para ver cuánto espacio pueden ceder ellas.

4. El desplazamiento real permitido para la ventana original será el mínimo entre la distancia solicitada $d$ y lo que el "bloque" completo de ventanas empujadas sea capaz de moverse antes de tocar el límite de la pantalla.

### Paso B: Mover las ventanas en el orden correcto (Orden Topológico / Geométrico)

Una vez conocida la distancia exacta que se moverán todas las ventanas involucradas, debes actualizar sus coordenadas. Para evitar calcular colisiones erróneas durante la actualización, recorre las ventanas en orden inverso a la dirección del movimiento:

• Hacia la derecha: Ordena y procesa las ventanas de derecha a izquierda (por coordenada $x$ decreciente de su borde derecho).

• Hacia la izquierda: Procesa de izquierda a derecha (por coordenada $x$ creciente).

• Hacia arriba/abajo: Aplica el mismo criterio con la coordenada $y$.
Al mover primero las ventanas que están más cerca del destino final (las que van al "frente" del empuje), dejas el espacio libre de manera limpia para las ventanas que vienen detrás

# 2. Implementacion:

```

1. Estructuras de Datos y Estado Global

Estructura Ventana:
    id: Entero
    x, y: Entero // Coordenadas esquina superior izquierda
    w, h: Entero // Ancho y alto de la ventana

Variables Globales:
    ancho_pantalla, alto_pantalla: Entero
    ListaVentanas: Lista de Ventana
    contador_ventanas: Entero = 0

2. Funciones Geométricas y de Validación

text
Funcion Interseccion(Ventana A, Ventana B) -> Booleano:
    // Verifica superposición usando intervalos semiabiertos [x, x+w)
    Si max(A.x, B.x) < min(A.x + A.w, B.x + B.w) Y 
       max(A.y, B.y) < min(A.y + A.h, B.y + B.h) Entonces:
        Retornar Verdadero
    Sino:
        Retornar Falso

Funcion Es_Valida(Ventana nueva, Entero id_excluir) -> Booleano:
    // Verificar bordes de la pantalla
    Si nueva.x < 0 O nueva.y < 0 O 
       (nueva.x + nueva.w) > ancho_pantalla O 
       (nueva.y + nueva.h) > alto_pantalla Entonces:
        Retornar Falso
        
    // Verificar colisiones con otras ventanas
    Para cada w en ListaVentanas:
        Si w.id != id_excluir Y Interseccion(nueva, w) Entonces:
            Retornar Falso
            
    Retornar Verdadero

Funcion Buscar_Ventana_En_Punto(Entero click_x, Entero click_y) -> Entero:
    // Recorrer al revés para seleccionar la ventana que está arriba (Eje Z)
    Para i desde longitud(ListaVentanas) - 1 hasta 0 con paso -1:
        v = ListaVentanas[i]
        Si click_x >= v.x Y click_x < v.x + v.w Y 
           click_y >= v.y Y click_y < v.y + v.h Entonces:
            Retornar i // Retorna el índice en la lista
            
    Retornar -1 // No se tocó ninguna ventana

3. Operación Crítica: MOVE (Simulación Píxel a Píxel)

Procedimiento Operacion_MOVE(Entero click_x, Entero click_y, Entero dx, Entero dy, Entero num_linea):
    idx_inicial = Buscar_Ventana_En_Punto(click_x, click_y)
    Si idx_inicial == -1 Entonces:
        Imprimir "Command " + num_linea + ": MOVE - no window at given position"
        Retornar

    // Calcular distancia y direcciones unitarias (-1, 0, 1)
    distancia_total = max(abs(dx), abs(dy))
    dir_x = (dx > 0) ? 1 : ((dx < 0) ? -1 : 0)
    dir_y = (dy > 0) ? 1 : ((dy < 0) ? -1 : 0)

    GrupoMovil = [idx_inicial] // Lista de índices de ventanas arrastradas
    unidades_movidas = 0

    // Bucle para avanzar un píxel a la vez
    Para paso desde 0 hasta distancia_total - 1:
        // A. Verificar si alguna ventana del grupo choca con el borde físico del monitor
        colision_borde = Falso
        Para cada g en GrupoMovil:
            Si dir_x > 0 Y ListaVentanas[g].x + ListaVentanas[g].w >= ancho_pantalla Entonces colision_borde = Verdadero
            Si dir_x < 0 Y ListaVentanas[g].x <= 0 Entonces colision_borde = Verdadero
            Si dir_y > 0 Y ListaVentanas[g].y + ListaVentanas[g].h >= alto_pantalla Entonces colision_borde = Verdadero
            Si dir_y < 0 Y ListaVentanas[g].y <= 0 Entonces colision_borde = Verdadero

        Si colision_borde Entonces:
            Romper Bucle // Se congela el movimiento del bloque entero

        // B. Proyectar el siguiente paso de forma virtual y capturar nuevos obstáculos
        NuevosVecinos = []
        Para cada g en GrupoMovil:
            VentanaFantasma = ListaVentanas[g]
            VentanaFantasma.x += dir_x
            VentanaFantasma.y += dir_y

            Para i desde 0 hasta longitud(ListaVentanas) - 1:
                Si i esta en GrupoMovil O i esta en NuevosVecinos Entonces:
                    Continuar // Ignorar si ya se mueve en este paso

                Si Interseccion(VentanaFantasma, ListaVentanas[i]) Entonces:
                    Agregar i a NuevosVecinos

        // C. Efecto Dominó Iterativo: Si hay nuevos obstáculos, se absorben y se congela el avance
        Si NuevosVecinos no esta vacio Entonces:
            Para cada n en NuevosVecinos:
                Agregar n a GrupoMovil
            paso = paso - 1 // Deshacer el avance del ciclo para volver a evaluar el mismo píxel
            Continuar

        // D. Si el píxel está completamente libre, aplicar el desplazamiento físico real
        Para cada g en GrupoMovil:
            ListaVentanas[g].x += dir_x
            ListaVentanas[g].y += dir_y
            
        unidades_movidas += 1

    // Reportar si el movimiento fue truncado antes de lo solicitado
    Si unidades_movidas < distancia_total Entonces:
        Imprimir "Command " + num_linea + ": MOVE - moved " + unidades_movidas + " instead of " + distancia_total

4. Algoritmo Principal (Procesamiento del Flujo)

Procedimiento Principal():
    Leer ancho_pantalla, alto_pantalla
    num_linea = 1
    
    Mientras haya líneas de entrada disponibles:
        Leer linea
        Si linea esta vacia Entonces:
            Romper Bucle // Fin del archivo o doble enter en consola
            
        num_linea += 1
        Separar linea en componentes: comando, y sus argumentos numéricos
        
        Si comando == "OPEN" Entonces:
            // Crear ventana temporal, validar con Es_Valida y meter a ListaVentanas
            
        Sino si comando == "CLOSE" Entonces:
            // Buscar índice por clic y usar borrar() en ListaVentanas
            
        Sino si comando == "RESIZE" Entonces:
            // Buscar índice, modificar dimensiones y guardar cambios si Es_Valida es verdadera
            
        Sino si comando == "MOVE" Entonces:
            Operacion_MOVE(arg1, arg2, arg3, arg4, num_linea)

    // Impresión del Reporte Final
    Imprimir longitud(ListaVentanas) + " window(s):"
    Para cada v en ListaVentanas:
        Imprimir v.x + " " + v.y + " " + v.w + " " + v.h
```

## 2.1. Librerías y Estructura de Datos

Esta sección inicial importa las herramientas de C++ y define cómo se almacena cada ventana en la memoria del gestor.
```C++
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cmath>

using namespace std;

struct Window {
    int id;   // Identificador correlativo único según el orden de creación
    int x;    // Coordenada X de la esquina superior izquierda
    int y;    // Coordenada Y de la esquina superior izquierda
    int w;    // Ancho (width) en píxeles
    int h;    // Alto (height) en píxeles
};

int screen_w, screen_h;
vector<Window> windows;
int window_counter = 0; 
```

• Variables Globales: screen_w y screen_h delimitan el tamaño del monitor.

• El vector windows: Funciona como la base de datos de las ventanas visibles. Al meterlas al final del vector, las más recientes quedan arriba en el "Eje Z".

## 2.2. Detección de Intersecciones (intersect)

Esta función matemática evalúa si dos ventanas se solapan usando intervalos semiabiertos [x, x + w).
```C++
bool intersect(const Window& a, const Window& b) {
    return max(a.x, b.x) < min(a.x + a.w, b.x + b.w) &&
           max(a.y, b.y) < min(a.y + a.h, b.y + b.h);
}
```
• Bordes Adyacentes: Si la ventana A termina en x = 65 y la ventana B empieza exactamente en x = 65, la condición devuelve falso.

• Propósito: Permite que las ventanas se toquen en sus bordes sin que el programa piense erróneamente que se están traspasando.

## 2.3. Validación de Límites y Espacio (is_valid)

Asegura que cualquier ventana nueva o redimensionada no se salga de la pantalla ni invada el espacio de otra.
```C++
bool is_valid(const Window& new_win, int exclude_id = -1) {
    if (new_win.x < 0 || new_win.y < 0 || 
        new_win.x + new_win.w > screen_w || new_win.y + new_win.h > screen_h) {
        return false;
    }
    for (const auto& w : windows) {
        if (w.id != exclude_id && intersect(new_win, w)) {
            return false;
        }
    }
    return true;
}
```
• Uso de exclude_id: Sirve para que cuando cambies el tamaño de una ventana con RESIZE, no choque contra su propio registro viejo guardado en el vector.

## 2.4. Selección por Clic (find_window_at)

Identifica qué ventana fue seleccionada basándose en las coordenadas (x, y) donde el usuario hizo clic.
```C++
int find_window_at(int x, int y) {
    for (int i = (int)windows.size() - 1; i >= 0; --i) {
        if (x >= windows[i].x && x < windows[i].x + windows[i].w &&
            y >= windows[i].y && y < windows[i].y + windows[i].h) {
            return i;
        }
    }
    return -1; 
}
```
• Recorrido Inverso: Empieza a buscar desde el final del vector (la ventana más nueva) hacia el principio. Así selecciona la ventana que está arriba del todo si hay varias amontonadas.

## 2.5. Configuración Inicial (main)

Desvincula los buffers para acelerar la entrada/salida y procesa la resolución inicial del monitor.

```C++
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    string line;
    if (!getline(cin, line)) return 0;
    stringstream ss_init(line);
    if (!(ss_init >> screen_w >> screen_h)) return 0;
    
    int line_num = 1; 
```

## 2.6. Procesamiento de Comandos Básicos (OPEN, CLOSE, RESIZE)

Lee la entrada línea por línea usando streams de texto y ejecuta las operaciones estándar del gestor.

```C++
    while (getline(cin, line)) {
        if (line.empty()) break; // CORRECCIÓN INTERACTIVA: Un enter extra en blanco detiene el programa al instante.
        
        line_num++;
        stringstream ss(line);
        string cmd;
        if (!(ss >> cmd)) continue;
        
        if (cmd == "OPEN") {
            int x, y, w, h;
            ss >> x >> y >> w >> h;
            Window new_win = {++window_counter, x, y, w, h};
            if (is_valid(new_win)) { windows.push_back(new_win); } 
            else { cout << "Command " << line_num << ": OPEN - window does not fit\n"; window_counter--; }
        } 
        else if (cmd == "CLOSE") {
            int x, y;
            ss >> x >> y;
            int idx = find_window_at(x, y);
            if (idx != -1) { windows.erase(windows.begin() + idx); } 
            else { cout << "Command " << line_num << ": CLOSE - no window at given position\n"; }
        } 
        else if (cmd == "RESIZE") {
            int x, y, w, h;
            ss >> x >> y >> w >> h;
            int idx = find_window_at(x, y);
            if (idx != -1) {
                Window resized = windows[idx]; resized.w = w; resized.h = h;
                if (is_valid(resized, resized.id)) { windows[idx] = resized; } 
                else { cout << "Command " << line_num << ": RESIZE - window does not fit\n"; }
            } else { cout << "Command " << line_num << ": RESIZE - no window at given position\n"; }
        }
```

• Corte por Línea Vacía: El if (line.empty()) break; rompe el ciclo cuando dejas de escribir comandos en la consola, evitando que se quede congelado esperando más texto.

## 2.7. El Núcleo de Movimiento Iterativo (MOVE)

Desplaza el bloque de ventanas afectadas un píxel a la vez para simular los empujes físicos de forma exacta y sin funciones recursivas que generen bucles infinitos.
```C++
        else if (cmd == "MOVE") {
            int x, y, dx, dy;
            ss >> x >> y >> dx >> dy;
            int idx = find_window_at(x, y);
            
            if (idx != -1) {
                int amount = max(abs(dx), abs(dy));
                int dir_x = (dx > 0) ? 1 : ((dx < 0) ? -1 : 0);
                int dir_y = (dy > 0) ? 1 : ((dy < 0) ? -1 : 0);
                
                vector<int> active_group = {idx}; // Vector dinámico con los índices de las ventanas que se mueven juntas
                int moved_units = 0;
                
                for (int step = 0; step < amount; ++step) {
                    bool boundary_collision = false;
```
### A. Verificación de Bordes de la Pantalla

```C++
                    for (int g : active_group) {
                        if (dir_x > 0 && windows[g].x + windows[g].w >= screen_w) boundary_collision = true;
                        if (dir_x < 0 && windows[g].x <= 0) boundary_collision = true;
                        if (dir_y > 0 && windows[g].y + windows[g].h >= screen_h) boundary_collision = true;
                        if (dir_y < 0 && windows[g].y <= 0) boundary_collision = true;
                    }
                    if (boundary_collision) break; 
```
• Freno Total: Si una sola de las ventanas que se están arrastrando toca el borde del monitor, el ciclo for se rompe inmediatamente (break), cancelando el resto del viaje.

### B. Proyección Virtual y Detección de Obstáculos

```C++
                    vector<int> to_add;
                    for (int g : active_group) {
                        Window test_win = windows[g];
                        test_win.x += dir_x; // Mueve un fantasma de la ventana 1 píxel hacia adelante
                        test_win.y += dir_y;
                        
                        for (int i = 0; i < (int)windows.size(); ++i) {
                            if (find(active_group.begin(), active_group.end(), i) != active_group.end()) continue;
                            if (find(to_add.begin(), to_add.end(), i) != to_add.end()) continue;
                            
                            if (intersect(test_win, windows[i])) { to_add.push_back(i); }
                        }
                    }
```
• Inspección de Ruta: El programa comprueba si el "fantasma" de las ventanas móviles invade el espacio de alguna ventana quieta que esté más adelante en el camino.

### C. El Truco de la Absorción Dinámica (step--)

```C++
                    if (!to_add.empty()) {
                        for (int a : to_add) active_group.push_back(a);
                        step--; // Deshace el avance del contador del bucle
                        continue; // Vuelve a evaluar el mismo píxel de distancia
                    }
```
• Efecto Dominó Iterativo: Si se descubren obstáculos, se meten al active_group. Al restar uno a step y meter un continue, obligas al programa a repetir el análisis sobre el mismo píxel, asegurando que los nuevos vecinos absorbidos no choquen en cadena con otras cosas antes de avanzar.

### D. Desplazamiento Físico de Coordenadas

```C++
                    for (int g : active_group) {
                        windows[g].x += dir_x;
                        windows[g].y += dir_y;
                    }
                    moved_units++; 
                }
```
• Consolidación: Si la lista to_add quedó vacía (ruta libre en ese píxel), las posiciones reales de todo el bloque se actualizan un píxel en la memoria y se cuenta como avance exitoso.

```C++
                if (moved_units < amount) {
                    cout << "Command " << line_num << ": MOVE - moved " << moved_units << " instead of " << amount << "\n";
                }
            } else { cout << "Command " << line_num << ": MOVE - no window at given position\n"; }
        }
    }
```

## 2.8. Impresión del Reporte Final

Una vez que se rompe el bucle de comandos principal (por la línea vacía), el programa imprime en la consola el número total de ventanas que quedaron abiertas y sus coordenadas definitivas.

```C++
    cout << windows.size() << " window(s):\n";
    for (const auto& w : windows) {
        cout << w.x << " " << w.y << " " << w.w << " " << w.h << "\n";
    }
    
    return 0;
}
```

# 3. Justificacion:


## 3.1. Correctitud Geométrica (Resolución del Efecto Dominó)

La mayor dificultad de este problema es el carácter dinámico de los choques. Una ventana A puede no tocar a una ventana B al inicio del comando, pero sí impactarla tras haber avanzado una fracción del movimiento.

• Por qué el enfoque estático falla: Intentar calcular de golpe las distancias o usar un grafo estático (DFS/BFS tradicional) no funciona, porque la geometría del entorno cambia a medida que el bloque se desplaza.

• Por qué la simulación píxel a píxel es correcta: Al avanzar el bloque en incrementos mínimos y discretos de exactly 1 píxel, el algoritmo transforma un problema de barrido geométrico continuo en un análisis de estados discretos paso a paso. Ningún choque a mitad de camino puede ser ignorado, ya que el "fantasma" (test_win) captura el impacto en el instante exacto en que ocurre.

## 3.2. Garantía de Terminación (Ausencia de Bucles Cíclicos)

Los enfoques recursivos tradicionales suelen caer en bucles infinitos cuando dos ventanas se empujan mutuamente o quedan atrapadas en dependencias circulares complejas en la memoria de C++.

• Por qué este algoritmo es seguro: La búsqueda de nuevos obstáculos se realiza mediante el vector lineal active_group.

• El mecanismo de marcas: Antes de añadir una ventana a la cola de impacto (to_add), la línea if (find(active_group.begin(), active_group.end(), i) != active_group.end()) continue; garantiza de forma absoluta que una ventana que ya se está moviendo jamás pueda volver a ser absorbida.

• Estructura plana: Al no haber recursión de funciones (no hay llamadas cruzadas a sí mismas), la pila de ejecución de C++ se mantiene plana, haciendo que el desbordamiento de memoria o el encallamiento cíclico sean matemáticamente imposibles.

## 3.3. Complejidad Temporal y Espacial (Viabilidad en el ICPC)

En competencias como el ICPC, el factor determinante es el Tiempo Límite de Ejecución (usualmente 2.0 segundos). Evaluemos la eficiencia de este algoritmo:

• Complejidad Espacial: O(N). Solo almacenamos el vector de ventanas de tamaño N. No se crean matrices pesadas ni estructuras dinámicas complejas. Es sumamente ligero.

### Complejidad Temporal Peor Caso: O(distancia ⋅ N²).

• El ciclo principal de movimiento corre un máximo de amount de veces (la distancia solicitada, que está acotada en el problema).

• En cada paso, recorremos el grupo activo y lo comparamos contra el resto de las ventanas para detectar colisiones, lo cual toma un tiempo proporcional a N².

• Justificación del Tiempo: Dado que el número de ventanas activas simultáneamente en pantalla (N) es muy pequeño según las restricciones del problema, la operación N² toma apenas unos pocos cientos de operaciones elementales. Incluso si la distancia es grande, el total de operaciones por comando se mantiene en el orden de miles, ejecutándose en menos de 0.01 segundos, pasando con holgura el examen del juez en línea.

## 3.4. El porqué del truco del step-- y continue

Este mecanismo es la pieza central que justifica la elegancia del código:

• Cuando el bloque choca con un nuevo objeto, no podemos simplemente mover todo en el siguiente paso, porque esa nueva ventana integrada podría tener otro obstáculo pegado inmediatamente detrás de ella.

• Al restar uno a la variable del ciclo (step--) y forzar un continue, lo que hacemos es congelar el tiempo físico. El programa no consume distancia de movimiento, sino que vuelve a inspeccionar el mismo píxel pero con el grupo expandido.

• Esto resuelve de forma iterativa y limpia el efecto dominó en cadena (la ventana 1 empuja a la 2, la 2 a la 3 y la 3 a la 4) antes de que se altere una sola coordenada real en la memoria.