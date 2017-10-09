//
// Created by Gijs Sijpesteijn on 06/10/2017.
//

#include "cpu.h"
#include "./observers/traffic_light.h"
#include "./observers/lane_detection.h"
#include <syslog.h>
#include <unistd.h>

void* checkObservers(void* params) {
    cpu *carmageddon = (cpu*) params;
    Mat snapshot = carmageddon->camera->takeSnapshot();
    while(1) {
        list<observer*> observers = {};
        for (auto const& i : carmageddon->getObservers()) {
            if (i->isActive()) {
                observers.push_back(i->processSnapshot(snapshot));
            }
        }
        carmageddon->ll_handler->notifyClients();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    return NULL;
}

cpu::cpu(Camera *camera, Car *car, lifeline_handler *ll_handler) {
    this->camera = camera;
    this->car = car;
    this->ll_handler = ll_handler;
    traffic_light *tl = new traffic_light(this->camera->getDimensions());
    this->observers.insert(pair<string, observer*>(tl->getType(), tl));
    lane_detection *ld = new lane_detection(this->camera->getDimensions());
    this->observers.insert(pair<string, observer*>(ld->getType(), ld));
    pthread_t observer_runner;
    pthread_create(&observer_runner, NULL, checkObservers, this);
}

list<observer*> cpu::getObservers() {
    list<observer*> observers = {};
    for(map<string,observer*>::iterator it = this->observers.begin(); it != this->observers.end(); ++it) {
        observers.push_back(it->second);
    }
    return observers;
}

observer* cpu::getObserver(string name) {
    map<string,observer*>::iterator it = this->observers.find(name);
    if (it == this->observers.end()) {
        syslog(LOG_ERR, "CPU: Could not get observer %s.", name.c_str());
        return NULL;
    } else {
        return it->second;
    }
}