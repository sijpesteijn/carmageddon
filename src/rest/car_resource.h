//
// Created by Gijs Sijpesteijn on 06/10/2017.
//

#ifndef CARMAGEDDON_CAR_RESOURCE_H
#define CARMAGEDDON_CAR_RESOURCE_H

#include "../domain/car.h"
#include <restbed>
#include <list>
#include "carmageddon_resource.h"

using namespace restbed;
using namespace std;

class car_resource: public carmageddon_resource {
public:
    car_resource(Car *car);
    list<shared_ptr<Resource>> getResources();
private:
    shared_ptr<Resource> carGetModeResource = make_shared< Resource >( );
    shared_ptr<Resource> carPostModeResource = make_shared< Resource >( );
    shared_ptr<Resource> steerResource = make_shared< Resource >( );
    shared_ptr<Resource> engineResource = make_shared< Resource >( );
    shared_ptr<Settings> settings = make_shared< Settings >( );
};

#endif //CARMAGEDDON_CAR_RESOURCE_H
