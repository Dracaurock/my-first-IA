// Petit utilitaire de DIAGNOSTIC : prend une ligne du CSV MNIST (deja normalise 0-255,
// label en premiere colonne) et la sauvegarde en PNG, pour pouvoir la faire passer
// dans predire.exe et verifier que le pipeline fonctionne sur une image "propre".
//
// Compilation :
//   g++ -std=c++17 -Wall csvVersPng.cpp -o csvVersPng
//
// Usage :
//   ./csvVersPng mnist_test_medium.csv 0 sortie.png
//   (0 = numero de ligne a extraire, en partant de 0)

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage : csvVersPng chemin.csv numeroLigne sortie.png" << std::endl;
        return 1;
    }

    std::string cheminCsv = argv[1];
    int numeroLigne = std::stoi(argv[2]);
    std::string cheminSortie = argv[3];

    std::ifstream fichier(cheminCsv);
    std::string ligne;
    int compteur = 0;

    while (std::getline(fichier, ligne)) {
        if (compteur == numeroLigne) {
            std::stringstream ss(ligne);
            std::string valeurTexte;
            std::vector<int> valeurs;

            while (std::getline(ss, valeurTexte, ',')) {
                valeurs.push_back(std::stoi(valeurTexte));
            }

            if (valeurs.size() != 785) {
                std::cerr << "Ligne invalide (taille=" << valeurs.size() << ")" << std::endl;
                return 1;
            }

            int label = valeurs[0];
            std::cout << "Label reel de cette image : " << label << std::endl;

            unsigned char image[784];
            for (int i = 0; i < 784; i++) {
                image[i] = static_cast<unsigned char>(255 - valeurs[i + 1]);  // inverse : simule fond clair / encre sombre
            }

            stbi_write_png(cheminSortie.c_str(), 28, 28, 1, image, 28);
            std::cout << "Image sauvegardee : " << cheminSortie << std::endl;
            return 0;
        }
        compteur++;
    }

    std::cerr << "Ligne " << numeroLigne << " introuvable dans le fichier." << std::endl;
    return 1;
}