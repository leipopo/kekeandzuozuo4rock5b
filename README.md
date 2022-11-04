# keke4rockpi5b //**个人完全验证**//

## 给RockPi 5B V1.4硬件版本用的壳壳

![rp1](/img/rp1.jpg "图")

![rp2](/img/rp2.jpg "图")

![rp3](/img/rp3.jpg "图")

![rp4](/img/rp4.jpg "图")

![rp5](/img/rp5.jpg "图")

![上图](/img/P1.jpg "上图")

![下图](/img/P2.jpg "下图")

![风道剖视图](/img/P4.jpg "风道剖视图")

## 物料购买注意事项

1. 散热鳍片25x25x11

2. 滚花螺母尺寸M3 H4 D4.5
滚花螺母尺寸M3 H4 D4.5
滚花螺母尺寸M3 H4 D4.5
重要的事情嗦三遍（用外径5的大了容易顶到附近的小电容）

3. 摄像头，屏幕，GPIO排针都需要线材引出

4. 无线网卡需要用粘上去的那种天线

5. 如果使用硅脂导热，需要购买3M的VHB双面胶贴在上盖压住散热片的位置，如上面最后一张图红色部分所示

6. 需要脚垫可淘宝搜索“橡胶脚垫 机箱仪器”，然后买内径3的如下图

![脚垫图](/img/jiaodian.jpg "脚垫图")

## FDM注意事项（实时更新，0.4线宽，0.2层高）

1. 下盖建议只打两层壁，不然搭桥会出现一点问题
2. 应该都不需要打支撑，有的话qq/issue见

## 关于风扇脚本

需要sudo权限，要自启的话需要自行操作一下。

拉取完整仓库：

```cd ~```

```git clone https://ghproxy.com/https://github.com/leipopo/keke4rockpi5b```

运行风扇脚本：

```cd $HOME/keke4rockpi5b/ctrlscript/fanctrl```

```sudo python3 fanctrl.py```
