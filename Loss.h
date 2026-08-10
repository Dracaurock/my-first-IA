#ifndef LOSS_H
#define LOSS_H

#include "Vecteur.h"

// Calcule l'erreur quadratique moyenne entre prediction et cible
double mse(const Vecteur& prediction, const Vecteur& cible);

// Calcule la dérivée de la MSE par rapport à chaque composante de prediction
// Renvoie un VECTEUR (une dérivée par composante), pas un seul nombre
Vecteur mse_derivee(const Vecteur& prediction, const Vecteur& cible);

#endif