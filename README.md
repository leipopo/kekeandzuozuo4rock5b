# kekeandzuozuo4rock5b 

## CAD源文件，链接 https://a360.co/3OjdNzo

## 给RockPi 5B V1.4硬件版本用的壳壳 （模型以CAD文件为准

![图](/img/P1.jpg "图")

![图](/img/P2.jpg "图")

![图](/img/P3.jpg "图")

![图](/img/P4.jpg "图")

![图](/img/P5.jpg "图")

![图](/img/P6.jpg "图")

![图](/img/P7.jpg "图")

![图](/img/P8.jpg "图")

买不起台灯不太好打光就举着拍吧

![rp1](/img/rp1.jpg "图")

![rp2](/img/rp2.jpg "图")

![rp3](/img/rp3.jpg "图")

![rp4](/img/rp4.jpg "图")

![rp5](/img/rp5.jpg "图")

![rp6](/img/rp6.jpg "图")

![rp7](/img/rp7.jpg "图")

![rp8](/img/rp8.jpg "图")

![rp9](/img/rp9.jpg "图")

![rp10](/img/rp10.jpg "图")

![rp11](/img/rp11.jpg "图")



## 物料购买注意事项

1. 散热鳍片25x25x11；

2. 滚花螺母尺寸M3 H4 D4.5
滚花螺母尺寸M3 H4 D4.5
滚花螺母尺寸M3 H4 D4.5
重要的事情嗦三遍（用外径5的大了容易顶到附近的小电容）；

3. 螺丝M3 长度8——25买齐 还需要一个六角螺母（散热片版本）；

4. 摄像头，屏幕，GPIO排针都需要线材引出；

5. 无线网卡需要用粘上去的那种天线；

6. 如果使用硅脂导热，需要购买3M的VHB双面胶贴在上盖压住散热片的位置，如上面最后一张图红色部分所示。屏幕的固定同样需要VHB，因为我担心这个小屏幕尺寸公差比较大所以游隙留的比较大；

7. WS2812灯条买100灯珠一米的，贴在脚撑内侧空白处，两侧的串联，应该可以一边五个，CAD图里用的60灯珠的示意一下；

8. 需要脚垫可淘宝搜索“橡胶脚垫 机箱仪器”，然后买内径3mm外径11mm及以下的如下图

![脚垫图](/img/jiaodian.jpg "脚垫图")

## FDM注意事项（实时更新，0.4线宽，0.2层高）

模型基础板为散热片版本

使用树莓派下压式散热需打印TOWER内的扣具和适配上盖，不打印基础上盖

使用RADXA官方散热风扇需打印上盖FE，不打印基础上盖

使用加高下框需打印下框加高，不打印基础下框

树莓派上盖有有外置天线/无外置天线两个版本，有天线上盖可以上小耳朵或者盖板代替外置天线，小耳朵里面可以塞一个内置天线

带maskrom按钮的上框只经过了1.41版本搭配散热片外壳的装配测试，故暂时不放出stl，有条件的朋友可以自行测试修改按钮CAD文件的外形及尺寸来配合自己使用的版本的外壳

带maskrom按钮的上框也可以放上底座

## 关于风扇脚本

需要sudo权限，要自启的话需要自行操作一下。

拉取完整仓库：

```cd ~```

```git clone https://ghproxy.com/https://github.com/leipopo/kekeandzuozuo4rockpi5b```

运行风扇脚本：

```cd $HOME/kekeandzuozuo4rockpi5b/ctrlscript/fanctrl/build```

```sudo ./fanctrl```

### 调整风扇初始化文件参数

不建议修改此文件排版

#### 参数命名

1. reset 调整参数后需要调整此参数为1

2. obj 温控对象，暂时只有soc

3. exp_temp 期望温度

4. wall_temp 上限温度
 
5. fan_maxpwm 风扇极限转速时pwm占空值

6. fan_minpwm 风扇起转时的pwm占空值

7. fan_maxpower 风扇最大功率 0--1.f

8. fan_mod 1.线性滞回控制，2.手动控制 3.开关控制
 
9. fan_pwm="0" 手动控制时给的pwm占空值

10. pwm_period="10000" pwm周期值

11. pwm_chip="/pwmchip15" pwm设备号

12. pwm_channel="/pwm0" pwm设备下通道号
