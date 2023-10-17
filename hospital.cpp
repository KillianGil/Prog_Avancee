#include "hospital.h"
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <thread>
#include <queue>

// Constructeur de l'hôpital
Hospital::Hospital(int capacity) : queueCapacity(capacity) {}

// Méthode pour ajouter un patient dans la salle d'attente avec un id
void Hospital::Patient(int id) {
    // On bloque le mutex pour faire en sorte que le patient ne soit pas interrompu dans sa tâche
    std::unique_lock<std::mutex> lock(mtx);

    // Si la salle d'attente n'est pas pleine, on ajoute le patient
    if (queue.size() < queueCapacity) {
        queue.push(id);
        std::cout << "Patient " << id << " est dans la salle d'attente." << std::endl;
        patientReady.notify_one();
    }
    // Sinon, le patient n'est pas pris en charge
    else {
        balk(id);
    }

    // On laisse un temps pour que le patient s'installe dans la salle d'attente (entre 100 et 400ms)
    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 300 + 100));

    // On débloque le mutex pour que le patient suivant puisse être traité
    lock.unlock();
}

// Méthode pour traiter un patient
void Hospital::Doctor() {
    while (true) {
        // On bloque le mutex pour faire en sorte que le patient ne soit pas interrompu dans sa tâche
        std::unique_lock<std::mutex> lock(mtx);

        // On attend soit disponible dans la salle d'attente
        patientReady.wait(lock, [this] { return !queue.empty(); });

        // On récupère le patient
        int id = queue.front();

        // On le supprime de la salle d'attente (le premier arrivé est le premier traité)
        // pop() retire le premier élément de la queue parce que cela fonctionne comme une file d'attente
        queue.pop();

        // On a guéri le patient
        getHeal(id);

        // On débloque le mutex pour que le patient suivant puisse être traité
        lock.unlock();

        // On notifie le patient suivant qu'il peut entrer dans le cabinet
        patientReady.notify_one();
    }
}

void Hospital::StartSimulation(int patientCount) {
    // Initialisation du thread pour le docteur
    std::thread doctorThread(&Hospital::Doctor, this);

    // Initialisation du vecteur de thread pour tous les patients
    std::vector<std::thread> patients;

    // On boucle pour tous les patients
    for (int currentPatientIndex = 0; currentPatientIndex < patientCount; ++currentPatientIndex) {
        // On attend entre 100 et 300
        std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 300 + 100)); // Attente aléatoire entre l'arrivée des patients

        // On ajoute le thread du patient dans le vecteur
        patients.emplace_back(&Hospital::Patient, this, currentPatientIndex);
    }

    // On attend que tous les patients aient fini
    for (int i = 0; i < patientCount; ++i) {
        patients[i].join();
    }

    // On attend que le docteur ait fini
    doctorThread.join();
}

// Méthode pour guérir un patient en faisant une pause de 1 seconde
void Hospital::getHeal(int patientId) {
    std::cout << "Le médecin traite le patient " << patientId << "." << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::cout << "Le patient " << patientId << " a été traité." << std::endl;
}

// Méthode pour refuser un patient
void Hospital::balk(int patientId) {
    std::cout << "La salle d'attente est pleine, patient " << patientId << " partira." << std::endl;
}