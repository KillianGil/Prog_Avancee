//
// Created by jules - on 16/10/2023.
//

#ifndef UNTITLED_HOSPITAL_H
#define UNTITLED_HOSPITAL_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

class Hospital {
public:
    Hospital(int capacity);
    void StartSimulation(int patientCount);

private:
    std::mutex mtx;
    std::condition_variable patientReady;
    std::condition_variable doctorReady;
    int queueCapacity;
    std::queue<int> queue;

    void Patient(int id);
    void Doctor();
    static void getHeal(int patientId);
    static void balk(int patientId);

    int test = 0;
};


#endif //UNTITLED_HOSPITAL_H
