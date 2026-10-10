# Problema A
## Amalgamated Artichokes by GoogleAI (Algoritmo de Kadane)
### Time limit: 5 Segundos

# El Concepto del Algoritmo (Cómo piensa)

Mientras que el enfoque Greedy miraba hacia atrás buscando el "pico más alto de la historia", la Programación Dinámica piensa de forma incremental, un paso a la vez. Se enfoca en la tendencia inmediata.
La pregunta que se hace la Programación Dinámica cada día es:
"Si yo sé cuál era la mayor caída acumulada que venía sufriendo el precio justo hasta el día de ayer... ¿el cambio de precio de hoy ayuda a que esa caída sea aún peor, o el precio subió tanto que la racha de caídas se ha roto por completo?"
Para lograr esto, el algoritmo utiliza una adaptación del Algoritmo de Kadane (un método clásico de DP para encontrar el subarreglo de suma máxima). En lugar de sumar ganancias, nosotros vamos acumulando pérdidas consecutivas.

## La Ecuacion de recurrencia:


La Ecuación de Recurrencia (El núcleo matemático)

En Programación Dinámica, la lógica se define mediante una fórmula matemática llamada relación de recurrencia. Para este problema es:

$DP[i]= max (0.0,DP[i-1] + Cambio\ de\ hoy)$

Donde:

• $DP[i]$: Es la máxima caída de precio acumulada que termina exactamente en el día $i$.

• $DP[i-1]$: Es la caída acumulada que traíamos hasta el día anterior.

• $Cambio\ de\ hoy$: Es la resta de $(Precio\ Anterior - Precio\ Actual)$. Si el precio bajó, este número es positivo (sumamos pérdida). Si el precio subió, este número es negativo (restamos pérdida).

¿Por qué el $max(0.0, ...)$?

Si el precio sube con demasiada fuerza, el cálculo $DP[i-1] + Cambio$ se volverá negativo. La función max descarta ese valor negativo y lo convierte en 0.0. Esto significa matemáticamente: "La racha de caídas anterior se ha roto; el precio actual es tan alto que cualquier caída futura debe empezar a contarse desde cero a partir de hoy".

# Explicación Línea por Línea del Código Optimizado

El codigo esta desglosado de la siguiente manera.

## 1. Inicializacion y caso base

• `precio_anterior`: Calculamos el precio del Día 1 para tener un punto de partida.

• `dp_caida_actual`: Es nuestra variable de estado DP. En el Día 1, la caída acumulada es obligatoriamente 0.0 porque no hay un pasado contra qué comparar.

• `max_caida_global`: Guardará la respuesta final. Empezo en 0.0.

```C++
double precio_anterior = p * (sin(a * 1 + b) + cos(c * 1 + d) + 2.0);
double dp_caida_actual = 0.0; 
double max_caida_global = 0.0; 
```

## 2. El Ciclo de Transición (De 2 hasta n)

Calculamos el precio del día de hoy (precio_actual) y medimos el cambio respecto a ayer. Si ayer valía 10 y hoy vale 7, el cambio es +3 (perdimos valor, lo cual es "bueno" porque buscamos una caída).

```C++
for (long long i = 2; i <= n; ++i) {
    double precio_actual = p * (sin(a * i + b) + cos(c * i + d) + 2.0);
    double cambio = precio_anterior - precio_actual; 
```

## 3. Aplicación de la Recurrencia

Aquí ocurre la magia de la Programación Dinámica. Tomamos la caída que ya traíamos acumulada de días anteriores (dp_caida_actual), le sumamos el cambio de hoy y verificamos con max que no se haya vuelto negativa. El resultado se sobreescribe en la misma variable.

```C++
dp_caida_actual = max(0.0, dp_caida_actual + cambio);
```

## 4. Actualización del Máximo Global y Preparación del Siguiente Paso

• `max_caida_global`: Compara si la caída acumulada que terminó hoy es la más grande detectada en todo el historial del programa.

• `precio_anterior` = precio_actual: El precio de hoy se convierte en el "precio de ayer" para la siguiente iteración del ciclo. Al hacer esto, ahorramos memoria O(1), ya que no necesitamos un arreglo gigante para guardar los precios de cada día.

```C++
max_caida_global = max(max_caida_global, dp_caida_actual);
precio_anterior = precio_actual;
```

# Resumen de mejora (Con resptecto a GoogleIA[Greedy])


| Característica | Google AI (DP) | Google AI (Greedy) |
| --------- | --------- | --------- |
| Algoritmo   | Programacion Dinamica | Greedy   |
| Complejidad de Tiempo   | \(O(n)\) (Tiempo Lineal - Ultra rápido) | \(O(n)\) (Tiempo Lineal - Ultra rápido) |
| Complejidad de Memoria   | \(O(1)\) (Memoria Constante - Usa pocas variables) | \(O(1)\) (Memoria Constante - Usa pocas variables)   |
| Operaciones (\(n=10^6\)) | \(approx 1,000,000\) operaciones | \(approx 1,000,000\) operaciones |
| Tiempo de ejecución   | 0.02 segundos | 0.02 segundos |
| Enfoque   | En la Tendencia Acumulada del paso inmediato anterior. | En buscar el Pico Máximo Histórico del pasado. |
