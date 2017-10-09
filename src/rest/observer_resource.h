//
// Created by Gijs Sijpesteijn on 06/10/2017.
//

#ifndef CARMAGEDDON_OBSERVER_RESOURCE_H
#define CARMAGEDDON_OBSERVER_RESOURCE_H

#include <restbed>
#include <list>
#include "carmageddon_resource.h"
#include "../cpu.h"

using namespace restbed;
using namespace std;

class observer_resource: public carmageddon_resource {
public:
    observer_resource(cpu *carmageddon);
    list<shared_ptr<Resource>> getResources();
    cpu *carmageddon;
private:
    shared_ptr<Resource> observerPostResource = make_shared< Resource >( );
    shared_ptr<Resource> observerGetResource = make_shared< Resource >( );
};
#endif //CARMAGEDDON_OBSERVER_RESOURCE_H
