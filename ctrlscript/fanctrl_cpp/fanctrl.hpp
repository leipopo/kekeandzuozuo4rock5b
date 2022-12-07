class FAN
{
public:
    float curworkpwm;
    float maxpower;
    float minworkpwm;
    float maxworkpwm;
    int mod;

    int power2pwm(float power);
    void readdate();
    void pwmcalc();
    void setpwm();
};

class PWMDEV
{
public:
    int pwmperiod;
    int pwmdutycycle;
    char pwmexport;
    char pwmenable;
    void init();
    void setpwmdev();
};

#define pwm_dutycycle_path "/sys/class/pwm/pwmchip1/pwm0/duty_cycle"
#define pwm_period_path "/sys/class/pwm/pwmchip1/pwm0/period"
#define pwm_enable_path "/sys/class/pwm/pwmchip1/pwm0/enable"
#define pwm_polarity_path "/sys/class/pwm/pwmchip1/pwm0/polarity"
#define pwm_export_path "/sys/class/pwm/pwmchip1/export"

#define cpu_temp_path "/sys/class/thermal/thermal_zone0/temp"

#define fan_mod_path "../fan_mod"
#define fan_maxpower_path "../fan_maxpower"
#define fan_minworkpwm_path "../fan_minpwm"
#define fan_maxworkpwm_path "../fan_maxpwm"
#define fan_curworkpwm_path "../fan_pwm"
#define fan_pwmperiod_path "../pwm_period"
#define exptemp_path "../exptemp"
#define walltemp_path "../walltemp"
