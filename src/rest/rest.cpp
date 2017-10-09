//
// Created by Gijs Sijpesteijn on 06/10/2017.
//

#include "rest.h"
#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

Rest::Rest(list<carmageddon_resource*> resources) {
    auto settings = make_shared< Settings >( );
    settings->set_port( 1984 );
    settings->set_default_header( "Connection", "close" );

    for ( carmageddon_resource *carma_resource: resources) {
        for ( shared_ptr<Resource> resource : carma_resource->getResources()) {
            this->service.publish( resource );
        }
    }
    this->service.start( settings );
}
