# 文件解释
1. exptemp 停转温度

2. walltemp 起转温度

3. fan_minpwm 风扇起转pwm值，需要自己测一下

4. fan_maxpwm 风扇最大转速pwm值，需要自己测一下

5. fan_maxpower 最大功率百分比 0--1

6. pwm_period pwm周期默认10000

7. fan_pwm 用户（脚本）写入的占空周期

8. fan_mod 风扇模式 0--手动写入的占空周期  1--自动开关  2--自动分段线性＋滞回调速

# 测试风扇pwm值

1. 将mod文件写2

2. 向fan_pwm写入0--10000的值，观察风扇转速，测得风扇起转pwm值写入fan_minpwm，再测得风扇最大转速pwm值写入fan_maxpwm

# 测量出合理的pwm值，给到合理的停转温度和起转温度可以让12k转的小风扇在超频至在1.4v时也能近乎安静地待机并且能够在瞬间高负载及时压住温度
