# Problema E
## Evolution in Parallel by GoogleAI (Greedy)
### Time limit: 2 Segundos

# 1. Enfoque:

El enfoque óptimo es una combinación de ordenamiento Greedy (voraz) y validación de subsecuencias lineales.

## 1.1. Enfoque de Solución:

El problema te pide determinar si un conjunto de cadenas de ADN (cadenas de caracteres) se puede dividir en exactamente dos líneas evolutivas independientes (dos caminos/cadenas principales) que parten de una cadena origen o raíz (la cadena más larga dada en la entrada). Una cadena puede evolucionar a otra si la segunda es una subsecuencia de la primera. Esto se modela formalmente como una cobertura de vértices por dos caminos en un Gráfico Acíclico Dirigido (DAG).
La estrategia paso a paso es la siguiente:

### 1. Ordenar por longitud (Greedy):

Ordena todas las cadenas de la entrada de mayor a menor longitud. La cadena más larga actúa como nuestro "ancestro común" y las dos líneas evolutivas comenzarán conteniendo esta cadena.

### 2. Asignación voraz a dos líneas de evolución:

Mantén el estado de las últimas cadenas añadidas a cada una de las dos líneas evolutivas (llamémoslas $L_{1}$ y $L_{2}$). Inicialmente, ambas apuntan a la cadena raíz (la más larga).
Al iterar sobre el resto de las cadenas ya ordenadas, para cada cadena actual $S$:
	
• Verifica si $S$ es subsecuencia de $L_{1}$ y si es subsecuencia de $L_{2}$.
	
• Si solo es subsecuencia de una: asígnala obligatoriamente a esa línea y actualiza el extremo de dicha línea a $S$.
	
• Si es subsecuencia de ambas (el caso crítico): el enfoque greedy correcto dicta asignarla a aquella línea cuyo extremo actual sea más corto. ¿Por qué? Porque una línea evolutiva con un extremo más corto es más "restrictiva" (tiene menos caracteres para emparejar en el futuro). Mantener la otra línea lo más larga posible maximiza las posibilidades de encajar futuras cadenas.
	
• Si no es subsecuencia de ninguna: el problema no tiene solución y se imprime impossible.

### 3. Verificación de subsecuencias eficiente:

Dado que realizarás muchas comprobaciones de subsecuencias, puedes implementar una función de dos punteros estándar que corre en tiempo $O(\vert{}A\vert{} + \vert{}B\vert{})$ para verificar si $B$ es subsecuencia de $A$. Con $N \le 4000$, esta estrategia se ejecuta cómodamente dentro del límite de tiempo oficial.

# 2. Implementacion:


## 2.1. El motor de búsqueda (is_subsequence)

```C++
bool is_subsequence(const string& main_str, const string& sub) {
    if (sub.length() > main_str.length()) return false;
```

• Qué hace: Define la función de validación y descarta de inmediato si la cadena buscada (sub) es más larga que la principal.

• Por qué: Si es más larga, es matemáticamente imposible que esté contenida en ella; ahorrar este cálculo acelera el programa.

```C++
    size_t i = 0, j = 0;
    while (i < main_str.length() && j < sub.length()) {
        if (main_str[i] == sub[j]) {
            j++;
        }
        i++;
    }
    return j == sub.length();
}
```

• Qué hace: Avanza el puntero i por toda la cadena grande, y el puntero j solo avanza cuando encuentra la letra correcta en orden. Al final, verifica si j llegó al final de la cadena corta.

• Por qué: Es el método óptimo de dos punteros de complejidad lineal \(O(L)\). Como el juez nos da cadenas de hasta 4000 caracteres, necesitamos que cada validación tome milisegundos.

## 2.2. Configuración y Lectura de Datos

```C++
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
```

• Qué hace: Inicia el programa principal y desactiva la sincronización de los flujos de lectura/escritura estándar de C++.

• Por qué: Optimiza la velocidad de entrada y salida de datos a nivel de hardware, evitando recibir un Time Limit Exceeded (TLE) al leer archivos masivos.

```C++
    int n;
    if (!(cin >> n)) return 0;
    string root;
    if (!(cin >> root)) return 0;
```

