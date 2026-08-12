#include "Donnees.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<ExempleEntrainement> chargerCSV(const std::string& chemin, int nbColonnesEntree) {
    std::vector<ExempleEntrainement> exemples;
    std::ifstream fichier(chemin);
    std::string ligne;

    while (std::getline(fichier, ligne)) {
        std::stringstream ss(ligne);
        std::string valeurTexte;
        std::vector<double> valeurs;  // toutes les valeurs de la ligne, en double

        // TODO 1 : découpe "ligne" par virgule (comme dans l'exemple montré plus haut),
        //          convertis chaque morceau en double avec std::stod, ajoute-le à "valeurs"
        while (std::getline(ss, valeurTexte, ',')) {
            try {
                double valeur = std::stod(valeurTexte);
                valeurs.push_back(valeur);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Erreur de conversion : " << valeurTexte << " n'est pas un nombre valide." << std::endl;
            }
        }

        // TODO 2 : sépare "valeurs" en deux Vecteur : entree (nbColonnesEntree premiers)
        //          et cible (le reste)
        //          crée un ExempleEntrainement avec ces deux Vecteur, ajoute-le à "exemples"
        if (valeurs.size() >= static_cast<size_t>(nbColonnesEntree)) {
            Vecteur entree(nbColonnesEntree);
            for (int i = 0; i < nbColonnesEntree; ++i) {
                entree[i] = valeurs[i];
            }

            Vecteur cible(static_cast<size_t>(valeurs.size()) - nbColonnesEntree);
            for (size_t i = nbColonnesEntree; i < valeurs.size(); ++i) {
                cible[i - nbColonnesEntree] = valeurs[i];
            }

            ExempleEntrainement exemple{entree, cible};
            exemples.push_back(exemple);
        } else {
            std::cerr << "Erreur : la ligne ne contient pas assez de colonnes pour l'entrée." << std::endl;
        }
    }

    return exemples;
}