#include "ReseauDeNeurones.h"
#include <fstream>
#include <thread>

void ReseauDeNeurones::ajouterCouche(int n_entree, int n_sortie) {
    // crée une Couche(n_entree, n_sortie) et ajoute-la à "couches"
    // std::vector a une méthode .push_back(element) qui ajoute à la fin
    couches.push_back(Couche(n_entree, n_sortie));
}

Vecteur ReseauDeNeurones::propagerAvant(const Vecteur& entree) const{
    // 1. commence avec "entree" comme valeur courante
    // 2. pour chaque couche du réseau (dans l'ordre) : 
    //      remplace la valeur courante par couche.propagerAvant(valeur_courante)
    // 3. renvoie la valeur finale (sortie de la dernière couche)
    // indice : boucle "for (const Couche& c : couches)" -> boucle "for each" (range-based for)
    Vecteur valeur_courante = entree;
    for (const Couche& c : couches){
        valeur_courante = c.propagerAvant(valeur_courante);
    }
    return valeur_courante;
}

GradientsReseau ReseauDeNeurones::calculerGradientsExemple(const Vecteur& entree, const Vecteur& cible) const {
    // TODO 1 : fais le forward pass "à la main" (PAS avec propagerAvant, car on a besoin
    //          du détail de CHAQUE couche) :
    //          - garde un Vecteur "valeur_courante" qui commence à "entree"
    //          - pour chaque couche : appelle couche.propagerAvantDetail(valeur_courante),
    //            stocke le résultat (SortieCouche a un .z et un .activation) dans un
    //            std::vector<SortieCouche> "sorties" (une par couche)
    //          - mets à jour valeur_courante = sorties.back().activation pour la couche suivante
    //          indice : sorties.back() donne le DERNIER élément d'un vector
    Vecteur valeur_courante = entree;
    std::vector<SortieCouche> sorties;
    for (const Couche& c : couches) {
        SortieCouche sortie = c.propagerAvantDetail(valeur_courante);
        sorties.push_back(sortie);
        valeur_courante = sortie.activation;
    }
    // TODO 2 : backward pass, comme avant mais on ACCUMULE dans un GradientsReseau au lieu
    //          d'appliquer directement :
    //          - crée GradientsReseau gradients(couches.size());  <- vector de la bonne taille d'un coup
    //          - boucle for (int i = couches.size()-1; i >= 0; i--) à l'envers, comme avant
    //          - l'entrée de la couche i : "entree" si i==0, sinon sorties[i-1].activation
    //          - appelle couches[i].calculerGradients(entree_de_cette_couche, sorties[i].z,
    //                       deltaCourant, W_suivante, estSortie, cible)
    //          - stocke le résultat dans gradients[i], et deltaCourant = gradients[i].delta pour la suite
    GradientsReseau gradients(couches.size());
    Vecteur deltaCourant(0);
    for (int i = static_cast<int>(couches.size()) - 1; i >= 0; i--) {
        bool estSortie = (i == static_cast<int>(couches.size()) - 1);
        Vecteur entree_couche = (i == 0) ? entree : sorties[i - 1].activation;
        Matrice W_suivante = (estSortie) ? Matrice(0, 0) : couches[i + 1].getW();

        gradients[i] = couches[i].calculerGradients(entree_couche, sorties[i].z,
                                                    deltaCourant, W_suivante,
                                                    estSortie, cible);
        deltaCourant = gradients[i].delta;
    }
    // TODO 3 : renvoie gradients
    return gradients;

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

double ReseauDeNeurones::evaluerPrecision(const std::vector<ExempleEntrainement>& exemples) const {
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

void ReseauDeNeurones::entrainerSurBatch(const std::vector<ExempleEntrainement>& batch, double tauxApprentissage) {
    int tailleBatch = static_cast<int>(batch.size());
    std::vector<GradientsReseau> resultats(tailleBatch);

    // TODO : remplace la création manuelle de std::thread par une boucle qui appelle
    //        pool.ajouterTache([&, i]() { resultats[i] = calculerGradientsExemple(batch[i].entree, batch[i].cible); });
    //        pour chaque i, PUIS pool.attendreFin() (remplace la boucle de .join())
    for (int i = 0; i < tailleBatch; i++) {
        threadPool.ajouterTache([&, i]() {
            resultats[i] = calculerGradientsExemple(batch[i].entree, batch[i].cible);
        });
    }
    threadPool.attendreFin();

    // Fait la moyenne des gradients de chaque couche, sur tout le batch
    for (size_t i = 0; i < couches.size(); i++) {
        Matrice sommeGradW = resultats[0][i].gradW;
        Vecteur sommeGradb = resultats[0][i].gradb;

        for (int ex = 1; ex < tailleBatch; ex++) {
            sommeGradW = sommeGradW + resultats[ex][i].gradW;
            sommeGradb = sommeGradb + resultats[ex][i].gradb;
        }

        Matrice moyenneGradW = sommeGradW * (1.0 / tailleBatch);
        Vecteur moyenneGradb = sommeGradb * (1.0 / tailleBatch);

        couches[i].appliquerGradients(moyenneGradW, moyenneGradb, tauxApprentissage);
    }
}

ReseauDeNeurones::ReseauDeNeurones(int nbThreads) : threadPool(nbThreads) {}