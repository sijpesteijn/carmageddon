//
// Created by Gijs Sijpesteijn on 06/10/2017.
//

#ifndef CARMAGEDDON_STEER_H
#define CARMAGEDDON_STEER_H

#include "pwm.h"

class Steer {
public:
    Steer();
    int setAngle(int angle);
    int getAngle();
    void setEnable(int enable);
    int getEnabled();
private:
    PWM pwm;
    int angle = 0;
};


#endif //CARMAGEDDON_STEER_H
