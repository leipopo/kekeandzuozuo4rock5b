#include "fanctrl.hpp"
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <math.h>
using namespace std;

int readfdate(const char *path)
{
    ifstream file;
    file.open(path, ios::in);
    int data;
    file >> data;
    file.close();
    return data;
}
void writefdate(const char *path, int data)
{
    ofstream file;
    file.open(path, ios::out);
    file << data;
    file.close();
}

void FAN::readdate()
{
    this->maxpower = readfdate(fan_maxpower_path);
    this->minworkpwm = readfdate(fan_minworkpwm_path);
    this->maxworkpwm = readfdate(fan_maxworkpwm_path);
    this->mod = readfdate(fan_mod_path);
}
void FAN::setpwm()
{
    writefdate(fan_curworkpwm_path, this->curworkpwm);
}

int FAN::power2pwm(float power)
{
    int pwm = (this->maxworkpwm - this->minworkpwm) * fmin(power, this->maxpower) + this->minworkpwm;
    return pwm;
}

void FAN::pwmcalc()
{

    float temp = readfdate(cpu_temp_path) / 1000.;
    float exptemp = readfdate(exptemp_path);
    float walltemp = readfdate(walltemp_path);
    int pwm = readfdate(fan_curworkpwm_path);

    if ((exptemp > walltemp) || (exptemp == walltemp))
    {
        writefdate(exptemp_path, walltemp - 1);
    }

    switch (this->mod)
    {
    case 0:
    {
        if (temp > walltemp)
        {
            if (pwm = 0)
            {
                writefdate(pwm_dutycycle_path, readfdate(fan_pwmperiod_path));
                usleep(100 * 1000); // 100ms
            }
            pwm = this->power2pwm(this->maxpower);
        }
        else if (temp < exptemp)
        {
            pwm = 0;
        }
    }
    break;

    case 1:
    {
        if (temp > exptemp + (walltemp - exptemp) * 3 / 5)
        {
            if (pwm = 0)
            {
                writefdate(pwm_dutycycle_path, readfdate(fan_pwmperiod_path));
                usleep(100 * 1000); // 100ms
            }
            pwm = this->power2pwm((temp - exptemp) / (walltemp - exptemp));
        }
        else if (temp < exptemp)
        {
            pwm = 0;
        }
    }
    break;

    default:
        break;
    }

    this->curworkpwm = pwm;
}

void PWMDEV::init()
{
    this->pwmperiod = readfdate(fan_pwmperiod_path);
    this->pwmdutycycle = readfdate(fan_curworkpwm_path);
    this->pwmexport = 0;
    this->pwmenable = 1;
    writefdate(pwm_dutycycle_path, this->pwmdutycycle);
    writefdate(pwm_period_path, this->pwmperiod);
    writefdate(pwm_enable_path, this->pwmenable);
    writefdate(pwm_export_path, this->pwmexport);
    ofstream file;
    file.open(pwm_polarity_path, ios::out);
    file << "normal";
    file.close();
}

void PWMDEV::setpwmdev()
{
    writefdate(pwm_dutycycle_path, readfdate(fan_curworkpwm_path));
}

int main()
{
    PWMDEV pwmdev;
    FAN fan;

    while (readfdate(pwm_enable_path)!=1)
    {
        pwmdev.init();
        pwmdev.setpwmdev();
        usleep(20 * 1000);
    }

    while (1)
    {
        fan.readdate();
        fan.pwmcalc();
        fan.setpwm();
        pwmdev.setpwmdev();
        usleep(20 * 1000); // 50ms
        // cout << "running" << endl;
        // cout << readfdate(exptemp_path) << endl;
        // cout << readfdate(cpu_temp_path) << endl;
    }
}
