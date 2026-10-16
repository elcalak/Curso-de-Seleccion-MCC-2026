# Problema B
## Asteroids by GoogleAI (Enfoque Geométrico + Interpolación)
### Time limit: 2 Segundos

# 1. Analisis Matematico:

El problema requiere de un analisis matematico antes de empezar:

## 1.1. Velocidad relativa y simplificacion del universo:

Mover dos objetos a la vez vuelve locas a las ecuaciones. En física y geometría, el primer paso inteligente es pararse encima de uno de los objetos.
Si el Asteroide 1 se mueve a velocidad $\vec{V}_1 = (vx_1, vy_1)$ y el Asteroide 2 a $\vec{V}_2 = (vx_2, vy_2)$, podemos decretar que el Asteroide 1 está completamente quieto. Para compensar esto, le sumamos la velocidad opuesta al Asteroide 2. Su velocidad relativa será:

$\vec{V}_{rel}=\vec{V}_{2}-\vec{V}_{1}=(vx_{2}-vx_{1},vy_{2}-vy_{1})$

A partir de este momento, los vértices del Asteroide 1 se quedan fijos en sus coordenadas iniciales, y el Asteroide 2 se desplaza en línea recta usando $\vec{V}_{rel}$.

## 1.2. Cambios en la geometria:

Un "evento" es el instante exacto en que la estructura de la intersección cambia (un vértice entra o sale del otro polígono). Esto ocurre matemáticamente cuando un vértice $P$ choca con la línea infinita que pasa por una arista $AB$.
Si el vértice pertenece al polígono en movimiento, su posición en el tiempo \(t\) es:

$P(t)=P_{0}+\vec{V}_{p}\cdot t$

Y si la arista $AB$ está en movimiento, sus extremos son:

$A(t)=A_{0}+\vec{V}_{a}\cdot t\quad \text{y}\quad B(t)=B_{0}+\vec{V}_{b}\cdot t$

Tres puntos $(P, A, B)$ son colineales (están en la misma línea) si el determinante de sus vectores es cero. Esto equivale al producto cruz:

$\text{Det}(A(t)-P(t),B(t)-P(t))=0$

Al expandir algebraicamente este producto cruz sustituyendo las ecuaciones de movimiento, se obtiene una ecuación de segundo grado respecto al tiempo $t$:

$c_{2}t^{2}+c_{1}t+c_{0}=0$

Donde:

• $c_{0}$: Es el producto cruz inicial en $t=0$. Si da 0, significa que ya empezaron chocando.

• $c_{1}$ y $c_{2}$: Son coeficientes que agrupan las combinaciones de posiciones iniciales y velocidades cruzadas (el código calcula exactamente estos componentes mediante diferencias de vectores).

Al resolver esta ecuación usando la fórmula cuadrática tradicional $t = \frac{-c_1 \pm \sqrt{c_1^2 - 4c_2c_0}}{2c_2}$, obtenemos los valores de $t$. Esos valores son nuestras alarmas. Si $t \ge 0$, significa que es un evento real que ocurrirá en el futuro.

## 1.3. Interpolacion cuadratica:

¿Por qué el área se comporta como una parábola entre evento y evento?
Geométricamente, el área de un polígono se calcula sumando determinantes (Fórmula de Shoelace). 

Como los vértices se mueven de forma lineal respecto a $t$ $(x = x_0 + vt)$, los productos en la fórmula del área multiplican una coordenada $x(t)$ por una coordenada $y(t)$:

$\text{Área}(t)\propto (x_{0}+v_{x}t)(y_{0}+v_{y}t)\propto t^{2}$

Por lo tanto, dentro de cualquier intervalo de tiempo $[t_0, t_2]$ donde no ocurran eventos, la función del área de intersección es una función cuadrática perfecta:

$f(t)=at^{2}+bt+c$

Deducir los coeficientes $(a, b, c)$ analíticamente analizando la geometría de la intersección es una pesadilla informática. El truco de los jueces del ICPC es usar interpolación polinómica de Lagrange. Evaluamos el área real en tres instantes del intervalo: el inicio $t_{0}$, la mitad $(t_{1}$, y el final $t_{2})$.
Para evitar números gigantescos que rompan la precisión flotante (double), trasladamos el tiempo haciendo que $(t_0 = 0)$. Ahora nuestras tres muestras de tiempo son:

