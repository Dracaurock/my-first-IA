#ifndef COUCHE_H
#define COUCHE_H

#include "Matrice.h"
#include "Vecteur.h"

// Résultat détaillé d'un forward pass : on a besoin de "z" (avant activation) pour le backward
struct SortieCouche {
    Vecteur z;
    Vecteur activation;
};

struct GradientsCouche {
    Vecteur delta;
    Matrice gradW;
    Vecteur gradb;

    // Constructeur par défaut : nécessaire pour que std::vector<GradientsCouche> puisse
    // créer des éléments "vides" (ex: GradientsReseau gradients(couches.size());)
    GradientsCouche() : delta(0), gradW(0, 0), gradb(0) {}

    // Constructeur avec les 3 valeurs (utilisé quand on écrit GradientsCouche{delta, gradW, gradb})
    GradientsCouche(const Vecteur& d, const Matrice& gW, const Vecteur& gb)
        : delta(d), gradW(gW), gradb(gb) {}
};

class Couche {
private:
    Matrice W;
    Vecteur b;

public:
    Couche(int n_entree, int n_sortie);

    // Entièrement CONST maintenant : ne modifie plus rien, donc utilisable en parallèle sans risque
    Vecteur propagerAvant(const Vecteur& entree) const;
    SortieCouche propagerAvantDetail(const Vecteur& entree) const;

    // Calcule le delta + les gradients, SANS toucher à W/b (pur calcul, lecture seule -> const, donc thread-safe)
    GradientsCouche calculerGradients(const Vecteur& entree, const Vecteur& z,
                                       const Vecteur& deltaSuivant, const Matrice& W_suivante,
                                       bool estCoucheDeSortie, const Vecteur& cible) const;

    // Applique un gradient (déjà calculé, éventuellement moyenné sur un batch) aux poids
    void appliquerGradients(const Matrice& gradW, const Vecteur& gradb, double tauxApprentissage);

    const Vecteur& getB() const;
    void definirPoids(const Matrice& nouveauW, const Vecteur& nouveauB);
    const Matrice& getW() const;
};

#endif