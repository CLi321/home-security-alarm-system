# home-security-alarm-system
 The Design of home security alarm system based on STM32

居室安全报警系统，基于STM32F103

此系统同时配套有微信小程序，STM32与微信小程序的交互通过中国移动OneNET云平台完成。

此系统实现了居室内火情监测、非法入侵监测，并在监测到异常时进行声光报警、远程报警；同时，此系统驱动4G模组与OneNET云平台交互，且用户可通过微信小程序与OneNET云平台交互，进而实现用户与此系统的交互。

OneNET-open-platform-RTThread文件夹下存放STM32程序，且系统使用了RT-Thread，其中云平台为OneNET物联网开放平台。

OneNET-Studio文件夹下存放STM32程序，其中云平台为OneNET Studio。

OneNET-Studio-RTThread文件夹下存放STM32程序，且系统使用了RT-Thread，其中云平台为OneNET Studio