$x_{0}=0,\quad x_{1}=\text{mitad},\quad x_{2}=\text{final}$

Resolvemos el sistema de ecuaciones para encontrar el coeficiente $a$ y $b$ de la parábola trasladada:

$a=\frac{x_{2}(y_{1}-y_{0})-x_{1}(y_{2}-y_{0})}{-x_{1}x_{2}(x_{2}-x_{1})}$ y
$b=\frac{x_{1}^{2}(y_{2}-y_{0})-x_{2}^{2}(y_{1}-y_{0})}{-x_{1}x_{2}(x_{2}-x_{1})}$

Si $a < 0$, la parábola se abre hacia abajo (tiene forma de montaña $(\cap )$), lo que significa que tiene un punto máximo. El vértice de una parábola se encuentra exactamente en:

$x_{máx}=-\frac{b}{2a}$

Cambiamos el tiempo de regreso a la escala original $(t_{máx} = x_{máx} + t_0)$. Si este tiempo cae dentro del intervalo estudiado $(t_0 \le t_{máx} \le t_2)$, calculamos el área exacta en ese instante. Si es mayor que nuestra área máxima registrada hasta ahora, ¡hemos encontrado un nuevo récord!.

## 1.4. Algoritmo de recorte (Sutherland-Hodgman):

Para medir el área real de intersección en un segundo $t$ específico, usamos el algoritmo de recorte. Matemáticamente, un polígono convexo es la intersección de múltiples semiplanos definidos por sus aristas.

Cuando enviamos una arista orientada $AB$ (en sentido antihorario), esta divide el universo en dos: el lado izquierdo (interior del polígono) y el lado derecho (exterior). Para saber de qué lado está un punto cualquiera $P$, calculamos el signo del producto cruz:

$\text{Signo}=(B.x-A.x)(P.y-A.y)-(B.y-A.y)(P.x-A.x)$

• Si $\text{Signo} \ge 0$: El punto está adentro o sobre la línea. Se conserva.

• Si $\text{Signo} < 0$: El punto está afuera. Se destruye.

Si al recorrer los vértices pasamos de "adentro" a "afuera" (o viceversa), calculamos analíticamente el punto exacto de intersección entre el segmento que estamos revisando y la línea infinita $AB$ usando interpolación lineal, y agregamos ese nuevo punto de corte a la silueta. Al repetir esto contra todas las paredes, la geometría limpia y calcula el polígono de choque exacto.

# 2. Implementacion en C++:

Usaremos C++ por la velocidad y legibilidad del codigo:

## Pseudo Codigo de la implementacion:

