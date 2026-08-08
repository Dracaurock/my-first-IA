#ifndef ACTIVATIONS_H
#define ACTIVATIONS_H

double sigmoide(double x);
double sigmoide_derivee(double x);

double relu(double x);
double relu_derivee(double x);

#include "Vecteur.h"  // il faut inclure Vecteur.h puisqu'on l'utilise maintenant

// Applique sigmoide() à chaque élément du vecteur, renvoie un nouveau vecteur
Vecteur sigmoide(const Vecteur& v);

// Applique relu() à chaque élément du vecteur, renvoie un nouveau vecteur
Vecteur relu(const Vecteur& v);

#endif