• Qué hace: Lee la cantidad de mutaciones intermedias (n) y la cadena más larga que representa el ancestro común original (root).

• Por qué: Establece las dimensiones del problema y captura la base sobre la cual se construirán las dos líneas evolutivas.

```C++
    vector<string> DNA(n);
    for (int i = 0; i < n; i++) {
        if (!(cin >> DNA[i])) return 0;
    }
```

• Qué hace: Declara un arreglo dinámico de tamaño n y lee todas las mutaciones intermedias, asegurando que si la lectura falla, el programa termine limpiamente.

• Por qué: Almacena temporalmente todas las piezas de ADN antes de empezar a clasificarlas.

## 2.3. Ordenamiento Estratégico y Filtro de Seguridad

```C++
    sort(DNA.begin(), DNA.end(), [](const string& a, const string& b) {
        return a.length() > b.length();
    });
```

• Qué hace: Ordena todo el vector DNA de mayor a menor longitud usando una función lambda.

• Por qué: Es el núcleo de la estrategia Greedy. Procesar de la cadena más larga a la más corta nos permite ir "hacia atrás" en la evolución, facilitando decisiones definitivas paso a paso.

```C++
    for (int i = 0; i < n; i++) {
        if (!is_subsequence(root, DNA[i])) {
            cout << "impossible\n";
            return 0;
        }
    }
```

• Qué hace: Compara cada mutación contra la raíz principal (root). Si encuentra una sola que no sea subsecuencia de ella, imprime impossible y cierra el programa.

• Por qué: Si una mutación intermedia tiene caracteres que no existían en el ancestro original, el árbol evolutivo es inválido desde el inicio.

## 2.4. Simulación de las Dos Líneas Evolutivas

```C++
    vector<string> path1, path2;
    string last1 = root;
    string last2 = root;
```

• Qué hace: Crea los contenedores de las dos ramas finales (path1 y path2) e inicializa sus extremos (last1 y last2) apuntando a la raíz.

• Por qué: Al principio de la historia evolutiva, ambas líneas se intersectan y nacen del mismo ancestro común más largo.

```C++
    for (int i = 0; i < n; i++) {
        bool sub1 = is_subsequence(last1, DNA[i]);
        bool sub2 = is_subsequence(last2, DNA[i]);
```

• Qué hace: Comienza a iterar el vector ordenado y evalúa si la mutación actual puede descender del último eslabón de la Rama 1 (sub1) y de la Rama 2 (sub2).

• Por qué: Necesitamos mapear las opciones de compatibilidad de la cadena actual con el estado presente de ambas ramas.

```C++
        if (!sub1 && !sub2) {
            cout << "impossible\n";
            return 0;
        }
```

• Qué hace: Si la cadena actual no puede ser subsecuencia de ninguna de las dos ramas disponibles, corta el programa.

• Por qué: Significa que la mutación quedó "huérfana" y no hay forma de encajarla en un modelo de dos líneas paralelas.

```C++
        else if (sub1 && !sub2) {
            path1.push_back(DNA[i]);
            last1 = DNA[i];
        }
        else if (!sub1 && sub2) {
            path2.push_back(DNA[i]);
            last2 = DNA[i];
        }
```

• Qué hace: Si la mutación solo es compatible con una de las dos ramas, la inserta en su respectivo vector y actualiza el extremo correspondiente (last1 o last2).

• Por qué: Son decisiones forzadas por la estructura matemática del problema; no hay otra opción donde ponerlas.

```C++
        else {
            if (is_subsequence(last1, last2)) {
                path2.push_back(DNA[i]);
                last2 = DNA[i];
            } else {
                path1.push_back(DNA[i]);
                last1 = DNA[i];
            }
        }
    }
```

• Qué hace: El caso crítico. Si la cadena cabe en ambas, verifica si el extremo de la Rama 2 está contenido en la Rama 1. Si es así, asigna la mutación a la Rama 2. De lo contrario, la asigna a la Rama 1.

• Por qué: Regla voraz óptima. Al enviar la mutación a la rama que está "contenida" (la más específica), mantenemos la otra rama lo más "ancha" y flexible posible para recibir mutaciones grandes que puedan aparecer más adelante.

