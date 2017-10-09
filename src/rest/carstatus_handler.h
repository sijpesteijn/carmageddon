//
// Created by Gijs Sijpesteijn on 06/10/2017.
//

#ifndef CARMAGEDDON_CARSTATUS_HANDLER_H
#define CARMAGEDDON_CARSTATUS_HANDLER_H


#include <restbed>
#include "../domain/car.h"

using namespace restbed;
using namespace std;

class carstatus_handler {
public:
    carstatus_handler(Car *car);
    shared_ptr<Resource> getResource();
private:
    shared_ptr<Resource> resource;
};

#endif //CARMAGEDDON_CARSTATUS_HANDLER_H
