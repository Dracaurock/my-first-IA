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

    int nbEpoques = 60;
    double tauxApprentissage = 1.0;

    std::cout << "Entrainement..." << std::endl;
    int tailleBatch = 32;
    for (int epoque = 0; epoque < nbEpoques; epoque++) {
        for (size_t i = 0; i < exemplesEntrainement.size(); i += tailleBatch) {
            // Prend un sous-ensemble (batch) de la liste d'exemples
            size_t fin = std::min(i + tailleBatch, exemplesEntrainement.size());
            std::vector<ExempleEntrainement> batch(exemplesEntrainement.begin() + i, exemplesEntrainement.begin() + fin);

            reseau.entrainerSurBatch(batch, tauxApprentissage);
        }

        if ((epoque + 1) % 5 == 0) {
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