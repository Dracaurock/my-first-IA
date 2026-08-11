#include "Couche.h"
#include "Activations.h"
#include "Loss.h"
#include <random>

Couche::Couche(int n_entree, int n_sortie)
    : W(n_sortie, n_entree), b(n_sortie),derniereEntree(n_entree), dernierZ(n_sortie)
{
    // 1. crée un générateur aléatoire (random_device + mt19937 + uniform_real_distribution(-1.0, 1.0))
    // 2. remplis W avec des valeurs aléatoires (boucle sur i = lignes, j = colonnes)
    // 3. remplis b avec des valeurs aléatoires (boucle sur i = taille)
    std::random_device rd;  // source d'entropie
    std::mt19937 gen(rd()); // générateur de nombres pseudo-aléatoires
    std::uniform_real_distribution<double> dis(-1.0, 1.0); // distribution uniforme entre -1 et 1
    for (int i=0; i<W.getLignes();i++){
        for (int j=0; j<W.getColonnes();j++){
            W(i,j) = dis(gen);
        }
    }
    for (int i=0; i<b.getTaille();i++){
        b[i] = dis(gen);
    }
}

Vecteur Couche::propagerAvant(const Vecteur& entree) {
    // 1. mémorise "entree" dans this->derniereEntree
    // 2. calcule z = W * entree + b, mémorise-le dans this->dernierZ
    // 3. calcule et renvoie sigmoide(z) 
    Vecteur z = W * entree + b;
    derniereEntree = entree;
    dernierZ = z;
    return sigmoide(z);
}

const Vecteur& Couche::getDerniereEntree() const {
    return derniereEntree;
}
const Vecteur& Couche::getDernierZ() const {
    return dernierZ;
}
const Matrice& Couche::getW() const {
    return W;
}

Vecteur Couche::retropropager(const Vecteur& deltaSuivant, const Matrice& W_suivante,double tauxApprentissage, bool estCoucheDeSortie, const Vecteur& cible) {
    Vecteur delta(0);  // valeur temporaire, sera remplacée en dessous

    if (estCoucheDeSortie) {
        // TODO : calcule delta pour le cas "couche de sortie"
        // indice : sigmoide(dernierZ) te donne la sortie de cette couche
        delta = mse_derivee(sigmoide(dernierZ), cible).hadamard(sigmoide_derivee(dernierZ));
    } else {
        // TODO : calcule delta pour le cas "couche cachée"
        // indice : W_suivante.transposee() * deltaSuivant, puis hadamard avec sigmoide_derivee(dernierZ)
        delta = (W_suivante.transposee() * deltaSuivant).hadamard(sigmoide_derivee(dernierZ));
    }

    // TODO : calcule gradient_W et gradient_b, puis met à jour W et b
    // TODO : renvoie delta
    Matrice gradient_W = delta.produitExterieur(derniereEntree);
    Vecteur gradient_b = delta;
    W = W - gradient_W * tauxApprentissage;
    b = b - gradient_b * tauxApprentissage;
    return delta;
}