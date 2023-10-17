#include "hospital.h"

int main() {
    // On lance un hôpital avec une salle d'attente de 3 places
    Hospital hospital(3);

    // On lance la simulation avec 10 patients au total
    hospital.StartSimulation(10);

    return 0;
}
