#include "ReseauDeNeurones.h"
#include <fstream>

void ReseauDeNeurones::ajouterCouche(int n_entree, int n_sortie) {
    // crée une Couche(n_entree, n_sortie) et ajoute-la à "couches"
    // std::vector a une méthode .push_back(element) qui ajoute à la fin
    couches.push_back(Couche(n_entree, n_sortie));
}

Vecteur ReseauDeNeurones::propagerAvant(const Vecteur& entree) {
    // 1. commence avec "entree" comme valeur courante
    // 2. pour chaque couche du réseau (dans l'ordre) : 
    //      remplace la valeur courante par couche.propagerAvant(valeur_courante)
    // 3. renvoie la valeur finale (sortie de la dernière couche)
    // indice : boucle "for (const Couche& c : couches)" -> boucle "for each" (range-based for)
    Vecteur valeur_courante = entree;
    for (Couche& c : couches){
        valeur_courante = c.propagerAvant(valeur_courante);
    }
    return valeur_courante;
}

void ReseauDeNeurones::entrainerSurExemple(const Vecteur& entree, const Vecteur& cible, double tauxApprentissage) {
    // 1. Forward pass (remplit derniereEntree/dernierZ dans chaque couche)
    propagerAvant(entree);

    // 2. Backward pass : on remonte les couches à l'envers
    Vecteur deltaCourant(0);  // sera mis à jour à chaque itération

    for (int i = static_cast<int>(couches.size()) - 1; i >= 0; i--) {
        bool estSortie = (i == static_cast<int>(couches.size()) - 1);

        if (estSortie) {
            // TODO : appelle couches[i].retropropager(...) avec estCoucheDeSortie=true
            //        deltaSuivant et W_suivante ne sont pas utilisés dans ce cas,
            //        tu peux passer deltaCourant et couches[i].getW() (peu importe, non utilisés)
            //        stocke le résultat dans deltaCourant
            deltaCourant = couches[i].retropropager(deltaCourant, couches[i].getW(), tauxApprentissage, true, cible);

        } else {
            // TODO : appelle couches[i].retropropager(...) avec estCoucheDeSortie=false
            //        deltaSuivant = deltaCourant (celui calculé à l'itération précédente)
            //        W_suivante = couches[i+1].getW()
            //        cible n'est pas utilisée dans ce cas, tu peux passer "cible" quand même (ignorée)
            //        stocke le résultat dans deltaCourant
            deltaCourant = couches[i].retropropager(deltaCourant, couches[i + 1].getW(), tauxApprentissage, false, cible);
        }
    }
}

void ReseauDeNeurones::sauvegarder(const std::string& chemin) const {
    std::ofstream fichier(chemin);

    // TODO 1 : écris le nombre de couches (couches.size()) sur la première ligne
    fichier << couches.size() << std::endl;

    for (const auto& c : couches) {
        const Matrice& W = c.getW();
        const Vecteur& b = c.getB();

        // TODO 2 : écris "W.getLignes() W.getColonnes()" (séparés par un espace)
        fichier << W.getLignes() << " " << W.getColonnes() << std::endl;
        // TODO 3 : écris toutes les valeurs de W (boucle i,j comme dans Matrice::afficher,
        //          mais avec "fichier <<" au lieu de "std::cout <<")
        for (int i=0; i<W.getLignes(); i++){
            for (int j=0; j<W.getColonnes(); j++){
                fichier << W(i,j) << " ";
            }
            fichier << std::endl;
        }
        // TODO 4 : écris toutes les valeurs de b (boucle simple)
        for (int i=0; i<b.getTaille(); i++){
            fichier << b[i] << " ";
        }
        fichier << std::endl;
    }
}

void ReseauDeNeurones::charger(const std::string& chemin) {
    std::ifstream fichier(chemin);

    couches.clear();  // vide le réseau actuel

    int nbCouches;
    fichier >> nbCouches;

    for (int c = 0; c < nbCouches; c++) {
        int n_sortie, n_entree;
        fichier >> n_sortie >> n_entree;

        ajouterCouche(n_entree, n_sortie);  // crée la couche (poids aléatoires pour l'instant)

        // TODO 1 : lis toutes les valeurs de W depuis le fichier (n_sortie x n_entree valeurs)
        //          et remplis une Matrice W_chargee
        Matrice W_chargee(n_sortie, n_entree);
        for (int i=0; i<n_sortie; i++){
            for (int j=0; j<n_entree;j++){
                double valeur;
                fichier >> valeur;
                W_chargee(i,j) = valeur;
            }
        }
        // TODO 2 : lis toutes les valeurs de b depuis le fichier (n_sortie valeurs)
        //          et remplis un Vecteur b_charge avec
        Vecteur b_charge(n_sortie);
        for (int i=0; i<n_sortie; i++){
            double valeur;
            fichier >> valeur;
            b_charge[i] = valeur;
        }
        // TODO 3 : appelle couches[c].definirPoids(W_chargee, b_charge)
        //          pour remplacer les poids aléatoires par ceux chargés
        couches[c].definirPoids(W_chargee, b_charge);
    }
}

double ReseauDeNeurones::evaluerPrecision(const std::vector<ExempleEntrainement>& exemples){
    // TODO :
    // 1. pour chaque exemple : fait propagerAvant(exemple.entree)
    //    (attention : propagerAvant n'est plus const... donc problème potentiel, vois plus bas)
    // 2. trouve l'indice du max de la sortie -> chiffrePredit
    // 3. trouve l'indice du max de exemple.cible -> chiffreReel (même logique que pour trouver le max de sortie,
    //    puisqu'un one-hot a sa plus grande valeur -1.0- exactement à la bonne position)
    // 4. compte le nombre de bonnes prédictions
    // 5. renvoie (nombre_correct / nombre_total_exemples), en double !
    int correct = 0;
    for (const auto& exemple : exemples) {
        Vecteur sortie = propagerAvant(exemple.entree);

        // Trouve l'indice de la plus grande valeur en sortie (= chiffre prédit)
        int chiffrePredit = 0;
        double maxValeur = sortie[0];
        for (int i = 1; i < 10; i++) {
            if (sortie[i] > maxValeur) {
                maxValeur = sortie[i];
                chiffrePredit = i;
            }
        }

        // Trouve le vrai chiffre (indice du 1 dans le one-hot de la cible)
        int chiffreReel = 0;
        for (int i = 0; i < 10; i++) {
            if (exemple.cible[i] == 1.0) {
                chiffreReel = i;
                break;
            }
        }

        if (chiffrePredit == chiffreReel) correct++;
    }

    return static_cast<double>(correct) / exemples.size();

}