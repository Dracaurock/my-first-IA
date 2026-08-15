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

        // TODO 1 : découpe "ligne" par virgule,
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

Vecteur encoderOneHot(int label, int nbClasses) {
    // TODO :
    // 1. crée un Vecteur "resultat" de taille nbClasses, initialisé à 0 (déjà le cas par défaut)
    // 2. mets resultat[label] à 1.0
    // 3. renvoie resultat
    Vecteur resultat(nbClasses);
    resultat[label] = 1.0;
    return resultat;
}

std::vector<ExempleEntrainement> chargerMNIST(const std::string& chemin) {
    std::vector<ExempleEntrainement> exemples;
    std::ifstream fichier(chemin);
    std::string ligne;

    while (std::getline(fichier, ligne)) {
        std::stringstream ss(ligne);
        std::string valeurTexte;
        std::vector<double> valeurs;

        while (std::getline(ss, valeurTexte, ',')) {
            try {
                valeurs.push_back(std::stod(valeurTexte));
            } catch (const std::invalid_argument& e) {
                std::cerr << "Erreur de conversion : " << valeurTexte << std::endl;
            }
        }

        if (valeurs.size() != 785) {
            std::cerr << "Ligne invalide, ignoree (taille=" << valeurs.size() << ")" << std::endl;
            continue;  // "continue" passe directement à la ligne suivante du while, sans faire le reste
        }

        // TODO 1 : le label est valeurs[0] -- convertis-le en int, encode-le en one-hot (10 classes)
        int label = static_cast<int>(valeurs[0]);
        Vecteur cible = encoderOneHot(label, 10);

        // TODO 2 : les 784 valeurs suivantes (valeurs[1] à valeurs[784]) sont les pixels
        //          crée un Vecteur "entree" de taille 784, rempli avec CHAQUE PIXEL DIVISE PAR 255.0
        //          (normalisation : ramène les valeurs de [0,255] à [0,1])
        Vecteur entree(784);
        for (int i = 0; i < 784; ++i) {
            entree[i] = valeurs[i + 1] / 255.0;
        }

        // TODO 3 : crée un ExempleEntrainement avec entree et le vecteur one-hot, ajoute-le à exemples
        ExempleEntrainement exemple{entree, cible};
        exemples.push_back(exemple);
    }

    return exemples;
}