```
ESTRUCTURA Punto:
    x, y: Real

ESTRUCTURA Asteroide:
    vertices: Lista de Punto
    vx, vy: Real

// 1. Motor de viaje en el tiempo
FUNCION mover_punto(p: Punto, vx: Real, vy: Real, t: Real) -> Punto:
    RETORNAR Punto(p.x + vx * t, p.y + vy * t)

// 2. Cálculo del área de una silueta (Fórmula de Shoelace)
FUNCION calcular_area(poligono: Lista de Punto) -> Real:
    n = longitud(poligono)
    SI n < 3 ENTONCES RETORNAR 0.0
    
    suma = 0.0
    PARA i DESDE 0 HASTA n - 1:
        siguiente = (i + 1) MOD n
        suma = suma + (poligono[i].x * poligono[siguiente].y) - (poligono[siguiente].x * poligono[i].y)
        
    RETORNAR ValorAbsoluto(suma) / 2.0

// 3. Forzar orientación antihoraria para el algoritmo de recorte
PROCEDIMIENTO asegurar_sentido_antihorario(VALOR_REF vertices: Lista de Punto):
    n = longitud(vertices)
    SI n < 3 ENTONCES RETORNAR
    suma = 0.0
    PARA i DESDE 0 HASTA n - 1:
        s_idx = (i + 1) MOD n
        suma = suma + (vertices[i].x * vertices[s_idx].y) - (vertices[s_idx].x * vertices[i].y)
    SI suma < 0 ENTONCES:
        Invertir_Orden_Lista(vertices)

// 4. Algoritmo de Recorte (Sutherland-Hodgman) contra una línea infinita AB
FUNCION recortar_poligono(poligono: Lista de Punto, a: Punto, b: Punto) -> Lista de Punto:
    resultado = Nueva Lista Vacía
    n = longitud(poligono)
    SI n == 0 ENTONCES RETORNAR resultado

    PARA i DESDE 0 HASTA n - 1:
        p1 = poligono[i]
        p2 = poligono[(i + 1) MOD n]

        // Producto cruz para saber si el punto está a la izquierda (adentro >= 0) o derecha
        cross1 = (b.x - a.x) * (p1.y - a.y) - (b.y - a.y) * (p1.x - a.x)
        cross2 = (b.x - a.x) * (p2.y - a.y) - (b.y - a.y) * (p2.x - a.x)

        SI cross1 >= -0.0000001 ENTONCES:
            Añadir p1 a resultado
            
        // Si el segmento cruza la línea, calculamos el punto de intersección exacto
        SI (cross1 > 0 E cross2 < 0) O (cross1 < 0 E cross2 > 0) ENTONCES:
            t_proporcion = cross1 / (cross1 - cross2)
            p_corte = Punto(p1.x + t_proporcion * (p2.x - p1.x), p1.y + t_proporcion * (p2.y - p1.y))
            Añadir p_corte a resultado

    RETORNAR resultado

// 5. Medir el área de choque en el segundo t
FUNCION obtener_area_interseccion(A1: Asteroide, A2: Asteroide, t: Real) -> Real:
    // Clonar y mover las figuras al tiempo t
    p1_temporal = Lista de puntos de A1 movidos al tiempo t con mover_punto()
    p2_temporal = Lista de puntos de A2 movidos al tiempo t con mover_punto()

    silueta_recortada = p1_temporal
    m = longitud(p2_temporal)
    
    // Recortar sucesivamente contra cada lado de la segunda figura
    PARA i DESDE 0 HASTA m - 1:
        silueta_recortada = recortar_poligono(silueta_recortada, p2_temporal[i], p2_temporal[(i + 1) MOD m])
        
    RETORNAR calcular_area(silueta_recortada)

// 6. Encontrar cuándo una esquina choca matemáticamente contra una pared
PROCEDIMIENTO registrar_eventos_colision(p: Punto, vp: Vector, a: Punto, b: Punto, va: Vector, vb: Vector, VALOR_REF alarmas: Lista de Real):
    // Se expande analíticamente el determinante algebraico (Det = 0)
    // Esto genera una ecuación cuadrática de tiempo: c2*t^2 + c1*t + c0 = 0
    Calcular coeficientes c2, c1, c0 basados en posiciones iniciales y velocidades relativas...

    SI ValorAbsoluto(c2) < 0.000000001 ENTONCES:
        SI ValorAbsoluto(c1) > 0.000000001 ENTONCES:
            t = -c0 / c1
            SI t >= 0 ENTONCES Añadir t a alarmas
    SINO:
        discriminante = c1*c1 - 4*c2*c0
        SI discriminante >= 0 ENTONCES:
            t1 = (-c1 + RaizCuadrada(discriminante)) / (2 * c2)
            t2 = (-c1 - RaizCuadrada(discriminante)) / (2 * c2)
            SI t1 >= 0 ENTONCES Añadir t1 a alarmas
            SI t2 >= 0 ENTONCES Añadir t2 a alarmas

// 7. Hilo Principal (Algoritmo de Barrido e Interpolación)
ALGORITMO PRINCIPAL:
    Leer datos de Asteroide A1 y Asteroide A2
    asegurar_sentido_antihorario(A1.vertices)
    asegurar_sentido_antihorario(A2.vertices)

    alarmas = Lista Inicializada con [0.0]

    // Generar todas las colisiones posibles (Vértices contra Aristas)
    PARA cada vertice de A1 y cada arista de A2:
        registrar_eventos_colision(vertice, arista, alarmas)
    PARA cada vertice de A2 y cada arista de A1:
        registrar_eventos_colision(vertice, arista, alarmas)

    Ordenar alarmas cronológicamente de menor a mayor
    Eliminar duplicados o tiempos extremadamente cercanos en alarmas
    Añadir una alarma de seguridad final: (Ultima_Alarma + 10.0)

    max_area = 0.0
    tiempo_optimo = 0.0

    // Analizar tramo por tramo usando parábolas
    PARA i DESDE 0 HASTA longitud(alarmas) - 2:
        t0 = alarmas[i]
        t2 = alarmas[i+1]
        t1 = (t0 + t2) / 2.0 // Punto medio

        // Medir el área real en los tres puntos del intervalo
        y0 = obtener_area_interseccion(A1, A2, t0)
        y1 = obtener_area_interseccion(A1, A2, t1)
        y2 = obtener_area_interseccion(A1, A2, t2)

        // Actualizar récords con los extremos analizados
        SI y0 > max_area ENTONCES: max_area = y0; tiempo_optimo = t0
        SI y1 > max_area ENTONCES: max_area = y1; tiempo_optimo = t1
        SI y2 > max_area ENTONCES: max_area = y2; tiempo_optimo = t2

        // Interpolar parábola trasladando el intervalo a origen t0=0
        Calcular coeficientes 'a' y 'b' de la parábola y = a*t^2 + b*t + c...
        
        SI a < -0.000000001 ENTONCES: // Si tiene un punto máximo arriba
            t_vertice = -b / (2.0 * a) + t0 // Revertir traslado temporal
            
            SI t_vertice >= t0 E t_vertice <= t2 ENTONCES:
                area_vertice = obtener_area_interseccion(A1, A2, t_vertice)
                SI area_vertice > max_area ENTONCES:
                    max_area = area_vertice
                    tiempo_optimo = t_vertice

    // Determinar la salida final
    SI max_area > 0.0000001 ENTONCES:
        Imprimir tiempo_optimo, max_area
    SINO:
        // Validación de contacto superficial para descartar el "never"
        se_roza = Falso
        tiempo_roce = 0.0
        PARA cada t en alarmas:
            SI la intersección en el tiempo t contiene puntos (aunque el área sea 0) ENTONCES:
                se_roza = Verdadero
                tiempo_roce = t
                Terminar Bucle
                
        SI se_roza ENTONCES:
            Imprimir tiempo_roce, 0.0
        SINO:
            Imprimir "never"
```

