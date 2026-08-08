#include "Vecteur.h"
#include <iostream>

Vecteur::Vecteur(int taille) {
    // TODO : initialise "donnees" avec "taille" éléments à 0.0
    donnees.resize(taille, 0.0);
}

int Vecteur::getTaille() const {
    // TODO
    //conversion de type implicite de size_t à int
    return static_cast<int>(donnees.size());
}

double& Vecteur::operator[](int index) {
    // TODO : renvoie une référence vers donnees[index]
    return donnees[index];
}

double Vecteur::operator[](int index) const {
    // TODO : renvoie une référence vers donnees[index]
    return donnees[index];
}


Vecteur Vecteur::operator+(const Vecteur& autre) const {
    // TODO :
    // 1. crée un nouveau Vecteur "resultat" de la bonne taille
    // 2. remplis-le avec la somme composante par composante
    // 3. renvoie-le
    int taille = getTaille();
    Vecteur resultat(taille);
    for (int i=0; i<taille; i++){
        resultat[i] = donnees[i] + autre.donnees[i];
    }
    return resultat;
}

void Vecteur::afficher() const {
    // TODO : affiche le vecteur, par exemple sous la forme [1, 2, 3]
    // indice : std::cout << "..." ; et une boucle for
    std::cout << "[";
    for (int i=0; i<getTaille(); i++){
        std::cout << donnees[i];
        if (i < getTaille() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
}