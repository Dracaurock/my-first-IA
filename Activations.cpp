#include "Activations.h"
#include <cmath>
#include <algorithm>

double sigmoide(double x) {
    // TODO : implémente 1 / (1 + e^(-x))
    return 1.0 / (1.0 + std::exp(-x));
}

double sigmoide_derivee(double x) {
    // TODO : implémente sigmoide(x) * (1 - sigmoide(x))
    // indice : tu peux appeler la fonction sigmoide() que tu viens d'écrire
    double s = sigmoide(x);
    return s * (1.0 - s);
}

double relu(double x) {
    // TODO : renvoie x si x > 0, sinon 0
    return std::max(0.0, x);
}

double relu_derivee(double x) {
    // TODO : renvoie 1.0 si x > 0, sinon 0.0
    return (x > 0.0) ? 1.0 : 0.0;
}

Vecteur sigmoide(const Vecteur& v) {
    // TODO :
    // 1. crée un Vecteur "resultat" de même taille que v
    // 2. pour chaque élément i : resultat[i] = sigmoide(v[i])  <- appelle la version "double" !
    // 3. renvoie resultat
    Vecteur resultat(v.getTaille());
    for (int i= 0; i<v.getTaille(); i++){
        resultat[i] = sigmoide(v[i]);
    }
    return resultat;
}

Vecteur relu(const Vecteur& v) {
    // TODO : même principe avec relu()
    Vecteur resultat(v.getTaille());
    for (int i= 0; i<v.getTaille(); i++){
        resultat[i] = relu(v[i]);
    }
    return resultat;
}

Vecteur sigmoide_derivee(const Vecteur& v) {
    // TODO :
    // 1. crée un Vecteur "resultat" de même taille que v
    // 2. pour chaque élément i : resultat[i] = sigmoide_derivee(v[i])  <- appelle la version "double" !
    // 3. renvoie resultat
    Vecteur resultat(v.getTaille());
    for (int i= 0; i<v.getTaille(); i++){
        resultat[i] = sigmoide_derivee(v[i]);
    }
    return resultat;
}

Vecteur relu_derivee(const Vecteur& v) {
    // TODO : même principe avec relu_derivee()
    Vecteur resultat(v.getTaille());
    for (int i= 0; i<v.getTaille(); i++){
        resultat[i] = relu_derivee(v[i]);
    }
    return resultat;
}