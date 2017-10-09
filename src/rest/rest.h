//
// Created by Gijs Sijpesteijn on 06/10/2017.
//

#ifndef CARMAGEDDON_REST_H
#define CARMAGEDDON_REST_H

#include <memory>
#include <restbed>
#include <string>
#include <list>
#include "carmageddon_resource.h"

using namespace restbed;
using namespace std;

class Rest {
public:
    Rest(list<carmageddon_resource*> resources);
private:
    Service service;
};

#endif //CARMAGEDDON_REST_H
