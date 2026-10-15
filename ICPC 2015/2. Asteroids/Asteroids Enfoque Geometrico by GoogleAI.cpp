/*

Solucion al Problema B Asteroids by: GoogleAI

Paradigma: Enfoque Geometrico

Comentarios by: Calak de Astora

Input:

6 3 2 2 4 3 6 6 6 7 4 6 2 2 2
4 18 5 22 9 26 5 22 1 -2 1

4 0 0 0 2 2 2 2 0 -1 1
4 10 0 10 2 12 2 12 0 1 1

*/

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

const double EPS = 1e-7;

struct Point {
    double x, y;
};

struct Polygon {
    vector<Point> pts;
    double vx, vy;
};

Point move_point(const Point& p, double vx, double vy, double t) {
    return {p.x + vx * t, p.y + vy * t};
}

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

// Asegura que los puntos estén en sentido antihorario para el algoritmo de recorte
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

// Encuentra el tiempo t >= 0 donde un vértice en movimiento cruza una arista en movimiento
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
