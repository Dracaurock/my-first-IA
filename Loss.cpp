#include "Loss.h"

double mse(const Vecteur& prediction, const Vecteur& cible) {
    // 1. calcule la somme des (prediction[i] - cible[i])² pour tout i
    // 2. divise par la taille (n)
    // 3. renvoie le résultat
    double somme = 0.0;
    int n = prediction.getTaille();
    for (int i=0; i<n; i++){
        double diff = prediction[i] - cible[i];
        somme += diff * diff;
    }
    return somme / static_cast<double>(n);
}

Vecteur mse_derivee(const Vecteur& prediction, const Vecteur& cible) {
    // pour chaque i : resultat[i] = (2.0 / n) * (prediction[i] - cible[i])
    // renvoie le vecteur resultat
    int n = prediction.getTaille();
    Vecteur resultat(n);
    double coef = 2.0 / n;
    for (int i = 0; i < n; i++) {
        resultat[i] = coef * (prediction[i] - cible[i]);
    }
    return resultat;
}