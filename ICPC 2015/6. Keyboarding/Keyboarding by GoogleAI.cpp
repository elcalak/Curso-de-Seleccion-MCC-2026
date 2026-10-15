/*

Solucion al Problema F Keyboarding by: GoogleAI

Paradigma: 

Comentarios by: Calak de Astora

Input:

4 7
ABCDEFG
HIJKLMN
OPQRSTU
VWXYZ**
CONTEST

5 20
12233445566778899000
QQWWEERRTTYYUUIIOOPP
-AASSDDFFGGHHJJKKLL*
--ZZXXCCVVBBNNMM--**
--------------------
ACM-ICPC-WORLD-FINALS-2015

2 19
ABCDEFGHIJKLMNOPQZY
X*****************Y
AZAZ

6 4
AXYB
BBBB
KLMB
OPQB
DEFB
GHI*
AB

*/

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>

using namespace std;

struct Position {
    int r, c;
};

struct State {
    int r, c, idx, dist;
};

const int dr[] = {-1, 1, 0, 0};
const int dc[] = {0, 0, -1, 1};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int R, C;
    if (!(cin >> R >> C)) return 0;

    vector<string> grid(R);
    for (int i = 0; i < R; ++i) {
        cin >> grid[i];
    }

    string target;
    cin >> target;
    target += "*"; // Agregar el carácter Enter obligatorio al final
    int target_len = target.length();

    // 1. PRECOMPUTACIÓN DE MOVIMIENTOS CORREGIDA
    vector<vector<vector<Position>>> next_pos(R, vector<vector<Position>>(C, vector<Position>(4)));

    for (int r = 0; r < R; ++r) {
        for (int c = 0; c < C; ++c) {
            for (int d = 0; d < 4; ++d) {
                int nr = r;
                int nc = c;
                bool found_different = false;
                
                while (true) {
                    int next_r = nr + dr[d];
                    int next_c = nc + dc[d];
                    if (next_r < 0 || next_r >= R || next_c < 0 || next_c >= C) break;
                    
                    nr = next_r;
                    nc = next_c;
                    
                    if (grid[nr][nc] != grid[r][c]) {
                        found_different = true;
                        break;
                    }
                }
                
                // REGLA CRUCIAL: Si no se encontró un carácter diferente en esa dirección, 
                // el cursor NO se mueve (se queda en la posición original r, c)
                if (found_different) {
                    next_pos[r][c][d] = {nr, nc};
                } else {
                    next_pos[r][c][d] = {r, c};
                }
            }
        }
    }

    // 2. CONFIGURACIÓN DEL BFS LIMPIO
    vector<vector<vector<int>>> dist(R, vector<vector<int>>(C, vector<int>(target_len + 1, -1)));
    queue<State> q;

    // Comenzamos estrictamente en (0,0) con progreso 0 y movimientos 0
    q.push({0, 0, 0, 0});
    dist[0][0][0] = 0;

    // 3. CICLO PRINCIPAL
    while (!q.empty()) {
        State current = q.front();
        q.pop();

        if (current.idx == target_len) {
            cout << current.dist << "\n";
            return 0;
        }

        if (current.dist > dist[current.r][current.c][current.idx]) continue;

        // ACCIÓN A: Seleccionar carácter actual (Pulsar '*')
        if (grid[current.r][current.c] == target[current.idx]) {
            int n_idx = current.idx + 1;
            int n_dist = current.dist + 1;
            
            if (dist[current.r][current.c][n_idx] == -1 || n_dist < dist[current.r][current.c][n_idx]) {
                dist[current.r][current.c][n_idx] = n_dist;
                q.push({current.r, current.c, n_idx, n_dist});
            }
        }

        // ACCIÓN B: Moverse en las 4 direcciones
        for (int d = 0; d < 4; ++d) {
            Position next_cell = next_pos[current.r][current.c][d];
            // Si el cursor se quedó quieto por la regla del borde, ignoramos el movimiento
            if (next_cell.r == current.r && next_cell.c == current.c) continue;

            int n_dist = current.dist + 1;

            if (dist[next_cell.r][next_cell.c][current.idx] == -1 || n_dist < dist[next_cell.r][next_cell.c][current.idx]) {
                dist[next_cell.r][next_cell.c][current.idx] = n_dist;
                q.push({next_cell.r, next_cell.c, current.idx, n_dist});
            }
        }
    }

    return 0;
}
