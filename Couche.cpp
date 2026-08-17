#include "Couche.h"
#include "Activations.h"
#include "Loss.h"
#include <random>

Couche::Couche(int n_entree, int n_sortie)
    : W(n_sortie, n_entree), b(n_sortie)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-1.0, 1.0);
    for (int i = 0; i < W.getLignes(); i++)
        for (int j = 0; j < W.getColonnes(); j++)
            W(i, j) = dis(gen);
    for (int i = 0; i < b.getTaille(); i++)
        b[i] = dis(gen);
}

SortieCouche Couche::propagerAvantDetail(const Vecteur& entree) const {
    Vecteur z = W * entree + b;
    return SortieCouche{ z, sigmoide(z) };
}

Vecteur Couche::propagerAvant(const Vecteur& entree) const {
    return propagerAvantDetail(entree).activation;
}

GradientsCouche Couche::calculerGradients(const Vecteur& entree, const Vecteur& z,
                                           const Vecteur& deltaSuivant, const Matrice& W_suivante,
                                           bool estCoucheDeSortie, const Vecteur& cible) const {
    // TODO : EXACTEMENT la même logique que dans l'ancien retropropager(), MAIS :
    // - utilise le paramètre "z" au lieu de "dernierZ"
    // - utilise le paramètre "entree" au lieu de "derniereEntree"
    // - NE FAIS PAS la mise à jour de W et b ici (plus de "W = W - ...")
    // - renvoie un GradientsCouche{ delta, gradW, gradb } au lieu de juste delta

    Vecteur delta(0);
    if (estCoucheDeSortie) {
        delta = mse_derivee(sigmoide(z), cible).hadamard(sigmoide_derivee(z));
    } else {
        delta = (W_suivante.transposee() * deltaSuivant).hadamard(sigmoide_derivee(z));
    }
    Matrice gradW = delta.produitExterieur(entree);
    Vecteur gradb = delta;
    return GradientsCouche{ delta, gradW, gradb };
}

void Couche::appliquerGradients(const Matrice& gradW, const Vecteur& gradb, double tauxApprentissage) {
    W = W - gradW * tauxApprentissage;
    b = b - gradb * tauxApprentissage;
}

const Vecteur& Couche::getB() const { return b; }
const Matrice& Couche::getW() const { return W; }
void Couche::definirPoids(const Matrice& nouveauW, const Vecteur& nouveauB) { W = nouveauW; b = nouveauB; }