#include "Vecteur.h"
#include "Matrice.h"
#include <iostream>

Vecteur::Vecteur(int taille) {
    // initialise "donnees" avec "taille" éléments à 0.0
    donnees.resize(taille, 0.0);
}

int Vecteur::getTaille() const {
    //conversion de type implicite de size_t à int
    return static_cast<int>(donnees.size());
}

double& Vecteur::operator[](int index) {
    // renvoie une référence vers donnees[index]
    return donnees[index];
}

double Vecteur::operator[](int index) const {
    // renvoie une référence vers donnees[index]
    return donnees[index];
}


Vecteur Vecteur::operator+(const Vecteur& autre) const {
    // 1. crée un nouveau Vecteur "resultat" de la bonne taille
    // 2. remplis-le avec la somme composante par composante
    // 3. renvoie-le
    int taille = getTaille();
    Vecteur resultat(taille);
    for (int i=0; i<taille; i++){
        resultat[i] = donnees[i] + autre.donnees[i];
    }
    return resultat;
}

void Vecteur::afficher() const {
    // affiche le vecteur, par exemple sous la forme [1, 2, 3]
    // indice : std::cout << "..." ; et une boucle for
    std::cout << "[";
    for (int i=0; i<getTaille(); i++){
        std::cout << donnees[i];
        if (i < getTaille() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
}

Vecteur Vecteur::hadamard(const Vecteur& autre) const {
    // 1. crée un Vecteur "resultat" de même taille
    // 2. pour chaque i : resultat[i] = (*this)[i] * autre[i]
    // 3. renvoie resultat
    int n = getTaille();
    Vecteur resultat(n);
    for (int i = 0; i < n ; i++) {
        resultat[i] = (*this)[i] * autre[i];
    }
    return resultat;
}

Matrice Vecteur::produitExterieur(const Vecteur& autre) const {
    // 1. crée une Matrice "resultat" de taille (getTaille() x autre.getTaille())
    // 2. pour chaque i (0 à getTaille()-1), pour chaque j (0 à autre.getTaille()-1) :
    //      resultat(i, j) = (*this)[i] * autre[j]
    // 3. renvoie resultat
    int n = getTaille();
    int m = autre.getTaille();
    Matrice resultat(n, m);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            resultat(i, j) = (*this)[i] * autre[j];
        }
    }
    return resultat;
}

Vecteur Vecteur::operator*(double k) const {
    // resultat[i] = (*this)[i] * k, pour chaque i
    int n = getTaille();
    Vecteur resultat(n);
    for (int i = 0; i < n; i++) {
        resultat[i] = (*this)[i] * k;
    }
    return resultat;
}

Vecteur Vecteur::operator-(const Vecteur& autre) const {
    // resultat[i] = (*this)[i] - autre[i], pour chaque i
    int n = getTaille();
    Vecteur resultat(n);
    for (int i = 0; i < n; i++) {
        resultat[i] = (*this)[i] - autre[i];
    }
    return resultat;
}