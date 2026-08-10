#include "Couche.h"
#include "Activations.h"

Couche::Couche(int n_entree, int n_sortie)
    : W(n_sortie, n_entree), b(n_sortie)  
{
    // rien à faire de plus ici pour l'instant
}

Vecteur Couche::propagerAvant(const Vecteur& entree) const {
    // TODO :
    // 1. calcule z = W * entree + b
    // 2. applique sigmoide à z
    // 3. renvoie le résultat
    // indice : tu as TOUS les opérateurs nécessaires déjà écrits (Matrice*Vecteur, Vecteur+Vecteur)
    //          et la fonction sigmoide(Vecteur) de Activations.h (n'oublie pas de l'inclure !)
    Vecteur z = W * entree + b;
    return sigmoide(z);
}