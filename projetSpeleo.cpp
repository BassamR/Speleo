#include <iostream>
#include <vector>
#include <iomanip>
#include <random>
#include <ctime>
using namespace std;
typedef vector<bool> vecteur;
typedef vector<vecteur> matrice;

void niveauA();
double niveauB(const int& n, const double& p, const int& nbt);
void niveauC();
matrice lireGrillePbm(); // lire en tenant compte du format pbm
void afficheTableauPbm(const matrice& tab); // afficher en pbm
    //above algs need to be optimized for n>70
void afficheTableau(const matrice& tab); // afficher un vector normalement
vector<int> initPassage(const matrice& grilleLibre, matrice& grillePassage);
void construirePassage(int a, int b, const matrice& grilleLibre, matrice& grillePassage,
    const vector<int>& indexes);
bool checkValidity(int a, int b, const matrice& grilleLibre, const matrice& grillePassage);
bool existencePassage(const matrice& grille);
matrice genereGrille(int n, double p, default_random_engine& e);

int main() {
    cout << setprecision(6);
    cout << fixed;
    char head;
    cin >> head;
    if (head == 'a') {
        niveauA();
    } else if (head == 'b') {
        int n, nbt;
        double p;
        cin >> n >> p >> nbt;
        cout << niveauB(n, p, nbt) << endl;
    } else if (head == 'c') {
        niveauC();
    } else {
        cout << "niveau invalide";
    }
    return 0;
}

void niveauA() {
    matrice libre;
    libre = lireGrillePbm();
    unsigned int n = libre.size();
    matrice passage(n, vecteur(n));

    vector<int> indexes;
    indexes = initPassage(libre, passage);
    for (int i = 0; i < indexes.size(); ++i) {
        int x = indexes[i];
        construirePassage(0, x, libre, passage, indexes);
    }
    afficheTableauPbm(passage);
} // fonctionne pour les grilles de petite taille

double niveauB(const int& n, const double& p, const int& nbt) { // n (taille grille), p (proba), nbt (nbr de grilles a generer)
    // if (p > 1 or p < 0) {
    //     cout << "probabilite incorrecte" << endl;
    //     return 0;
    // }

    double compte = 0.0;
    default_random_engine e(time(0));
    for (int i = 0; i < nbt; ++i) {
        matrice passagetest;
        passagetest = genereGrille(n, p, e);
        if (existencePassage(passagetest)) {
            ++compte;
        }
    }
    return compte/nbt;
}

void niveauC() { // n (taille grille), nbt (nbr de grilles)
    int NBP = 102;
    double pas = 1.0/(NBP-2);
    double p = 0.0;
    int n, nbt;
    cin >> n >> nbt;
    cout << n << " " << nbt << endl;
    while (p <= 1.0) {
        cout << p << " " << niveauB(n, p, nbt) << endl;
        if (niveauB(n, p, nbt) == 1.0) {
            break;
        } else {
            p += pas;
        }
    }
    do {
        cout << p << " " << 1.0 << endl;
        p += pas;
    } while (p <= 1.0);
}

matrice lireGrillePbm() {
    int n;
    cin >> n;
    matrice grille(n, vecteur(n));
    bool a;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> a;
            grille[i][j] = !a;
        }
    }
    return grille;
} // lire une grille en la transformant de pbm a c++
//urgent: ajouter un saut de ligne si plus de 70 caracteres par ligne

void afficheTableauPbm(const matrice& tab) {
    unsigned int n = tab.size();
    cout << "P1" << endl;
    cout << n << " " << n << endl;
    for (auto ligne : tab) {
        for (auto colonne : ligne) {
            cout << !colonne << " ";
        }
        cout << endl;
    }
} // affiche grille en transformant c++ en pbm
//urgent: ajouter un saut de ligne si plus de 70 caracteres par ligne

void afficheTableau(const matrice& tab) {
    for (auto ligne : tab) {
        for (auto colonne : ligne) {
            cout << colonne << " ";
        }
        cout << endl;
    }
} // affiche un tableau normalement

vector<int> initPassage(const matrice& grilleLibre, matrice& grillePassage) {
    unsigned int n = grilleLibre.size();
    vector<int> indexes;
    for (int i = 0; i < n; ++i) {
        if (grilleLibre[0][i] == 1) {
            grillePassage[0][i] = 1;
            indexes.push_back(i);
        }
    }
    return indexes;
}

void construirePassage(int a, int b, const matrice& grilleLibre, matrice& grillePassage, 
    const vector<int>& indexes) {
    unsigned int n = grilleLibre.size();
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

bool checkValidity(int a, int b, const matrice& grilleLibre, const matrice& grillePassage) {
    unsigned int n = grilleLibre.size();
    bool dehors = false;
    bool obstruee = false;
    bool visitee = false;
    if (a >= n or b >= n or a < 0 or b < 0) {
        dehors = true; // true if outside the grille
    } else {
        obstruee = not grilleLibre[a][b]; //obstructed ? true if yes
        visitee = grillePassage[a][b]; // true if grillepassage has been visited 
    }
    bool X = dehors or obstruee or visitee; // true if one of the conditions is met
    return not X;
} // returns false when a cell is not valid

bool existencePassage(const matrice& grille) {
    bool X = false;
    size_t n = grille.size();
    for (int i = 0; i < n; ++i) {
        if (grille[n-1][i] == 1) {
            X = true;
        }
    }
    return X;
} // returns true if theres a free cell in the last line

matrice genereGrille(int n, double p, default_random_engine& e) {
    bernoulli_distribution b(p);
    matrice grille(n, vecteur(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            grille[i][j] = b(e);
        }
    }
    matrice grillePassage(n, vecteur(n));
    vector<int> indexes;
    indexes = initPassage(grille, grillePassage);
    for (int i = 0; i < indexes.size(); ++i) {
        int x = indexes[i];
        construirePassage(0, x, grille, grillePassage, indexes);
    }
    return grillePassage;
} // generates a random grid and solves it
