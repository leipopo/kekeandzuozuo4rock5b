#include <fstream>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

#define soc_temp_path "/sys/class/thermal/thermal_zone1/temp"
#define pwm_path "/sys/class/pwm"
#define soc_fan_configfile_path "../source/soc_fan_config"
#define ssd_fan_configfile_path "../source/ssd_fan_config"
#define soc 0
#define linermod 1
#define manualmod 2
#define switchmod 3


class FAN
{
public:
    int obj;
    int pwm_period;
    int fan_maxpwm;
    int fan_minpwm;
    int fan_mod;
    int fan_pwm;
    
    float obj_temp;
    float fan_maxpower;
    float exp_temp;
    float wall_temp;

    int power2pwm(float power);
    void init(const char *path);
    void readconfigfile(const char *path);
    void pwmcalc();
    void setpwmdev();
    void boost();

    const char *pwm_chip;
    const char *pwm_channel;
    
    const char *configfile_path;
    const char *objtemp_path;
    const char *pwm_dutycycle_path;
    const char *pwm_period_path;
    const char *pwm_enable_path;
    const char *pwm_polarity_path;
    const char *pwm_export_path;
};