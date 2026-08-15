#include "ReseauDeNeurones.h"
#include "Donnees.h"
#include <iostream>

int main() {
    std::cout << "Chargement des donnees..." << std::endl;
    std::vector<ExempleEntrainement> exemplesEntrainement = chargerMNIST("mnist_medium.csv");
    std::vector<ExempleEntrainement> exemplesTest = chargerMNIST("mnist_test_medium.csv");
    std::cout << "Entrainement : " << exemplesEntrainement.size() << " exemples" << std::endl;
    std::cout << "Test : " << exemplesTest.size() << " exemples" << std::endl;

    ReseauDeNeurones reseau;
    reseau.ajouterCouche(784, 128);
    reseau.ajouterCouche(128, 10);

    int nbEpoques = 30;
    double tauxApprentissage = 0.1;

    std::cout << "Entrainement..." << std::endl;
    for (int epoque = 0; epoque < nbEpoques; epoque++) {
        for (const auto& exemple : exemplesEntrainement) {
            reseau.entrainerSurExemple(exemple.entree, exemple.cible, tauxApprentissage);
        }

        if ((epoque + 1) % 5 == 0) {  // toutes les 5 époques
            double precisionTest = reseau.evaluerPrecision(exemplesTest);
            std::cout << "Epoque " << epoque + 1 << "/" << nbEpoques << " - precision test : " << precisionTest * 100 << "%" << std::endl;
        }
    }

    double precisionTrain = reseau.evaluerPrecision(exemplesEntrainement);
    double precisionTest = reseau.evaluerPrecision(exemplesTest);

    std::cout << "Precision sur entrainement : " << precisionTrain * 100 << "%" << std::endl;
    std::cout << "Precision sur test (jamais vu) : " << precisionTest * 100 << "%" << std::endl;

    reseau.sauvegarder("poids_mnist.txt");

    return 0;
}