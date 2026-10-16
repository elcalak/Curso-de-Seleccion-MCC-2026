/*

Solucion al Problema I Ship Trafic by: GoogleAI

Paradigma: Barrido

Comentarios by: Calak de Astora

Input:

3 100 5 10 0 100
E 2 100 -300 50 -100
W 3 10 60 50 200 200 400
E 1 100 -300

1 100 5 10 0 200
W 4 100 100 100 300 100 700 100 900

*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cmath>

using namespace std;

struct Interval {
    double l, r;
    bool operator<(const Interval& other) const {
        if (abs(l - other.l) > 1e-9) return l < other.l;
        return r < other.r;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    double w, u, v, t1, t2;

    // Si cin falla al leer los primeros parámetros o llega al final, el programa termina de inmediato
    if (!(cin >> n >> w >> u >> v >> t1 >> t2)) {
        return 0;
    }

    vector<Interval> blocked_intervals;

    for (int i = 0; i < n; ++i) {
        char dir;
        int m;
        cin >> dir >> m;

        double t_start_lane = (i * w) / v;
        double t_end_lane = ((i + 1) * w) / v;

        for (int j = 0; j < m; ++j) {
            double length, pos;
            cin >> length >> pos;

            double t_entry, t_exit;

            if (dir == 'E') {
                t_entry = -pos / u;
                t_exit = (length - pos) / u;
            } else {
                t_entry = pos / u;
                t_exit = (pos + length) / u;
            }

            double l = t_entry - t_end_lane;
            double r = t_exit - t_start_lane;

            l = max(l, t1);
            r = min(r, t2);

            if (l < r - 1e-9) {
                blocked_intervals.push_back({l, r});
            }
        }
    }

    // Si no hay intervalos bloqueados, la respuesta es el rango completo
    if (blocked_intervals.empty()) {
        cout << fixed << setprecision(8) << (t2 - t1) << "\n";
        return 0;
    }

    // 1. Ordenar
    sort(blocked_intervals.begin(), blocked_intervals.end());

    // 2. Fusionar
    vector<Interval> merged;
    merged.push_back(blocked_intervals[0]); 

    for (size_t i = 1; i < blocked_intervals.size(); ++i) {
        if (blocked_intervals[i].l <= merged.back().r + 1e-9) {
            merged.back().r = max(merged.back().r, blocked_intervals[i].r);
        } else {
            merged.push_back(blocked_intervals[i]);
        }
    }

    // 3. Encontrar ventana máxima
    double max_gap = 0.0;

    max_gap = max(max_gap, merged[0].l - t1);

    for (size_t i = 1; i < merged.size(); ++i) {
        max_gap = max(max_gap, merged[i].l - merged[i-1].r);
    }

    max_gap = max(max_gap, t2 - merged.back().r);

    // Imprimir resultado y terminar de inmediato
    cout << fixed << setprecision(8) << max_gap << "\n";

    return 0;
}
