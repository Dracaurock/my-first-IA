#include "ReseauDeNeurones.h"
#include "Vecteur.h"

int main() {
    ReseauDeNeurones reseau;
    reseau.ajouterCouche(3, 4);  // 3 entrées -> 4 neurones
    reseau.ajouterCouche(4, 2);  // 4 -> 2 neurones
    reseau.ajouterCouche(2, 1);  // 2 -> 1 sortie

    Vecteur entree(3);
    entree[0] = 1.0;
    entree[1] = 0.5;
    entree[2] = -1.0;

    Vecteur sortie = reseau.propagerAvant(entree);
    sortie.afficher();  // un seul nombre entre 0 et 1 (à cause de sigmoide)

    return 0;
}