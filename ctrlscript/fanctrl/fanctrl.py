from threading import Timer
from pathlib import Path
from time import sleep

cpu0_temp_path = "/sys/class/thermal/thermal_zone0/temp"
fan_pwm_reg_path = "/sys/class/pwm/pwmchip1/export"
fan_pwm_peri_set_path = "/sys/class/pwm/pwmchip1/pwm0/period"
fan_pwm_duty_cycle_set_path = "/sys/class/pwm/pwmchip1/pwm0/duty_cycle"
fan_pwm_switch_set_path = "/sys/class/pwm/pwmchip1/pwm0/enable"
fan_pwm_mod_set_path = "/sys/class/pwm/pwmchip1/pwm0/polarity"

userset_fan_pwm_mod_path = "./mod"
userset_fan_pwm_cycle = "./pwm_duty_cycle"
userset_fan_pwm_period = "./pwm_period"
userset_fan_maxpower = "./fan_max_power"
userset_fan_dead_duty_cycle = "./fan_dead_duty_cycle"
userset_expecttemp = "./exp_temp"
userset_walltemp = "./wall_temp"


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
    def __init__(self):
        self.cpu_temp = float(read_cputemp())
        self.pwmperiod = float(read_valuefrompath(userset_fan_pwm_period))
        self.maxpower = float(read_valuefrompath(userset_fan_maxpower))
        self.deaddutycycle = float(
            read_valuefrompath(userset_fan_dead_duty_cycle))
        self.exptemp = float(read_valuefrompath(userset_expecttemp))
        self.walltemp = float(read_valuefrompath(userset_walltemp))
        self.mod = read_valuefrompath(userset_fan_pwm_mod_path)

    def percent2value(self, power):
        return int(power*(self.pwmperiod-self.deaddutycycle)+self.deaddutycycle)

    def set_power_percent(self, power):
        set_value2path(fan_pwm_duty_cycle_set_path,
                       self.percent2value(power))

    def fan_switch_ctrl(self,):

        if (self.exptemp > self.walltemp):
            self.exptemp = self.walltemp-1
            set_value2path(userset_expecttemp, self.exptemp)

        if (self.cpu_temp > self.walltemp):
            # print(fan.cpu_temp)
            self.set_power_percent(1)
            sleep(0.02)
            self.set_power_percent(self.maxpower)

        elif (self.cpu_temp < self.exptemp):
            # print(fan.walltemp)
            self.set_power_percent(0)


# check if registered pwm device
set_value2path(fan_pwm_reg_path, '0')
set_value2path(fan_pwm_peri_set_path, read_valuefrompath(
    userset_fan_pwm_period))  # set pwm period
set_value2path(fan_pwm_duty_cycle_set_path, read_valuefrompath(
    userset_fan_dead_duty_cycle))  # set to initial pwm
set_value2path(fan_pwm_switch_set_path, '1')  # enable pwm
set_value2path(fan_pwm_mod_set_path, 'normal')  # set pwm normal mod

fan = FAN()


while True:
    fan.__init__()
    if (fan.mod == 0):
        print(fan.cpu_temp)
        set_value2path(read_valuefrompath(userset_fan_pwm_cycle))
    else:
        fan.fan_switch_ctrl()

    sleep(2)
