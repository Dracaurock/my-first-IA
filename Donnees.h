#ifndef DONNEES_H
#define DONNEES_H

#include <vector>
#include <string>
#include "Vecteur.h"

// Représente un exemple d'entraînement : une entrée et sa cible attendue
struct ExempleEntrainement {
    Vecteur entree;
    Vecteur cible;
};

// Lit un fichier CSV et renvoie la liste des exemples.
// nbColonnesEntree : combien des premières colonnes sont des features (le reste = cibles)
std::vector<ExempleEntrainement> chargerCSV(const std::string& chemin, int nbColonnesEntree);

// Transforme un label (ex: 7) en vecteur one-hot de taille nbClasses (ex: [0,0,0,0,0,0,0,1,0,0])
Vecteur encoderOneHot(int label, int nbClasses);

std::vector<ExempleEntrainement> chargerMNIST(const std::string& chemin);

#endif