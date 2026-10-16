# Problema G
## Pipe Stream by GoogleAI ()
### Time limit: 2 Segundos

# 1. Enfoque del problema:

El problema te pide encontrar la estrategia que requiera la mínima cantidad de golpes en el peor de los casos para determinar la velocidad $v$ del líquido dentro de un margen de error absoluto de $t/2$ (lo que equivale a acotar la velocidad en un intervalo de longitud máxima $t$).
El enfoque detallado se estructura de la siguiente manera:

## 1.1. El caso ideal (Tubería infinita)

Si la tubería tuviera una longitud infinita, la solución óptima sería una búsqueda binaria estándar sobre el rango de velocidades $[v_1, v_2]$. En cada golpe, elegirías un punto que divida el intervalo actual a la mitad. La cantidad de golpes necesarios sería exactamente $\lceil \log_2(\frac{v_2 - v_1}{t}) \rceil$.

## 1.2. La restricción del mundo real (Longitud finita $L$)

Debido a que la tubería mide exactamente $L$ metros, no puedes realizar mediciones indefinidamente. Un golpe en la posición \(x\) al tiempo \(\tau \) te dice si \(v \ge x/\tau\). Sin embargo, estás limitado a que $x \le L$.

• Si intentas medir una velocidad muy alta, requerirás un tiempo $\tau$ muy pequeño para que $x = v \cdot \tau \le L$.

• Dado que el primer golpe debe ser en $\tau \ge s$ y los golpes subsecuentes deben espaciarse al menos $s$ segundos, el tiempo avanza inevitablemente. Si el líquido viaja demasiado rápido, llegará al final de la tubería ($L$) antes de que tengas tiempo de realizar los golpes restantes necesarios para subdividir el rango de velocidades altas. Una vez que el fluido sale de la tubería, ya no se puede distinguir qué tan rápido iba por encima de ese límite.

## 1.3. Estrategia Codiciosa (Greedy)

Para maximizar la cantidad de información y el tiempo disponible, siempre es óptimo realizar los golpes lo antes posible. Esto significa que el $k-ésimo$ golpe de tu estrategia debe ocurrir exactamente en el segundo $\tau = k \cdot s$.

## 1.4. Enfoque con Programación Dinámica (Árbol de Decisión)

El problema puede modelarse al revés utilizando programación dinámica para calcular el rango máximo de velocidades que se puede resolver con $k$ golpes restantes.
Imagina el árbol de decisión donde cada hoja representa un intervalo válido de tamaño $\le t$:

• Estado: Define una función de DP o recursión con memoización que determine si es posible cubrir un rango de velocidades con un número determinado de golpes $k$, tomando en cuenta que para las velocidades más altas, el tiempo límite para realizar mediciones válidas es $\tau_{max} = L/v$.

• Transición: En cada paso, un golpe define un umbral de velocidad $v_{mid}$. Este golpe divide tu rango en dos: las velocidades menores a $v_{mid}$ (donde el líquido aún no llega al punto del golpe) y las velocidades mayores o iguales a $v_{mid}$. El subárbol derecho (velocidades altas) estará más severamente restringido por el tiempo, limitando el número de golpes futuros que puedes hacer antes de que el líquido escape de la tubería.

• Construcción de intervalos: De manera óptima, querrás que los intervalos de las hojas del árbol de decisión (que cubren las velocidades más altas) se ajusten perfectamente de derecha a izquierda. El intervalo más a la derecha terminará exactamente en $v_{2}$, teniendo un tamaño máximo de $t$ (es decir, $[v_2-t, v_2]$). A partir de ahí, puedes calcular cuál es la velocidad máxima que puede permitir los golpes restantes y usarla como límite para el siguiente intervalo, moviéndote de forma inductiva hacia $v_{1}$.

## 1.5. Resumen del Algoritmo

1. Incrementa el número total de golpes permitidos $K$ empezando desde $\lceil \log_2(\frac{v_2 - v_1}{t}) \rceil$.
2. Para un $K$ fijo, usa la DP (o una función recursiva de verificación) para calcular inductivamente el límite inferior de velocidad que puedes discernir con $K$ golpes en total, asumiendo que los intervalos de incertidumbre miden exactamente $t$ y se acomodan de forma (greedy) desde las velocidades más altas a las más bajas.
3. El primer $K$ que logre cubrir exitosamente todo el rango desde $v_{2}$ hasta $v_{1}$ (o menos) será tu respuesta óptima. Si ningún valor de $K$ puede lograrlo debido a que el líquido se escapa demasiado rápido (por ejemplo, si $v_2 > L/s$), el problema se vuelve imposible.