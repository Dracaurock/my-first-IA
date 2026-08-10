#include "ReseauDeNeurones.h"


void ReseauDeNeurones::ajouterCouche(int n_entree, int n_sortie) {
    // TODO : crée une Couche(n_entree, n_sortie) et ajoute-la à "couches"
    // indice : std::vector a une méthode .push_back(element) qui ajoute à la fin
    couches.push_back(Couche(n_entree, n_sortie));
}

Vecteur ReseauDeNeurones::propagerAvant(const Vecteur& entree) const {
    // TODO :
    // 1. commence avec "entree" comme valeur courante
    // 2. pour chaque couche du réseau (dans l'ordre) : 
    //      remplace la valeur courante par couche.propagerAvant(valeur_courante)
    // 3. renvoie la valeur finale (sortie de la dernière couche)
    // indice : boucle "for (const Couche& c : couches)" -> boucle "for each" (range-based for)
    Vecteur valeur_courante = entree;
    for (const Couche& c : couches){
        valeur_courante = c.propagerAvant(valeur_courante);
    }
    return valeur_courante;
}