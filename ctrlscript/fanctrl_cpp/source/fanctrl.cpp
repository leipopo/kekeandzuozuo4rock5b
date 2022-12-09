#include "fanctrl.hpp"
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <math.h>
using namespace std;

int readfdata(const char *path)
{
    ifstream file;
    file.open(path, ios::in);
    int data;
    file >> data;
    file.close();
    return data;
}
void writefdata(const char *path, int data)
{
    ofstream file;
    file.open(path, ios::out);
    file << data;
    file.close();
}

void FAN::readdata()
{
    this->maxpower = readfdata(fan_maxpower_path);
    this->minworkpwm = readfdata(fan_minworkpwm_path);
    this->maxworkpwm = readfdata(fan_maxworkpwm_path);
    this->mod = readfdata(fan_mod_path);
}
void FAN::setpwm()
{
    writefdata(fan_curworkpwm_path, this->curworkpwm);
}

int FAN::power2pwm(float power)
{
    int pwm = (this->maxworkpwm - this->minworkpwm) * fmin(power, this->maxpower) + this->minworkpwm;
    return pwm;
}

void FAN::pwmcalc()
{

    float temp = readfdata(cpu_temp_path) / 1000.f;
    float exptemp = readfdata(exptemp_path);
    float walltemp = readfdata(walltemp_path);
    int pwm = readfdata(fan_curworkpwm_path);

    if ((exptemp > walltemp) || (exptemp == walltemp))
    {
        writefdata(exptemp_path, walltemp - 1);
    }

    switch (this->mod)
    {
    case 0:
    {
        if (temp > walltemp)
        {
            if (pwm = 0)
            {
                writefdata(pwm_dutycycle_path, readfdata(fan_pwmperiod_path));
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
                writefdata(pwm_dutycycle_path, readfdata(fan_pwmperiod_path));
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
    this->pwmperiod = readfdata(fan_pwmperiod_path);
    this->pwmdutycycle = readfdata(fan_curworkpwm_path);
    this->pwmexport = 0;
    this->pwmenable = 1;
    writefdata(pwm_dutycycle_path, this->pwmdutycycle);
    writefdata(pwm_period_path, this->pwmperiod);
    writefdata(pwm_enable_path, this->pwmenable);
    writefdata(pwm_export_path, this->pwmexport);
    ofstream file;
    file.open(pwm_polarity_path, ios::out);
    file << "normal";
    file.close();
}

void PWMDEV::setpwmdev()
{
    writefdata(pwm_dutycycle_path, readfdata(fan_curworkpwm_path));
}

int main()
{
    PWMDEV pwmdev;
    FAN fan;

    while (readfdata(pwm_enable_path)!=1)
    {
        pwmdev.init();
        pwmdev.setpwmdev();
        usleep(20 * 1000);
    }

    while (1)
    {
        fan.readdata();
        fan.pwmcalc();
        fan.setpwm();
        pwmdev.setpwmdev();
        usleep(20 * 1000); // 20ms
        // cout << "running" << endl;
        // cout << readfdata(exptemp_path) << endl;
        // cout << readfdata(cpu_temp_path) << endl;
    }
}
