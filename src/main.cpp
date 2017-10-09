#include <syslog.h>
#include <iostream>
#include <list>
#include "cpu.h"
#include "rest/rest.h"
#include "rest/observer_resource.h"
#include "rest/camera_resource.h"
#include "rest/car_resource.h"

int main( const int, const char** )
{
    openlog ("Carmageddon", LOG_PID | LOG_CONS | LOG_NDELAY | LOG_NOWAIT, LOG_LOCAL0);
    setlogmask(LOG_UPTO(LOG_DEBUG));
    syslog (LOG_INFO, "%s", "Starting Carmageddon");


    Car car;
    Camera camera;

    camera_resource camera_res(&camera);
    car_resource car_res(&car);
    lifeline_handler ll_handler(&car);
    cpu *carmageddon = new cpu(&camera, &car, &ll_handler);
    observer_resource observer_res(carmageddon);

    Rest rest({&camera_res, &car_res, &ll_handler, &observer_res});
    return EXIT_SUCCESS;
}
