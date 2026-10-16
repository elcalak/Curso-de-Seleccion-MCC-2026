/*

Solucion al Problema M Window Manager by: GoogleAI

Paradigma: 

Comentarios by: Calak de Astora

Input:

320 200
OPEN 50 50 10 10
OPEN 70 55 10 10
OPEN 90 50 10 10
RESIZE 55 55 40 40
RESIZE 55 55 15 15
MOVE 55 55 40 0
CLOSE 55 55
CLOSE 110 60
MOVE 95 55 0 -100

*/

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cmath>

using namespace std;

struct Window {
    int id;
    int x, y, w, h;
};

int screen_w, screen_h;
vector<Window> windows;
int window_counter = 0;

// Intersección estricta de intervalos discretos semiabiertos [x, x+w)
bool intersect(const Window& a, const Window& b) {
    return max(a.x, b.x) < min(a.x + a.w, b.x + b.w) &&
           max(a.y, b.y) < min(a.y + a.h, b.y + b.h);
}

// Validación estática para OPEN y RESIZE
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

// Encuentra la ventana cliqueada (orden Z invertido)
int find_window_at(int x, int y) {
    for (int i = (int)windows.size() - 1; i >= 0; --i) {
        if (x >= windows[i].x && x < windows[i].x + windows[i].w &&
            y >= windows[i].y && y < windows[i].y + windows[i].h) {
            return i;
        }
    }
    return -1;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    string line;
    if (!getline(cin, line)) return 0;
    stringstream ss_init(line);
    if (!(ss_init >> screen_w >> screen_h)) return 0;
    
    int line_num = 1;
    
    while (getline(cin, line)) {
        if (line.empty()) break; // Terminar inmediatamente si hay línea vacía en consola
        
        line_num++;
        stringstream ss(line);
        string cmd;
        if (!(ss >> cmd)) continue;
        
        if (cmd == "OPEN") {
            int x, y, w, h;
            ss >> x >> y >> w >> h;
            Window new_win = {++window_counter, x, y, w, h};
            if (is_valid(new_win)) {
                windows.push_back(new_win);
            } else {
                cout << "Command " << line_num << ": OPEN - window does not fit\n";
                window_counter--;
            }
        } 
        else if (cmd == "CLOSE") {
            int x, y;
            ss >> x >> y;
            int idx = find_window_at(x, y);
            if (idx != -1) {
                windows.erase(windows.begin() + idx);
            } else {
                cout << "Command " << line_num << ": CLOSE - no window at given position\n";
            }
        } 
        else if (cmd == "RESIZE") {
            int x, y, w, h;
            ss >> x >> y >> w >> h;
            int idx = find_window_at(x, y);
            if (idx != -1) {
                Window resized = windows[idx];
                resized.w = w;
                resized.h = h;
                if (is_valid(resized, resized.id)) {
                    windows[idx] = resized;
                } else {
                    cout << "Command " << line_num << ": RESIZE - window does not fit\n";
                }
            } else {
                cout << "Command " << line_num << ": RESIZE - no window at given position\n";
            }
        } 
        else if (cmd == "MOVE") {
            int x, y, dx, dy;
            ss >> x >> y >> dx >> dy;
            int idx = find_window_at(x, y);
            
            if (idx != -1) {
                int amount = max(abs(dx), abs(dy));
                int dir_x = (dx > 0) ? 1 : ((dx < 0) ? -1 : 0);
                int dir_y = (dy > 0) ? 1 : ((dy < 0) ? -1 : 0);
                
                vector<int> active_group = {idx};
                int moved_units = 0;
                
                // Mover píxel a píxel de forma controlada iterativa libre de bucles recursivos
                for (int step = 0; step < amount; ++step) {
                    bool boundary_collision = false;
                    for (int g : active_group) {
                        if (dir_x > 0 && windows[g].x + windows[g].w >= screen_w) boundary_collision = true;
                        if (dir_x < 0 && windows[g].x <= 0) boundary_collision = true;
                        if (dir_y > 0 && windows[g].y + windows[g].h >= screen_h) boundary_collision = true;
                        if (dir_y < 0 && windows[g].y <= 0) boundary_collision = true;
                    }
                    if (boundary_collision) break;
                    
                    // Encontrar nuevos vecinos impactados en este píxel específico
                    vector<int> to_add;
                    for (int g : active_group) {
                        Window test_win = windows[g];
                        test_win.x += dir_x;
                        test_win.y += dir_y;
                        
                        for (int i = 0; i < (int)windows.size(); ++i) {
                            if (find(active_group.begin(), active_group.end(), i) != active_group.end()) continue;
                            if (find(to_add.begin(), to_add.end(), i) != to_add.end()) continue;
                            
                            if (intersect(test_win, windows[i])) {
                                to_add.push_back(i);
                            }
                        }
                    }
                    
                    // Si hay colisiones, absorbemos los vecinos y reevaluamos el paso actual
                    if (!to_add.empty()) {
                        for (int a : to_add) active_group.push_back(a);
                        step--; 
                        continue;
                    }
                    
                    // Si el paso fue completamente libre, movemos físicamente 1 píxel al grupo entero
                    for (int g : active_group) {
                        windows[g].x += dir_x;
                        windows[g].y += dir_y;
                    }
                    moved_units++;
                }
                
                if (moved_units < amount) {
                    cout << "Command " << line_num << ": MOVE - moved " << moved_units << " instead of " << amount << "\n";
                }
            } else {
                cout << "Command " << line_num << ": MOVE - no window at given position\n";
            }
        }
    }
    
    // Imprimir el reporte del estado final de las ventanas
    cout << windows.size() << " window(s):\n";
    for (const auto& w : windows) {
        cout << w.x << " " << w.y << " " << w.w << " " << w.h << "\n";
    }
    
    return 0;
}

