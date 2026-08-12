#include "ReseauDeNeurones.h"
#include "Donnees.h"
#include <iostream>

int main() {
    // 1. Charger les données depuis le CSV
    std::vector<ExempleEntrainement> exemples = chargerCSV("xor_data.csv", 2);
    // "2" = les 2 premières colonnes sont l'entrée, le reste (1 colonne) est la cible

    std::cout << "Nombre d'exemples charges : " << exemples.size() << std::endl;

    // 2. Construire le réseau (structure identique à avant)
    ReseauDeNeurones reseau;
    reseau.ajouterCouche(2, 4);
    reseau.ajouterCouche(4, 1);

    // 3. Boucle d'entraînement générique
    int nbEpoques = 10000;
    double tauxApprentissage = 0.5;

    for (int epoque = 0; epoque < nbEpoques; epoque++) {
        for (const auto& exemple : exemples) {
            reseau.entrainerSurExemple(exemple.entree, exemple.cible, tauxApprentissage);
        }
    }

    // 4. Test final
    for (const auto& exemple : exemples) {
        Vecteur sortie = reseau.propagerAvant(exemple.entree);
        std::cout << "entree=[" << exemple.entree[0] << "," << exemple.entree[1] << "] -> ";
        sortie.afficher();
    }

    return 0;
}