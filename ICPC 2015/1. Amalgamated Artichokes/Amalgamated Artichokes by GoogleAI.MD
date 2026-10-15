# Problema A
## Amalgamated Artichokes by GoogleAI (Algoritmo del máximo acumulado)
### Time limit: 5 Segundos

# El Concepto del Algoritmo (Cómo piensa)

En lugar de comparar todos los días contra todos los días futuros (lo que hacía tu código original con los dos bucles for), este método procesa los días de forma cronológica (del día \(1\) al \(n\)) y se hace una pregunta muy simple cada día:

"Si el precio más alto de la historia de esta acción ocurrió en el pasado, y yo vendiera hoy que el precio bajó... ¿esta sería la peor caída que hemos registrado?"

Para responder a esto, el algoritmo solo necesita recordar una sola cosa del pasado: el precio más alto visto hasta el momento (max_so_far).

# Explicación Línea por Línea del Código Optimizado

El codigo esta desglosado de la siguiente manera.

## 1. Lectura Unica del Caso

En lugar de un do-while interactivo, los jueces en línea del ICPC envían archivos de texto con múltiples casos de prueba seguidos. Colocar el cin dentro del if hace que el programa procese un caso a la vez, y se detenga elegantemente cuando el archivo de entrada se quede sin datos (Fin de Archivo / EOF).

```C++
if (cin >> p >> a >> b >> c >> d >> n) {
```

## 2. Inicialización de Variables de Control
• max_decline: Guardará la caída más grande encontrada. Empieza en 0.0 porque si los precios solo suben, la caída es cero.

• max_so_far: Guardará el pico más alto de precio registrado en el viaje.

```C++
double max_decline = 0.0;
double max_so_far = -1e9; // Un número muy pequeño como valor inicial
```

## 3. El Bucle Único (La magia del \(O(n)\))

Calculamos el precio del día actual i usando la fórmula. Nota que no lo guardamos en un arreglo. Una vez usado, el valor se descarta en la siguiente iteración, ahorrando megabytes de memoria.

```C++
for (int i = 1; i <= n; ++i) {
    double current_price = p * (sin(a * i + b) + cos(c * i + d) + 2);
```

## 4. La Lógica Codiciosa (Greedy)

• Caso Base (\(i=1\)): El primer día no hay pasado, así que el precio actual se convierte en nuestro punto de referencia más alto.

• Evaluación de Caída: Si el precio de hoy (current_price) es menor que el pico más alto (max_so_far), calculamos la diferencia. La función std::max compara esa diferencia contra la caída máxima que ya teníamos guardada y se queda con la peor de las dos.

• Actualización del Pico: Si el precio de hoy es más alto que nuestro max_so_far, significa que alcanzamos una nueva "cumbre". Actualizamos max_so_far con este nuevo precio. A partir de aquí, las futuras caídas se calcularán partiendo de este nuevo punto más alto.

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

# Resumen de mejora (Con resptecto a TanmayKedari[Fuerza Bruta])


| Característica | Codigo de Tanmay | Google AI |
| --------- | --------- | --------- |
| Algoritmo   | Fuerza Bruta | Greedy   |
| Complejidad de Tiempo   | \(O(n^2)\) (Inaceptable para el ICPC)   | \(O(n)\) (Tiempo Lineal - Ultra rápido) |
| Complejidad de Memoria   | \(O(n)\) (Crea un arreglo gigante) | \(O(1)\) (Memoria Constante - Usa pocas variables)   |
| Operaciones (\(n=10^6\)) | \(n=10^6\)) \(\approx 500,000,000,000\) operaciones   | \(\approx 1,000,000\) operaciones |
| Tiempo de ejecución   | Horas   | 0.02 segundos   |
