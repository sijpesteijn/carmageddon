//
// Created by Gijs Sijpesteijn on 06/10/2017.
//

#ifndef CARMAGEDDON_CAMERA_RESOURCE_H
#define CARMAGEDDON_CAMERA_RESOURCE_H


#include "../domain/camera.h"
#include "carmageddon_resource.h"
#include <restbed>
#include <list>

using namespace std;
using namespace restbed;

class camera_resource: public carmageddon_resource {
public:
    camera_resource(Camera *camera);
    list<shared_ptr<Resource>> getResources();
private:
    shared_ptr<Resource> cameraSettingsResource = make_shared< Resource >( );
    shared_ptr<Resource> cameraGetDimensionsResource = make_shared< Resource >( );
    shared_ptr<Resource> cameraSnapshotResource = make_shared< Resource >( );

};

#endif //CARMAGEDDON_CAMERA_RESOURCE_H
