from threading import Timer
from pathlib import Path
from time import sleep
import turtle

maxfanspeed=10000
deadfanspeed=2800
minfanspeed=2800

cpu0_temp_path = "/sys/class/thermal/thermal_zone0/temp"
fan_pwm_reg_path = "/sys/class/pwm/pwmchip1/export"
fan_pwm_peri_set_path = "/sys/class/pwm/pwmchip1/pwm0/period"
fan_pwm_duty_cycle_set_path = "/sys/class/pwm/pwmchip1/pwm0/duty_cycle"
fan_pwm_switch_set_path = "/sys/class/pwm/pwmchip1/pwm0/enable"
fan_pwm_mod_set_path = "/sys/class/pwm/pwmchip1/pwm0/polarity"

# set path by certain value


def set_value2path(path, value):
    file = open(path, 'w')
    file.write(str(value))

# read from path


def read_valuefrompath(path):
    file = open(path, 'r')
    return file.read()

# read cpu temeprature


def read_cputemp():
    return (float(read_valuefrompath(cpu0_temp_path))/1000.0)

# set pwm period


class FAN:
    def __init__(self, maxspeed_value=maxfanspeed,
                 deadspeed_value=deadfanspeed,
                 minspeed_value=minfanspeed):
        self.maxvalue = maxspeed_value
        self.devalue = deadspeed_value
        self.minvalue = minspeed_value

    def percent2value(self, power_percent):
        return int(power_percent*(self.maxvalue-self.devalue)+self.devalue)

    def set_power_percent(self, power_percent):
        set_value2path(fan_pwm_duty_cycle_set_path, self.percent2value(power_percent))

    def on():
        set_value2path(fan_pwm_switch_set_path, '1')

    def off():
        set_value2path(fan_pwm_switch_set_path, '0')

def fan_foliner_ctrl(expecttemp, tempwall):
    cpu_temp = read_cputemp()
    if (expecttemp > tempwall):
        expecttemp = tempwall-1
    if (cpu_temp < expecttemp):
        fan.set_power_percent(0)
    else:
        fan.set_power_percent((cpu_temp-expecttemp)/(tempwall-expecttemp))



fan = FAN()

# check if registered pwm device

set_value2path(fan_pwm_reg_path, '0')
set_value2path(fan_pwm_peri_set_path, fan.maxvalue)  # set pwm period
set_value2path(fan_pwm_duty_cycle_set_path, fan.devalue)  # set to initial pwm
set_value2path(fan_pwm_switch_set_path, '1')  # enable pwm
set_value2path(fan_pwm_mod_set_path, 'normal')  # set pwm normal mod

while True:
    fan_foliner_ctrl(40.0,50.0)
    sleep(2)
