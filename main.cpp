#include "ReseauDeNeurones.h"
#include "Donnees.h"
#include <iostream>

int main() {
    std::vector<ExempleEntrainement> exemples = chargerCSV("xor_data.csv", 2);

    ReseauDeNeurones reseau;
    reseau.ajouterCouche(2, 4);
    reseau.ajouterCouche(4, 1);

    for (int epoque = 0; epoque < 10000; epoque++) {
        for (const auto& exemple : exemples) {
            reseau.entrainerSurExemple(exemple.entree, exemple.cible, 0.5);
        }
    }

    std::cout << "--- Avant sauvegarde ---" << std::endl;
    for (const auto& exemple : exemples) {
        reseau.propagerAvant(exemple.entree).afficher();
    }

    reseau.sauvegarder("poids_xor.txt");

    // Nouveau réseau VIDE, sans entraînement, qui va juste charger les poids
    ReseauDeNeurones reseauCharge;
    reseauCharge.charger("poids_xor.txt");

    std::cout << "--- Apres chargement (nouveau reseau, sans entrainement) ---" << std::endl;
    for (const auto& exemple : exemples) {
        reseauCharge.propagerAvant(exemple.entree).afficher();
    }

    return 0;
}