#include "Matrice.h"
#include <iostream>

Matrice::Matrice(int lignes, int colonnes) {
    // 1. stocke "lignes" dans nbLignes, "colonnes" dans nbColonnes
    // 2. redimensionne "donnees" à la bonne taille (combien d'éléments au total ?), rempli de 0.0
    nbLignes = lignes;
    nbColonnes = colonnes;
    donnees.resize(lignes * colonnes, 0.0);
}

int Matrice::getLignes() const {
    // renvoie nbLignes
    return nbLignes;
}

int Matrice::getColonnes() const {
    // renvoie nbColonnes
    return nbColonnes;
}

// Version modifiable (référence)
double& Matrice::operator()(int i, int j) {
    // calcule l'index à plat, renvoie donnees[index]
    int index = i * nbColonnes + j;
    return donnees[index];
}

// Version lecture seule (const)
double Matrice::operator()(int i, int j) const {
    // même calcul, mais on renvoie une copie (pas une référence)
    int index = i * nbColonnes + j;
    return donnees[index]; 
}

Vecteur Matrice::operator*(const Vecteur& v) const {
    // 1. crée un Vecteur "resultat" de taille nbLignes
    // 2. pour chaque ligne i (boucle externe) :
    //      pour chaque colonne j (boucle interne) :
    //        additionne (*this)(i,j) * v[j] dans resultat[i]
    // 3. renvoie resultat
    Vecteur resultat(nbLignes);
    for (int i=0; i<nbLignes; i++){
        for (int j=0; j<nbColonnes; j++){
            resultat[i] += (*this)(i,j) * v[j];
        }
    }
    return resultat;
}

Matrice Matrice::operator+(const Matrice& autre) const {
    // 1. crée une Matrice "resultat" de même taille (nbLignes x nbColonnes)
    // 2. pour chaque case (i, j) : resultat(i,j) = (*this)(i,j) + autre(i,j)
    // 3. renvoie resultat
    Matrice resultat(nbLignes, nbColonnes);
    for (int i=0; i<nbLignes; i++){
        for (int j=0; j<nbColonnes; j++){
            resultat(i,j) = (*this)(i,j) + autre(i,j);
        }
    }
    return resultat;
}

void Matrice::afficher() const {
    // affiche quelque chose comme :
    // [1, 2, 3]
    // [4, 5, 6]
    // indice : boucle externe sur les lignes, boucle interne sur les colonnes,
    // std::endl à la fin de chaque ligne
    for (int i=0; i<nbLignes; i++){
        std::cout << "[";
        for (int j=0; j<nbColonnes; j++){
            std::cout << (*this)(i,j);
            if (j < nbColonnes - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "]" << std::endl;
    }
}

Matrice Matrice::transposee() const {
    // 1. crée une Matrice "resultat" de taille (nbColonnes x nbLignes) -- attention, c'est INVERSÉ !
    // 2. pour chaque i (lignes de *this), pour chaque j (colonnes de *this) :
    //      resultat(j, i) = (*this)(i, j)     <- remarque l'inversion i/j dans "resultat"
    // 3. renvoie resultat
    Matrice resultat(nbColonnes, nbLignes);
    for (int i=0; i<nbLignes; i++){
        for (int j=0; j<nbColonnes; j++){
            resultat(j,i) = (*this)(i,j);
        }
    }
    return resultat;
}

Matrice Matrice::operator*(double k) const {
    // resultat(i,j) = (*this)(i,j) * k, pour chaque i,j
    Matrice resultat(nbLignes, nbColonnes);
    for (int i=0; i<nbLignes; i++){
        for (int j=0; j<nbColonnes; j++){
            resultat(i,j) = (*this)(i,j) * k;
        }
    }
    return resultat;
}

Matrice Matrice::operator-(const Matrice& autre) const {
    //resultat(i,j) = (*this)(i,j) - autre(i,j), pour chaque i,j
    Matrice resultat(nbLignes, nbColonnes);
    for (int i=0; i<nbLignes; i++){
        for (int j=0; j<nbColonnes; j++){
            resultat(i,j) = (*this)(i,j) - autre(i,j);
        }
    }
    return resultat;
}