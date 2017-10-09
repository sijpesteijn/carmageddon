//
// Created by Gijs Sijpesteijn on 06/10/2017.
//

#ifndef CARMAGEDDON_LIFELINE_HANDLER_H
#define CARMAGEDDON_LIFELINE_HANDLER_H
#include <restbed>
#include "../domain/car.h"
#include "carmageddon_resource.h"

using namespace restbed;
using namespace std;

class lifeline_handler: public carmageddon_resource {
public:
    lifeline_handler(Car *car);
    list<shared_ptr<Resource>> getResources();
    void notifyClients();
private:
    shared_ptr<Resource> resource;
};


#endif //CARMAGEDDON_LIFELINE_HANDLER_H
