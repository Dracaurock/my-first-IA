#include "ReseauDeNeurones.h"


void ReseauDeNeurones::ajouterCouche(int n_entree, int n_sortie) {
    // crée une Couche(n_entree, n_sortie) et ajoute-la à "couches"
    // std::vector a une méthode .push_back(element) qui ajoute à la fin
    couches.push_back(Couche(n_entree, n_sortie));
}

Vecteur ReseauDeNeurones::propagerAvant(const Vecteur& entree) {
    // 1. commence avec "entree" comme valeur courante
    // 2. pour chaque couche du réseau (dans l'ordre) : 
    //      remplace la valeur courante par couche.propagerAvant(valeur_courante)
    // 3. renvoie la valeur finale (sortie de la dernière couche)
    // indice : boucle "for (const Couche& c : couches)" -> boucle "for each" (range-based for)
    Vecteur valeur_courante = entree;
    for (Couche& c : couches){
        valeur_courante = c.propagerAvant(valeur_courante);
    }
    return valeur_courante;
}

void ReseauDeNeurones::entrainerSurExemple(const Vecteur& entree, const Vecteur& cible, double tauxApprentissage) {
    // 1. Forward pass (remplit derniereEntree/dernierZ dans chaque couche)
    propagerAvant(entree);

    // 2. Backward pass : on remonte les couches à l'envers
    Vecteur deltaCourant(0);  // sera mis à jour à chaque itération

    for (int i = static_cast<int>(couches.size()) - 1; i >= 0; i--) {
        bool estSortie = (i == static_cast<int>(couches.size()) - 1);

        if (estSortie) {
            // TODO : appelle couches[i].retropropager(...) avec estCoucheDeSortie=true
            //        deltaSuivant et W_suivante ne sont pas utilisés dans ce cas,
            //        tu peux passer deltaCourant et couches[i].getW() (peu importe, non utilisés)
            //        stocke le résultat dans deltaCourant
            deltaCourant = couches[i].retropropager(deltaCourant, couches[i].getW(), tauxApprentissage, true, cible);

        } else {
            // TODO : appelle couches[i].retropropager(...) avec estCoucheDeSortie=false
            //        deltaSuivant = deltaCourant (celui calculé à l'itération précédente)
            //        W_suivante = couches[i+1].getW()
            //        cible n'est pas utilisée dans ce cas, tu peux passer "cible" quand même (ignorée)
            //        stocke le résultat dans deltaCourant
            deltaCourant = couches[i].retropropager(deltaCourant, couches[i + 1].getW(), tauxApprentissage, false, cible);
        }
    }
}