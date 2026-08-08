#include "Matrice.h"
#include <iostream>

Matrice::Matrice(int lignes, int colonnes) {
    // TODO :
    // 1. stocke "lignes" dans nbLignes, "colonnes" dans nbColonnes
    // 2. redimensionne "donnees" à la bonne taille (combien d'éléments au total ?), rempli de 0.0
    nbLignes = lignes;
    nbColonnes = colonnes;
    donnees.resize(lignes * colonnes, 0.0);
}

int Matrice::getLignes() const {
    // TODO : renvoie nbLignes
    return nbLignes;
}

int Matrice::getColonnes() const {
    // TODO : renvoie nbColonnes
    return nbColonnes;
}

// Version modifiable (référence)
double& Matrice::operator()(int i, int j) {
    // TODO : calcule l'index à plat, renvoie donnees[index]
    int index = i * nbColonnes + j;
    return donnees[index];
}

// Version lecture seule (const)
double Matrice::operator()(int i, int j) const {
    // TODO : même calcul, mais on renvoie une copie (pas une référence)
    // Note : dans une méthode const, on ne peut pas modifier les attributs
    int index = i * nbColonnes + j;
    return donnees[index]; 
}

Vecteur Matrice::operator*(const Vecteur& v) const {
    // TODO :
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
    // TODO :
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
    // TODO : affiche quelque chose comme :
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