//
// Created by Gijs Sijpesteijn on 06/10/2017.
//

#include "car.h"
#include <syslog.h>


Car::Car() {
    this->enabled = 0;
    this->mode = car_mode::stopped;
}

int Car::setMode(car_mode status) {
    if (this->enabled == 1) {
        this->mode = status;
        syslog(LOG_DEBUG, "Mode set to: %d", this->mode);
        return 0;
    }
    syslog(LOG_ERR, "%s", "Car not enabled");
    return -1;
}

int Car::setAngle(int angle) {
    if (this->enabled == 1) {
        this->steer.setAngle(angle);
        syslog(LOG_DEBUG, "Angle set to: %d", angle);
        return 0;
    }
    syslog(LOG_ERR, "%s", "Car not enabled.");
    return -1;
}

int Car::setThrottle(int throttle) {
    if (this->enabled == 1) {
        this->engine.setThrottle(throttle);
        syslog(LOG_DEBUG, "Throttle set to: %d", throttle);
        return 0;
    }
    syslog(LOG_ERR, "%s", "Car not enabled.");
    return -1;
}

void Car::setEnabled(int enabled) {
    syslog(LOG_DEBUG, "Car enabled set to: %d", enabled);
    this->enabled = enabled;
    if (this->enabled == 0) {
        this->mode = car_mode::stopped;
    }
    this->steer.setEnable(this->enabled);
    this->engine.setEnable(this->enabled);
}

car_mode Car::getMode() {
    return this->mode;
}

int Car::getAngle() {
    return this->steer.getAngle();
}

int Car::getThrottle() {
    return this->engine.getThrottle();
}

int Car::getEnabled() {
    return this->enabled;
}