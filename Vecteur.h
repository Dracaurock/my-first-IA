#ifndef VECTEUR_H  // "include guard" : évite que ce fichier soit inclus 2x (erreur de compilation sinon)
#define VECTEUR_H

#include <vector>   // on utilise std::vector pour stocker nos nombres

class Vecteur {
private:
    std::vector<double> donnees;  // les valeurs du vecteur (ex: [1.0, 2.5, -3.0])

public:
    // Constructeur : crée un vecteur de "taille" éléments, tous initialisés à 0
    Vecteur(int taille);

    // Renvoie le nombre d'éléments
    int getTaille() const;  
    // le "const" ici promet que la fonction ne modifie pas l'objet

    // Accès en lecture/écriture à un élément (comme vec[2])
    double& operator[](int index);
    double operator[](int index) const;  // version lecture seule (const)
    // "operator[]" permet de surcharger l'opérateur [] pour notre classe
    // le "&" (référence) permet de MODIFIER l'élément directement, sans copie

    // Addition de deux vecteurs
    Vecteur operator+(const Vecteur& autre) const;

    // Affiche le vecteur (pratique pour debug)
    void afficher() const;
};

#endif