## 2.1. Declaracion de constantes y bibliotecas:

```C++
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

const double EPS = 1e-7;
```
Usamos las librerias de entrada y salida estandar de C++, la libreria de vectores de C++, la libreria estandar de matematicas de C++, la libreria algoritmo de C++ y la libreria de formato de entrada y salida de C++.

Se usa el namespacer standart y se declara una variable epsilon o Umbral de tolerancia para las fracciones.

## 2.2. Los Cimientos (Representacion de los asteroides) y la maquina del tiempo:

Antes de mover nada, necesitamos enseñarle a la computadora qué es un asteroide. Creamos dos estructuras:

• `Point` y `Polygon`: Un asteroide es simplemente una lista ordenada de esquinas (pts) y un motor que lo empuja a una velocidad constante en el mapa (vx, vy).

```C++
struct Point {

    double x, y;

};

struct Polygon {
    
    vector<Point> pts;
    double vx, vy;

};
```

• `move_point`: Es la máquina del tiempo de los puntos. Si le das un vértice en la posición cero y le dices "avanza 3.5 segundos", calcula matemáticamente dónde estará esa esquina en ese segundo exacto usando Posición + Velocidad * Tiempo

```C++
Point move_point(const Point& p, double vx, double vy, double t) {
    return {p.x + vx * t, p.y + vy * t};
}
```
## 2.3. Calculo de superficies:

```C++
double polygon_area(const vector<Point>& poly) {
    int n = poly.size();
    if (n < 3) return 0.0;
    double area = 0.0;
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        area += poly[i].x * poly[j].y - poly[j].x * poly[i].y;
    }
    return abs(area) / 2.0;
}
```

