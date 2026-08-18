#include "ReseauDeNeurones.h"
#include "ChargeurImage.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage : predire.exe chemin_vers_image.png" << std::endl;
        return 1;
    }

    std::string cheminImage = argv[1];

    ReseauDeNeurones reseau;
    reseau.charger("poids_mnist.txt");

    Vecteur entree = chargerImageChiffre(cheminImage);
    afficherASCII(entree);
    Vecteur sortie = reseau.propagerAvant(entree);

    std::cout << "Probabilites par chiffre :" << std::endl;
    for (int i = 0; i < 10; i++) {
        std::cout << i << " : " << sortie[i] * 100 << "%" << std::endl;
    }

    int chiffrePredit = 0;
    double maxValeur = sortie[0];
    for (int i = 1; i < 10; i++) {
        if (sortie[i] > maxValeur) {
            maxValeur = sortie[i];
            chiffrePredit = i;
        }
    }

    std::cout << "\n=> Chiffre predit : " << chiffrePredit << std::endl;

    return 0;
}