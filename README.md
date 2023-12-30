# home-security-alarm-system
 The Design of home security alarm system based on STM32

居室安全报警系统，基于STM32F103

---

此系统同时配套有微信小程序，STM32与微信小程序的交互通过中国移动OneNET云平台完成（暂未上传微信小程序代码）。

此系统实现了居室内火情监测、非法入侵监测，并在监测到异常时进行声光报警、远程报警；同时，此系统驱动4G模组与OneNET云平台交互，且用户可通过微信小程序与OneNET云平台交互，进而实现用户与此系统的交互。

- 火情监测的实现：火焰传感器 + 可燃气及烟雾检测传感器

- 非法入侵监测的实现：人脸识别模块 + 雷达人体感应模块

- 声光报警的实现：LED灯 + 蜂鸣器，模拟实现声光报警

- 远程报警的实现：通过4G模组，实现短信报警、电话报警

- STM32与云端交互的实现：通过4G模组联网，使用MQTT协议连接中国移动OneNET云平台

---

【注】

①OneNET-open-platform-RTThread文件夹下存放STM32程序，且系统使用了RT-Thread，其中云平台为OneNET物联网开放平台；实现了STM32、OneNET云平台、微信小程序之间的两两双向交互。

②OneNET-Studio文件夹下存放STM32程序，其中云平台为OneNET Studio；实现了STM32、OneNET云平台、微信小程序之间的两两双向交互。

③OneNET-Studio-RTThread文件夹下存放STM32程序，且系统使用了RT-Thread，其中云平台为OneNET Studio；实现了STM32、OneNET云平台、微信小程序之间的两两双向交互。

