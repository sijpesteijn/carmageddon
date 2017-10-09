//
// Created by Gijs Sijpesteijn on 06/10/2017.
//

#include "pwm.h"
#include <syslog.h>
#include <map>

map<string, string> pwmMap = {
        { "P9.21", "/sys/class/pwm/pwmchip0/pwm1/"},
        { "P9.22", "/sys/class/pwm/pwmchip0/pwm0/"}
};

PWM::PWM(string pwmNr) {
    this->pwmNr = pwmMap[pwmNr];
    this->setPeriod(20000000);
    this->setDutyCycle(1000000);
}

PWM::PWM(string pwmNr, int period, int duty_cycle) {
    this->pwmNr = pwmMap[pwmNr];
    this->setPeriod(period);
    this->setDutyCycle(duty_cycle);
}

void PWM::setPeriod(int period) {
    this->period = period;
    string cmd = "echo " + to_string(this->period) + " > " + this->pwmNr + "period";
    system(cmd.c_str());
    syslog(LOG_DEBUG, "Period set to %d", this->period);
}

void PWM::setDutyCycle(int duty_cycle) {
    this->duty_cycle = duty_cycle;
    string cmd = "echo " + to_string(this->duty_cycle) + " > " + this->pwmNr + "duty_cycle";
    system(cmd.c_str());
    syslog(LOG_DEBUG, "Duty cycle set to %d ", this->duty_cycle);
}

void PWM::setEnable(int enable) {
    this->enable = enable;
    string cmd = "echo " + to_string(this->enable) + " > " + this->pwmNr + "enable";
    system(cmd.c_str());
}

int PWM::getEnabled() {
    return this->enable;
}
