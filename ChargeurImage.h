#ifndef CHARGEUR_IMAGE_H
#define CHARGEUR_IMAGE_H

#include "Vecteur.h"
#include <string>

// Charge une image depuis un fichier, la convertit en Vecteur de 784 valeurs (28x28, normalisé),
// prêt à être donné à ReseauDeNeurones::propagerAvant()
Vecteur chargerImageChiffre(const std::string& chemin);

// ChargeurImage.h -- ajoute cette déclaration
void afficherASCII(const Vecteur& image28x28);
#endif