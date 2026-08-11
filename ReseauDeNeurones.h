#ifndef RESEAU_DE_NEURONES_H
#define RESEAU_DE_NEURONES_H

#include <vector>
#include "Couche.h"
#include "Vecteur.h"

class ReseauDeNeurones {
private:
    std::vector<Couche> couches;

public:
    // Ajoute une couche au réseau (à la fin de la pile)
    void ajouterCouche(int n_entree, int n_sortie);

    // Propagation avant à travers TOUTES les couches
    Vecteur propagerAvant(const Vecteur& entree);

    // Entraîne le réseau sur UN exemple (entrée, cible), avec un taux d'apprentissage donné
    void entrainerSurExemple(const Vecteur& entree, const Vecteur& cible, double tauxApprentissage);
};

#endif