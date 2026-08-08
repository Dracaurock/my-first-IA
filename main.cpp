#include "Vecteur.h"
#include "Matrice.h"

int main() {
    Vecteur v1(4);
    v1[0] = 1.0;
    v1[1] = 2.0;
    v1[2] = 3.0;
    v1[3] = 4.0;

    Vecteur v2(4);
    v2[0] = 4.0;
    v2[1] = 5.0;
    v2[2] = 6.0;
    v2[3] = 7.0;  // This line will cause an out-of-bounds error since v2 has size 3

    Matrice m(2, 3);
    m(0,0) = 1; m(0,1) = 2; m(0,2) = 3;
    m(1,0) = 4; m(1,1) = 5; m(1,2) = 1;

    Matrice m2(2, 3);
    m2(0,0) = 7; m2(0,1) = 8; m2(0,2) = 9;
    m2(1,0) = 10; m2(1,1) = 11; m2(1,2) = 12;

    Matrice m3 = m + m2;
    m3.afficher();  // attendu : [8, 10, 12]

    // Vecteur v3 = v1 + v2;
    // v3.afficher();  // attendu : [5, 7, 9]

    Vecteur v(3);
    v[0] = 1.0;
    v[1] = 0.0;
    v[2] = 1.0;

    // Vecteur resultat = m * v;
    // resultat.afficher();  // attendu : [4, 10]

    return 0;
}