## 2.5. Inversión y Formateo de Salida

```C++
    reverse(path1.begin(), path1.end());
    reverse(path2.begin(), path2.end());
```

• Qué hace: Voltea por completo el orden de los elementos dentro de ambos vectores.

• Por qué: Como procesamos todo de mayor a menor longitud, las respuestas quedaron al revés. El problema exige mostrarlas en orden cronológico (de menor a mayor longitud).

```C++
    cout << path1.size() << " " << path2.size() << "\n";
    for (const string& s : path1) cout << s << "\n";
    for (const string& s : path2) cout << s << "\n";

    return 0;
}
```

• Qué hace: Imprime los tamaños de ambas ramas en la primera línea, seguido de todas las cadenas de la Rama 1 y luego todas las de la Rama 2. Finalmente retorna 0 indicando éxito.

• Por qué: Sigue estrictamente la especificación de salida del juez en línea del ICPC para obtener el veredicto de Accepted.

# 3. Justificacion:

Para dar el cierre definitivo y entender por qué este problema se puede resolver de forma voraz (Greedy) en lugar de usar un algoritmo costoso como programación dinámica o flujo máximo, debemos analizar la justificación matemática del enfoque.
El problema formalmente nos pide una cobertura por 2 caminos (2-path cover) en un conjunto parcialmente ordenado (POSET) definido por la relación de "ser subsecuencia".

## 3.1. Justificación de ordenar por longitud (De mayor a menor)

Si una cadena $A$ es subsecuencia de $B$, la longitud de $A$ es estrictamente menor o igual que la de $B$ $\vert{}A\vert{} \le \vert{}B\vert{}$. Al ordenar de mayor a menor longitud, garantizamos una propiedad topológica:

• Ninguna cadena que procesemos más adelante podrá ser un "ancestro" (supersecuencia) de las cadenas que ya procesamos.

• Esto elimina la necesidad de mirar "hacia el futuro". Cada decisión que tomamos sobre la cadena actual es definitiva porque las cadenas restantes solo pueden ser subestructuras más pequeñas.

## 3.2. Justificación del Caso Crítico (El dilema de los dos caminos)

Cuando una cadena actual $S$ puede ser colocada tanto al final de la Línea 1 ($L_{1}$) como de la Línea 2 ($L_{2}$), tenemos una bifurcación. El algoritmo evalúa: if (is_subsequence(l1, l2)).
Esto se justifica mediante el principio de flexibilidad dominante:

• Si $L_{2}$ es subsecuencia de $L_{1}$, significa que cualquier cadena futura que pueda ser contenida por $L_{2}$, también podrá ser contenida por $L_{1}$ (por propiedad transitiva). En este escenario, el camino $L_{1}$ es "dominante" o más amplio, y el camino $L_{2}$ es más restrictivo.

• Si asignamos la cadena actual $S$ a $L_{1}$ (el camino ancho), encogemos severamente la flexibilidad de $L_{1}$. Si en el futuro llega una cadena que obligatoriamente necesitaba el espacio amplio de $L_{1}$ pero que no cabía en $L_{2}$, el algoritmo fallará erróneamente (impossible).

• Por lo tanto, la opción óptima global es asignar $S$ a $L_{2}$ (el camino restrictivo). Al hacer esto, "sacrificamos" el camino que de por sí ya era estrecho, y mantenemos intacta la flexibilidad del camino amplio ($L_{1}$) para absorber cualquier sorpresa en las siguientes iteraciones.

## 3.3. Complejidad y Optimalidad

Gracias a que el ordenamiento de longitudes fuerza que la evolución vaya en un solo sentido topológico, y que la condición is_subsequence(l1, l2) resuelve perfectamente cuál camino preserva más estados válidos para el futuro, el enfoque voraz encuentra la solución óptima (si existe) en tiempo $O(N \cdot L)$.
Esto es drásticamente más eficiente que intentar un algoritmo de Flujo Máximo / Matching Bipartito (como el Teorema de Dilworth), el cual requeriría construir un grafo midiendo subsecuencias entre todos los pares de palabras, elevando el tiempo a $O(N^2 \cdot L)$, al borde del límite permitido.