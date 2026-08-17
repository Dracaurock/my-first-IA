#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

class ThreadPool {
private:
    std::vector<std::thread> workers;         // les threads permanents du pool
    std::queue<std::function<void()>> taches; // file d'attente des tâches à exécuter

    std::mutex mutexQueue;                    // protège l'accès à "taches"
    std::condition_variable condition;        // pour réveiller les workers quand il y a du travail

    int tachesEnCours = 0;                    // compte les tâches pas encore terminées
    std::condition_variable conditionFin;     // pour signaler "toutes les tâches sont finies"
    std::mutex mutexFin;

    bool arret = false;                       // signal pour arrêter proprement les threads

public:
    ThreadPool(int nbThreads) {
        for (int i = 0; i < nbThreads; i++) {
            workers.push_back(std::thread([this]() { boucleWorker(); }));
        }
    }

    ~ThreadPool() {
        {
            std::lock_guard<std::mutex> lock(mutexQueue);
            arret = true;
        }
        condition.notify_all();
        for (auto& w : workers) w.join();
    }

    void boucleWorker() {
        while (true) {
            std::function<void()> tache;
            {
                std::unique_lock<std::mutex> lock(mutexQueue);
                condition.wait(lock, [this]() { return arret || !taches.empty(); });
                if (arret && taches.empty()) return;
                tache = taches.front();
                taches.pop();
            }
            tache();  // exécute la tâche EN DEHORS du verrou (important pour la performance)
            {
                std::lock_guard<std::mutex> lock(mutexFin);
                tachesEnCours--;
                if (tachesEnCours == 0) conditionFin.notify_all();
            }
        }
    }

    void ajouterTache(std::function<void()> tache) {
        {
            std::lock_guard<std::mutex> lock(mutexQueue);
            taches.push(tache);
        }
        {
            std::lock_guard<std::mutex> lock(mutexFin);
            tachesEnCours++;
        }
        condition.notify_one();
    }

    void attendreFin() {
        std::unique_lock<std::mutex> lock(mutexFin);
        conditionFin.wait(lock, [this]() { return tachesEnCours == 0; });
    }
};

#endif