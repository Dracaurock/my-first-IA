#include "ChargeurImage.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <vector>

Vecteur chargerImageChiffre(const std::string& chemin) {
    int largeur, hauteur, canaux;
    unsigned char* pixels = stbi_load(chemin.c_str(), &largeur, &hauteur, &canaux, 1);

    if (pixels == nullptr) {
        std::cerr << "Erreur : impossible de charger l'image " << chemin << std::endl;
        return Vecteur(784);
    }

    std::cout << "Image chargee : " << largeur << "x" << hauteur << " pixels" << std::endl;

    // TODO 1 : redimensionne en 28x28 (plus proche voisin)
    double pixelsNormalises[28 * 28];
    for (int y = 0; y < 28; ++y) {
        for (int x = 0; x < 28; ++x) {
            int srcX = x * largeur / 28;
            int srcY = y * hauteur / 28;
            unsigned char valeurGris = pixels[srcY * largeur + srcX];
            pixelsNormalises[y * 28 + x] = static_cast<double>(valeurGris);
        }
    }
    // Trouve min et max parmi les 784 valeurs
    double minVal = pixelsNormalises[0];
    double maxVal = pixelsNormalises[0];
    for (int i = 1; i < 784; i++) {
        if (pixelsNormalises[i] < minVal) minVal = pixelsNormalises[i];
        if (pixelsNormalises[i] > maxVal) maxVal = pixelsNormalises[i];
    }

    // TODO 2 : construis le Vecteur final (inversion + normalisation)
    Vecteur resultat(784);
    for (int i = 0; i < 784; ++i) {
        resultat[i] = 1.0 - ((pixelsNormalises[i] - minVal) / (maxVal - minVal)); // inversion et normalisation
    }

    stbi_image_free(pixels);

    // TODO 3 : renvoie le Vecteur final
    return resultat;
}

#include <iostream>

void afficherASCII(const Vecteur& image28x28) {
    const std::string niveaux = " .:-=+*#%@";  // du plus clair (espace) au plus foncé (@)
    for (int y = 0; y < 28; y++) {
        for (int x = 0; x < 28; x++) {
            double valeur = image28x28[y * 28 + x];  // entre 0.0 et 1.0
            int index = static_cast<int>(valeur * (niveaux.size() - 1));
            std::cout << niveaux[index];
        }
        std::cout << std::endl;
    }
}