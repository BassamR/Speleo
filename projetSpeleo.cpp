#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <iomanip>
#include <cmath>
#include <algorithm>
using namespace std;
typedef vector<bool> Vecteur;
typedef vector<Vecteur> Matrice;

void choisirNiveau();
void niveauA();
double niveauB(const int& n, const double& p, const int& nbt);
void niveauC();
Matrice lireGrillePbm(); // lire en tenant compte du format pbm
void afficheTableauPbm(const Matrice& tab); // afficher en pbm
vector<int> initPassage(const Matrice& grilleLibre, Matrice& grillePassage);
void construirePassage(const int& a, const int& b, const Matrice& grilleLibre, Matrice& grillePassage,
    const vector<int>& indexes);
bool checkValidity(const int& a, const int& b, const Matrice& grilleLibre, 
    const Matrice& grillePassage);
bool existencePassage(const Matrice& grille);
Matrice genereGrille(const size_t& n, const double& p, default_random_engine& e);

int main() {
    cout << setprecision(6);
    cout << fixed;
    choisirNiveau();
    return 0;
}

void choisirNiveau() {
    char niveau;
    cin >> niveau;
    switch (niveau) {
        case 'a': niveauA(); break;
        case 'b':
            int n, nbt;
            double p;
            cin >> n >> p >> nbt;
            cout << niveauB(n, p, nbt) << endl;
            break;
        case 'c': niveauC(); break;
        default: cout << "niveau invalide" << endl; break;
    }
}

void niveauA() {
    Matrice libre(lireGrillePbm());
    size_t n = libre.size();
    Matrice passage(n, Vecteur(n));

    vector<int> indexes;
    indexes = initPassage(libre, passage);
    for (size_t i = 0; i < indexes.size(); ++i) {
        int x = indexes[i];
        construirePassage(0, x, libre, passage, indexes);
    }
    afficheTableauPbm(passage);
}

double niveauB(const int& n, const double& p, const int& nbt) {
    if (p > 1 or p < 0) {
        cout << "probabilite incorrecte" << endl;
        return 0;
    }
    double compte = 0.0;
    default_random_engine e(time(0));
    for (int i = 0; i < nbt; ++i) {
        Matrice passagetest;
        passagetest = genereGrille(n, p, e);
        if (existencePassage(passagetest)) {
            ++compte;
        }
    }
    return compte/nbt;
}

void niveauC() {
    int n, nbt;
    cin >> n >> nbt;
    const double MIN_DELTA_P = 0.01;
    const double MAX_ERROR = 0.000001;
    double min = 0.0;
    double max = 1.0;
    vector<double> p, fp;

    do {
        double error = niveauB(n, (min+max)/2, nbt) - ((niveauB(n, min, nbt) + niveauB(n, max, nbt))/2);
        if (error > 0) {
            p.push_back((max+min)/2);
            fp.push_back(niveauB(n, (max+min)/2, nbt));
            max = (max+min)/2;
        } else {
            p.push_back((max+min)/2);
            fp.push_back(niveauB(n, (max+min)/2, nbt));
            min = (max+min)/2;
        }
    } while ((max - min > MIN_DELTA_P) and (abs(niveauB(n, (min+max)/2, nbt) - ((niveauB(n, min, nbt) + niveauB(n, max, nbt))/2)) > MAX_ERROR));

    sort(p.begin(), p.end());
    sort(fp.begin(), fp.end());
    cout << 0.0 << " " <<  0.0 << endl;
    for (size_t i = 0; i < p.size(); ++i) {
        cout << p[i] << " " << fp[i] << endl;
    }
    cout << 1.0 << " " << 1.0 << endl;
}

Matrice lireGrillePbm() {
    int n;
    cin >> n;
    Matrice grille(n, Vecteur(n));
    bool cellule;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            cin >> cellule;
            grille[i][j] = not cellule;
        }
    }
    return grille;
}

void afficheTableauPbm(const Matrice& tab) {
    const int width = 35;
    size_t n = tab.size();
    cout << "P1" << endl;
    cout << n << " " << n << endl;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            if (j % width == 0 and j != 0) {
                cout << endl;
            }
            cout << not tab[i][j] << " ";
        }
        cout << endl;
    }
}

vector<int> initPassage(const Matrice& grilleLibre, Matrice& grillePassage) {
    size_t n = grilleLibre.size();
    vector<int> indexes;
    for (size_t i = 0; i < n; ++i) {
        if (grilleLibre[0][i] == 1) {
            grillePassage[0][i] = 1;
            indexes.push_back(i);
        }
    }
    return indexes;
}

void construirePassage(const int& a, const int& b, const Matrice& grilleLibre, Matrice& grillePassage, 
    const vector<int>& indexes) {
    if (checkValidity(a-1, b, grilleLibre, grillePassage)) {
        grillePassage[a-1][b] = 1;
        construirePassage(a-1, b, grilleLibre, grillePassage, indexes);
    }
    if (checkValidity(a, b+1, grilleLibre, grillePassage)) {
        grillePassage[a][b+1] = 1;
        construirePassage(a, b+1, grilleLibre, grillePassage, indexes);
    }
    if (checkValidity(a+1, b, grilleLibre, grillePassage)) {
        grillePassage[a+1][b] = 1;
        construirePassage(a+1, b, grilleLibre, grillePassage, indexes);
    }
    if (checkValidity(a, b-1, grilleLibre, grillePassage)) {
        grillePassage[a][b-1] = 1;
        construirePassage(a, b-1, grilleLibre, grillePassage, indexes);
    }
}

bool checkValidity(const int& a, const int& b, const Matrice& grilleLibre, const Matrice& grillePassage) {
    size_t n = grilleLibre.size();
    bool dehors = false;
    bool obstruee = false;
    bool visitee = false;
    if (a >= n or b >= n or a < 0 or b < 0) {
        dehors = true;
    } else {
        obstruee = not grilleLibre[a][b];
        visitee = grillePassage[a][b];
    }
    bool X = dehors or obstruee or visitee;
    return not X;
}

bool existencePassage(const Matrice& grille) {
    bool passage = false;
    size_t n = grille.size();
    for (size_t i = 0; i < n; ++i) {
        if (grille[n-1][i] == 1) {
            passage = true;
        }
    }
    return passage;
}

Matrice genereGrille(const size_t& n, const double& p, default_random_engine& e) {
    bernoulli_distribution b(p);
    Matrice grille(n, Vecteur(n));
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            grille[i][j] = b(e);
        }
    }
    Matrice grillePassage(n, Vecteur(n));
    vector<int> indexes;
    indexes = initPassage(grille, grillePassage);
    for (size_t i = 0; i < indexes.size(); ++i) {
        int x = indexes[i];
        construirePassage(0, x, grille, grillePassage, indexes);
    }
    return grillePassage;
}
