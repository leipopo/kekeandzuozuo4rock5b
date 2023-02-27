#include <fstream>
#include <iostream>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

#define soc_temp_path "/sys/class/thermal/thermal_zone2/temp"
#define pwm_path "/sys/class/pwm"
#define soc_fan_configfile_path "../source/soc_fan_config"
#define ssd_fan_configfile_path "../source/ssd_fan_config"
#define soc 0
#define linermod 1
#define manualmod 2
#define switchmod 3
#define boosttime_max 200

class FAN
{
public:
    bool switcher;
    int boosttime;
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
    float lowpower_temp;
    float fanon_temp;

    int power2pwm(float power);
    void readconfigfile(const char *path);
    void pwmcalc();
    void setpwmdev();
    void boost();

    char pwm_chip[16]={'0',};
    char pwm_channel[16]={'0',};

    char objtemp_path[64]={'0',};
    char pwm_dutycycle_path[64]={'0',};
    char pwm_period_path[64]={'0',};
    char pwm_enable_path[64]={'0',};
    char pwm_polarity_path[64]={'0',};
    char pwm_export_path[64]={'0',};
};