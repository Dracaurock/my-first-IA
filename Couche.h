#ifndef COUCHE_H
#define COUCHE_H

#include "Matrice.h"
#include "Vecteur.h"

class Couche {
private:
    Matrice W;   // poids : n_sortie x n_entree
    Vecteur b;   // biais : n_sortie

public:
    // Construit une couche avec n_entree neurones en entrée, n_sortie en sortie
    Couche(int n_entree, int n_sortie);

    // Propagation avant : calcule activation(W * entree + b)
    // On commence avec sigmoide "en dur", on rendra ça flexible plus tard si besoin
    Vecteur propagerAvant(const Vecteur& entree) const;
};

#endif