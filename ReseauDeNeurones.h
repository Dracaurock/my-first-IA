#ifndef RESEAU_DE_NEURONES_H
#define RESEAU_DE_NEURONES_H

#include <vector>
#include <string>
#include "Couche.h"
#include "Vecteur.h"
#include "Donnees.h"
#include "ThreadPool.h"

// Gradients pour tout le réseau, un GradientsCouche par couche
using GradientsReseau = std::vector<GradientsCouche>;

class ReseauDeNeurones {
private:
    std::vector<Couche> couches;
    ThreadPool threadPool;  // pool de threads pour l'entraînement parallèle

public:
    ReseauDeNeurones(int nbThreads = std::thread::hardware_concurrency());
    void ajouterCouche(int n_entree, int n_sortie);
    Vecteur propagerAvant(const Vecteur& entree) const;  // redevient const !

    // Calcule les gradients de TOUTES les couches pour UN exemple (thread-safe : aucune modification)
    GradientsReseau calculerGradientsExemple(const Vecteur& entree, const Vecteur& cible) const;

    // Entraîne sur un batch d'exemples EN PARALLELE (un thread par exemple), puis moyenne + applique
    void entrainerSurBatch(const std::vector<ExempleEntrainement>& batch, double tauxApprentissage);

    double evaluerPrecision(const std::vector<ExempleEntrainement>& exemples) const;  // redevient const !

    void sauvegarder(const std::string& chemin) const;
    void charger(const std::string& chemin);
};

#endif