#ifndef COUCHE_H
#define COUCHE_H

#include "Matrice.h"
#include "Vecteur.h"

class Couche {
private:
    Matrice W;   // poids : n_sortie x n_entree
    Vecteur b;   // biais : n_sortie

    // on mémorise les valeurs du dernier forward pass pour le backward
    Vecteur derniereEntree;   // l'entrée reçue par cette couche
    Vecteur dernierZ;         // W * entree + b, avant activation

public:
    // Construit une couche avec n_entree neurones en entrée, n_sortie en sortie
    Couche(int n_entree, int n_sortie);

    // Propagation avant : calcule activation(W * entree + b)
    // On commence avec sigmoide "en dur", on rendra ça flexible plus tard si besoin
    Vecteur propagerAvant(const Vecteur& entree);

    // Le réseau peux récupérer ces valeurs pendant le backward
    const Vecteur& getDerniereEntree() const;
    const Vecteur& getDernierZ() const;
    const Matrice& getW() const;
    
    // Rétropropagation pour cette couche.
    // deltaSuivant : le delta de la couche SUIVANTE (celle d'après, vers la sortie)
    // W_suivante : la matrice de poids de la couche suivante (nécessaire pour calculer CE delta)
    // tauxApprentissage : pour la mise à jour des poids
    // estCoucheDeSortie : true si c'est la dernière couche (le calcul du delta est différent)
    // cible : utile seulement si c'est la couche de sortie (pour mse_derivee)
    //
    // Renvoie : le delta de CETTE couche (pour que le réseau le transmette à la couche précédente)
    Vecteur retropropager(const Vecteur& deltaSuivant, const Matrice& W_suivante,double tauxApprentissage, bool estCoucheDeSortie, const Vecteur& cible);

    const Vecteur& getB() const;

    // Remplace W et b par de nouvelles valeurs (utilisé lors du chargement depuis fichier)
    void definirPoids(const Matrice& nouveauW, const Vecteur& nouveauB);
};
#endif