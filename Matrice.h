#ifndef MATRICE_H
#define MATRICE_H

#include <vector>
#include "Vecteur.h"

class Matrice {
private:
    std::vector<double> donnees;  // stockage "à plat" (row-major)
    int nbLignes;
    int nbColonnes;

public:
    // Constructeur : crée une matrice lignes x colonnes, remplie de 0.0
    Matrice(int lignes, int colonnes);

    int getLignes() const;
    int getColonnes() const;

    // Accès à l'élément (i, j) — équivalent 2D de operator[]
    double& operator()(int i, int j);

    // Version "lecture seule" (utile plus tard, on y reviendra si besoin)
    double operator()(int i, int j) const;

    // Produit matriciel : Matrice * Vecteur (essentiel pour le forward pass)
    Vecteur operator*(const Vecteur& v) const;

    // Addition de deux matrices (utile pour certaines opérations d'apprentissage)
    Matrice operator+(const Matrice& autre) const;

    // Renvoie la transposée de cette matrice (lignes/colonnes inversées)
    Matrice transposee() const;

    void afficher() const;

    // Multiplication par un scalaire : chaque élément * k
    Matrice operator*(double k) const;

    // Soustraction terme à terme
    Matrice operator-(const Matrice& autre) const;
};

#endif