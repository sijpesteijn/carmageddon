//
// Created by Gijs Sijpesteijn on 06/10/2017.
//

#ifndef CARMAGEDDON_CPU_H
#define CARMAGEDDON_CPU_H

#include "./domain/observer.h"
#include "./domain/camera.h"
#include "./domain/car.h"
#include "rest/lifeline_handler.h"
#include <map>
#include <list>
#include <string>
using namespace std;

class cpu {
private:
    map<string, observer*> observers;
public:
    Camera *camera;
    Car *car;
    lifeline_handler *ll_handler;
    cpu(Camera *camera, Car *car, lifeline_handler *ll_handler);
    list<observer*> getObservers();
    observer* getObserver(string name);
};

#endif //CARMAGEDDON_CPU_H
