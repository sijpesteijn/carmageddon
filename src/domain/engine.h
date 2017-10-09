//
// Created by Gijs Sijpesteijn on 06/10/2017.
//

#ifndef CARMAGEDDON_ENGINE_H
#define CARMAGEDDON_ENGINE_H

#include "pwm.h"

class Engine {
public:
    Engine();
    int setThrottle(int throttle);
    int getThrottle();
    void setEnable(int enable);
private:
    PWM pwm;
    int throttle = 0;
};


#endif //CARMAGEDDON_ENGINE_H