• Función: Calcula el área en metros cuadrados de cualquier polígono.

• Línea por línea:
	
    • if (n < 3) return 0.0;: Si la figura tiene menos de 3 puntos, es una línea o un punto aislado, por lo que su área es cero.
	
    • El ciclo for implementa la Fórmula de Shoelace (o de Gauss). Multiplica la coordenada X de un vértice por la Y del siguiente (poly[i].x * poly[j].y) y le resta la coordenada X del siguiente por la Y del actual. El uso de (i + 1) % n asegura que cuando estemos en el último vértice, el "siguiente" sea automáticamente el primero, cerrando la figura.
	
    • return abs(area) / 2.0;: Al final, la suma se divide entre 2. Usamos abs (valor absoluto) porque el resultado puede ser negativo si el polígono se leyó al revés.

## 2.4. Correccion de orientacion:

Las matemáticas de recorte fallan si leemos las esquinas al revés (como intentar cortar un papel de afuera hacia adentro).

```C++
void ensure_counter_clockwise(vector<Point>& pts) {
    int n = pts.size();
    if (n < 3) return;
    double area = 0;
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        area += pts[i].x * pts[j].y - pts[j].x * pts[i].y;
    }
    if (area < 0) {
        reverse(pts.begin(), pts.end());
    }
}
```

• Esta función calcula rápidamente el área del asteroide. Si el área da un número negativo, significa que los puntos entraron en el sentido de las agujas del reloj.

• Si es así, invierte el orden de la lista (`reverse`) para obligar a que todos los asteroides se lean siempre en sentido antihorario. Así las fórmulas de colisión funcionan a la perfección.

## 2.5. Guillotina de poligonos:

¿Cómo sabemos el tamaño del choque entre dos asteroides? Dejamos un asteroide quieto y usamos las aristas del otro como una "guillotina".

```C++
vector<Point> clip_polygon(const vector<Point>& poly, Point a, Point b) {
    vector<Point> next_poly;
    int n = poly.size();
    if (n == 0) return next_poly;

    for (int i = 0; i < n; i++) {
        Point p1 = poly[i];
        Point p2 = poly[(i + 1) % n];

        double cross1 = (b.x - a.x) * (p1.y - a.y) - (b.y - a.y) * (p1.x - a.x);
        double cross2 = (b.x - a.x) * (p2.y - a.y) - (b.y - a.y) * (p2.x - a.x);

        if (cross1 >= -EPS) {
            next_poly.push_back(p1);
        }
        if ((cross1 > EPS && cross2 < -EPS) || (cross1 < -EPS && cross2 > EPS)) {
            double t = cross1 / (cross1 - cross2);
            next_poly.push_back({p1.x + t * (p2.x - p1.x), p1.y + t * (p2.y - p1.y)});
        }
    }
    return next_poly;
}
```

• `clip_polygon` toma el asteroide 1 y traza una línea infinita usando un lado del asteroide 2. Revisa qué esquinas quedan del lado "de adentro" y cuáles "de afuera". Corta los pedazos que sobresalen y se queda solo con lo interno.

• Al repetir este corte contra todos los lados del asteroide 2, el asteroide 1 termina perfectamente recortado. Lo que sobrevive es la silueta exacta del choque.

• `polygon_area` usa la famosa Fórmula de la Aguja (Shoelace) para medir cuántos metros cuadrados mide esa silueta superviviente.

## 2.6. Choque Estatico:

• Función: Calcula el área de intersección exacta entre los dos asteroides en un segundo t específico.

```C++
double intersection_area(const Polygon& P1, const Polygon& P2, double t) {
    vector<Point> poly1, poly2;
    for (const auto& p : P1.pts) poly1.push_back(move_point(p, P1.vx, P1.vy, t));
    for (const auto& p : P2.pts) poly2.push_back(move_point(p, P2.vx, P2.vy, t));

    vector<Point> clipped = poly1;
    int m = poly2.size();
    for (int i = 0; i < m; i++) {
        clipped = clip_polygon(clipped, poly2[i], poly2[(i + 1) % m]);
    }
    return polygon_area(clipped);
}
```

