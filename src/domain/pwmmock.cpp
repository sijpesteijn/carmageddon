//
// Created by Gijs Sijpesteijn on 06/10/2017.
//

#import "pwm.h"
#include <syslog.h>

PWM::PWM(string pwmNr) {
    this->pwmNr = pwmNr;
}

PWM::PWM(string pwmNr, int period, int duty_cycle) {
    this->pwmNr = pwmNr;
    this->setPeriod(period);
    this->setDutyCycle(duty_cycle);
}

void PWM::setPeriod(int period) {
    this->period = period;
    syslog(LOG_DEBUG, "Period set to %d", this->period);
}

void PWM::setDutyCycle(int duty_cycle) {
    this->duty_cycle = duty_cycle;
    syslog(LOG_DEBUG, "Duty cycle set to %d", this->duty_cycle);
}

void PWM::setEnable(int enable) {
    this->enable = enable;
    syslog(LOG_DEBUG, "Enable set to %d", this->enable);
}

int PWM::getEnabled() {
    return this->enable;
}
