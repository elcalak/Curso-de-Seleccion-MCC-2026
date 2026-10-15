/*

Solucion al Problema E Evolution in Parallel by: GoogleAI

Paradigma: Greedy y valor se subsecuencias locales

Comentarios by: Calak de Astora

Input:

5
AACCMMAA
ACA
MM
ACMAA
AA
A

3
ACMA
ACM
ACA
AMA

1
AM
MA

4
AAAAAA
AA
AAA
A
AAAAA

*/

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Función eficiente de dos punteros O(N + M) para verificar subsecuencias
bool is_subsequence(const string& main_str, const string& sub) {
    if (sub.length() > main_str.length()) return false;
    
    size_t i = 0, j = 0;
    while (i < main_str.length() && j < sub.length()) {
        if (main_str[i] == sub[j]) {
            j++;
        }
        i++;
    }
    return j == sub.length();
}

int main() {
    // Optimización de I/O para programación competitiva
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    string root;
    if (!(cin >> root)) return 0;

    vector<string> DNA(n);
    for (int i = 0; i < n; i++) {
        if (!(cin >> DNA[i])) return 0;
    }

    // Ordenar de mayor a menor longitud
    sort(DNA.begin(), DNA.end(), [](const string& a, const string& b) {
        return a.length() > b.length();
    });

    // Validar inicialmente que todas las cadenas provienen de la raíz principal
    for (int i = 0; i < n; i++) {
        if (!is_subsequence(root, DNA[i])) {
            cout << "impossible\n";
            return 0;
        }
    }

    vector<string> path1, path2;
    string last1 = root;
    string last2 = root;

    for (int i = 0; i < n; i++) {
        bool sub1 = is_subsequence(last1, DNA[i]);
        bool sub2 = is_subsequence(last2, DNA[i]);

        if (!sub1 && !sub2) {
            cout << "impossible\n";
            return 0;
        }
        else if (sub1 && !sub2) {
            path1.push_back(DNA[i]);
            last1 = DNA[i];
        }
        else if (!sub1 && sub2) {
            path2.push_back(DNA[i]);
            last2 = DNA[i];
        }
        else {
            // CASO CRÍTICO CORREGIDO:
            // Si el extremo de la rama 2 está contenido en la rama 1,
            // extendemos la rama 2. De lo contrario, extendemos la rama 1.
            if (is_subsequence(last1, last2)) {
                path2.push_back(DNA[i]);
                last2 = DNA[i];
            } else {
                path1.push_back(DNA[i]);
                last1 = DNA[i];
            }
        }
    }

    // El problema requiere imprimir en orden cronológico (de menor a mayor longitud)
    reverse(path1.begin(), path1.end());
    reverse(path2.begin(), path2.end());

    // Imprimir tamaños de ambas ramas
    cout << path1.size() << " " << path2.size() << "\n";
    
    // Imprimir elementos de cada camino
    for (const string& s : path1) {
        cout << s << "\n";
    }
    for (const string& s : path2) {
        cout << s << "\n";
    }

    return 0;
}