• Línea por línea:
	
    • Los dos primeros ciclos for clonan los asteroides y los "viajan en el tiempo" hasta el segundo t usando move_point. Ahora tenemos dos figuras estáticas en sus posiciones correspondientes al tiempo t.
	
    • vector<Point> clipped = poly1;: Copia la silueta del primer asteroide.
	
    • El ciclo for (int i = 0; i < m; i++): Toma esa silueta y la recorta de manera sucesiva contra cada una de las paredes de la arista del segundo asteroide (poly2[i] y poly2[(i+1)%m]).
	
    • return polygon_area(clipped);: Lo que sobrevive de esos recortes sucesivos es el polígono del impacto. Se mide su área y se regresa el resultado.

## 2.7. Calculador de eventos criticos:

Este es el "corazón analítico". Evaluar el área segundo a segundo de forma aleatoria es imposible (el tiempo es infinito). Necesitamos saber cuándo ocurren los momentos dramáticos.

```C++
void add_intersection_events(Point p, double vpx, double vpy, Point a, Point b, double vax, double vay, double vbx, double vby, vector<double>& events) {
    // Ecuación lineal en t para determinar colinealidad: Det(A(t) - P(t), B(t) - P(t)) = 0
    // Esto se reduce a una ecuación cuadrática o lineal de la forma: c2*t^2 + c1*t + c0 = 0
    double x_p0 = p.x, y_p0 = p.y, v_px = vpx, v_py = vpy;
    double x_a0 = a.x, y_a0 = a.y, v_ax = vax, v_ay = vay;
    double x_b0 = b.x, y_b0 = b.y, v_bx = vbx, v_by = vby;

    double dx_a = x_a0 - x_p0, dy_a = y_a0 - y_p0;
    double dx_b = x_b0 - x_p0, dy_b = y_b0 - y_p0;
    double dvx_a = v_ax - v_px, dvy_a = v_ay - v_py;
    double dvx_b = v_bx - v_px, dvy_b = v_by - v_py;

    double c2 = dvx_a * dvy_b - dvy_a * dvx_b;
    double c1 = dx_a * dvy_b + dvx_a * dy_b - (dy_a * dvx_b + dvy_a * dx_b);
    double c0 = dx_a * dy_b - dy_a * dx_b;

    if (abs(c2) < 1e-9) {
        if (abs(c1) > 1e-9) {
            double t = -c0 / c1;
            if (t >= 0) events.push_back(t);
        }
    } else {
        double disc = c1 * c1 - 4 * c2 * c0;
        if (disc >= 0) {
            double t1 = (-c1 + sqrt(disc)) / (2 * c2);
            double t2 = (-c1 - sqrt(disc)) / (2 * c2);
            if (t1 >= 0) events.push_back(t1);
            if (t2 >= 0) events.push_back(t2);
        }
    }
}
```

• Un momento dramático ocurre exactamente cuando una esquina de un asteroide choca contra la pared (arista) del otro.

• Esta función toma una esquina y un segmento, plantea una ecuación cuadrática basándose en sus velocidades relativas (at² + bt + c = 0) y despeja la variable t (tiempo) con la fórmula general de la escuela.

• Cada tiempo t ≥ 0 donde la esquina toca la pared se guarda en una lista de alarmas llamada events.

## 2.8. Construccion de la linea temporal (Funcion Main):

Aquí se junta todo en una línea temporal ordenada:

