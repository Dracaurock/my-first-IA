#include "ChargeurImage.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <algorithm>

Vecteur chargerImageChiffre(const std::string& chemin) {
    int largeur, hauteur, canaux;
    unsigned char* pixels = stbi_load(chemin.c_str(), &largeur, &hauteur, &canaux, 1);

    if (pixels == nullptr) {
        std::cerr << "Erreur : impossible de charger l'image " << chemin << std::endl;
        return Vecteur(784);
    }

    std::cout << "Image chargee : " << largeur << "x" << hauteur << " pixels" << std::endl;

    // Trouve la boite englobante du chiffre (pixels suffisamment sombres = pas le fond)
    int xMin = largeur, xMax = 0, yMin = hauteur, yMax = 0;

    // Trouve min/max de TOUTE l'image d'abord, pour calibrer un seuil adaptatif
    unsigned char minBrut = 255, maxBrut = 0;
    for (int i = 0; i < largeur * hauteur; i++) {
        if (pixels[i] < minBrut) minBrut = pixels[i];
        if (pixels[i] > maxBrut) maxBrut = pixels[i];
    }
    unsigned char seuil = (minBrut + maxBrut) / 2;  // seuil = milieu entre le plus sombre et le plus clair
    std::cout << "Brut min = " << (int)minBrut << ", max = " << (int)maxBrut << ", seuil = " << (int)seuil << std::endl;

    for (int y = 0; y < hauteur; y++) {
        for (int x = 0; x < largeur; x++) {
            unsigned char valeur = pixels[y * largeur + x];
            if (valeur < seuil) {
                if (x < xMin) xMin = x;
                if (x > xMax) xMax = x;
                if (y < yMin) yMin = y;
                if (y > yMax) yMax = y;
            }
        }
    }

    // Ajoute une petite marge (10% de la taille de la boite) pour ne pas couper le trait pile au bord
    int margeX = (xMax - xMin) / 10;
    int margeY = (yMax - yMin) / 10;
    xMin = std::max(0, xMin - margeX);
    xMax = std::min(largeur - 1, xMax + margeX);
    yMin = std::max(0, yMin - margeY);
    yMax = std::min(hauteur - 1, yMax + margeY);

    int largeurBoite = xMax - xMin + 1;
    int hauteurBoite = yMax - yMin + 1;
    std::cout << "Boite englobante : x[" << xMin << "-" << xMax << "] y[" << yMin << "-" << yMax
               << "] (image totale : " << largeur << "x" << hauteur << ")" << std::endl;

    // Redimensionne EN CONSERVANT LES PROPORTIONS : le plus grand cote de la boite
    // devient 20 pixels (comme dans le vrai MNIST), puis on CENTRE ce resultat dans
    // le cadre 28x28 -- au lieu d'etirer betement pour remplir tout l'espace, ce qui
    // deformait les chiffres (notamment ceux qui ne sont pas carres, comme "1").
    int plusGrandCote = std::max(largeurBoite, hauteurBoite);
    double echelle = 20.0 / plusGrandCote;

    int nouvelleLargeur = static_cast<int>(largeurBoite * echelle);
    int nouvelleHauteur = static_cast<int>(hauteurBoite * echelle);
    if (nouvelleLargeur < 1) nouvelleLargeur = 1;
    if (nouvelleHauteur < 1) nouvelleHauteur = 1;

    int decalageX = (28 - nouvelleLargeur) / 2;
    int decalageY = (28 - nouvelleHauteur) / 2;

    // Initialise tout le cadre a "clair" (255 = sera interprete comme fond apres binarisation)
    double pixelsNormalises[28 * 28];
    for (int i = 0; i < 784; i++) pixelsNormalises[i] = 255.0;

    // Dessine le chiffre redimensionne (proportions conservees) au centre du cadre
    for (int y = 0; y < nouvelleHauteur; ++y) {
        for (int x = 0; x < nouvelleLargeur; ++x) {
            int srcX = xMin + x * largeurBoite / nouvelleLargeur;
            int srcY = yMin + y * hauteurBoite / nouvelleHauteur;
            unsigned char valeurGris = pixels[srcY * largeur + srcX];  // toujours indexe sur largeur totale !

            int destX = decalageX + x;
            int destY = decalageY + y;
            if (destX >= 0 && destX < 28 && destY >= 0 && destY < 28) {
                pixelsNormalises[destY * 28 + destX] = static_cast<double>(valeurGris);
            }
        }
    }

    // Binarisation : chaque pixel devient soit 1.0 (trait) soit 0.0 (fond), selon le seuil
    // (evite le bruit de texture qu'un etirement de contraste continu laisserait passer)
    Vecteur resultat(784);
    for (int i = 0; i < 784; ++i) {
        resultat[i] = (pixelsNormalises[i] < seuil) ? 1.0 : 0.0;
    }

    stbi_image_free(pixels);

    return resultat;
}

void afficherASCII(const Vecteur& image28x28) {
    const std::string niveaux = " .:-=+*#%@";
    for (int y = 0; y < 28; y++) {
        for (int x = 0; x < 28; x++) {
            double valeur = image28x28[y * 28 + x];
            int index = static_cast<int>(valeur * (niveaux.size() - 1));
            std::cout << niveaux[index];
        }
        std::cout << std::endl;
    }
}