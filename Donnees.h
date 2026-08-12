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

#endif