```C++
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n1, n2;
    if (!(cin >> n1)) return 0;

    Polygon P1, P2;
    P1.pts.resize(n1);
    for (int i = 0; i < n1; i++) cin >> P1.pts[i].x >> P1.pts[i].y;
    cin >> P1.vx >> P1.vy;

    cin >> n2;
    P2.pts.resize(n2);
    for (int i = 0; i < n2; i++) cin >> P2.pts[i].x >> P2.pts[i].y;
    cin >> P2.vx >> P2.vy;

    ensure_counter_clockwise(P1.pts);
    ensure_counter_clockwise(P2.pts);

    vector<double> events = {0.0};

    // Calcular eventos: Vértices de P1 cruzando Aristas de P2
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < n2; j++) {
            add_intersection_events(P1.pts[i], P1.vx, P1.vy, 
                                    P2.pts[j], P2.pts[(j+1)%n2], 
                                    P2.vx, P2.vy, P2.vx, P2.vy, events);
        }
    }
    // Calcular eventos: Vértices de P2 cruzando Aristas de P1
    for (int i = 0; i < n2; i++) {
        for (int j = 0; j < n1; j++) {
            add_intersection_events(P2.pts[i], P2.vx, P2.vy, 
                                    P1.pts[j], P1.pts[(j+1)%n1], 
                                    P1.vx, P1.vy, P1.vx, P1.vy, events);
        }
    }

    // Filtrar, ordenar y limpiar eventos duplicados
    sort(events.begin(), events.end());
    vector<double> unique_events;
    for (double t : events) {
        if (unique_events.empty() || t - unique_events.back() > 1e-7) {
            unique_events.push_back(t);
        }
    }
    // Añadimos un margen final seguro por si el máximo ocurre justo al terminar el último evento detectado
    unique_events.push_back(unique_events.back() + 10.0);

    double max_area = 0.0;
    double best_t = 0.0;

    for (size_t i = 0; i < unique_events.size() - 1; i++) {
        double t0 = unique_events[i];
        double t2 = unique_events[i+1];
        double t1 = (t0 + t2) / 2.0;

        double y0 = intersection_area(P1, P2, t0);
        double y1 = intersection_area(P1, P2, t1);
        double y2 = intersection_area(P1, P2, t2);

        if (y0 > max_area + EPS) { max_area = y0; best_t = t0; }
        if (y1 > max_area + EPS) { max_area = y1; best_t = t1; }
        if (y2 > max_area + EPS) { max_area = y2; best_t = t2; }

        double mid = t1 - t0;
        double end = t2 - t0;
        double det = mid * end * (end - mid);

        if (abs(det) > 1e-9) {
            double a = (end * (y1 - y0) - mid * (y2 - y0)) / (-det);
            double b = (mid * mid * (y2 - y0) - end * end * (y1 - y0)) / (-det);
            
            if (a < -1e-9) { 
                double local_max_t = -b / (2.0 * a) + t0;
                if (local_max_t >= t0 && local_max_t <= t2) {
                    double area_at_max = intersection_area(P1, P2, local_max_t);
                    if (area_at_max > max_area + EPS) {
                        max_area = area_at_max;
                        best_t = local_max_t;
                    }
                }
            }
        }
    }

    // El problema requiere imprimir tanto el tiempo óptimo como el área máxima
    
    if(best_t > 0){
    
        cout << fixed << setprecision(9) << best_t;

    }else{cout << "never\n";}
    
    return 0;
}
```

1. Lee los asteroides y los fuerza a ir en sentido antihorario.

2. Genera las alarmas: Revisa cada esquina del primer asteroide contra cada pared del segundo (y viceversa) y guarda todos los tiempos de colisión posibles.

3. Ordena la línea temporal: Ordena las alarmas cronológicamente de menor a mayor y elimina los duplicados (tiempos idénticos).

4. Inspecciona entre alarmas (Interpolación): El programa sabe que entre la alarma A y la alarma B, el área cambia suavemente como una curva matemática (una parábola). Evalúa el área en tres instantes de ese pequeño intervalo (inicio, mitad y fin), deduce la forma de la parábola y calcula matemáticamente si el punto más alto (el vértice -b / 2a) cae dentro de ese lapso de tiempo.

5. Veredicto Final: Al recorrer todos los intervalos, el programa recuerda cuál fue el pico más alto de área visto en la simulación.

	• Si el pico más alto es mayor a cero, te imprime el segundo exacto y el tamaño del choque.

	• Si después de revisar todas las alarmas descubres que el área máxima fue absolutamente cero, hace un último chequeo para ver si al menos las siluetas se rozaron (!clipped.empty()). Si ni siquiera se rozaron las esquinas, la pantalla imprime con total seguridad: never.

# 3. Justificacion:

