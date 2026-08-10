#include "Couche.h"
#include "Activations.h"
#include <random>   // n'oublie pas cet include

Couche::Couche(int n_entree, int n_sortie)
    : W(n_sortie, n_entree), b(n_sortie)
{
    // TODO :
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