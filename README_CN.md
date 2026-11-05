# DFRobot_BloodOxygen_S

- [英文版](./README.md)

DFRobot MAX30102 心率血氧传感器模块搭载了美信(maxim)的MAX30102心率血氧芯片和一颗集成心率血氧算法的微控制器，可以直接输出心率血氧数值。MAX30102采用PPG光电容积脉搏波描记法(PhotoPlethysmoGraphy)测量数据，微控制器对这些数据进行处理运算过后得到心率血氧数值，再通过I2C或UART接口输出，大大降低了传感器的使用难度和对主控的资源占用。同时该传感器还配备有对应的上位机，可通过电脑直接读取数据。

![正反面svg效果图](./resources/images/SEN0344.png)

## 产品链接（[https://www.dfrobot.com.cn/goods-3392.html](https://www.dfrobot.com.cn/goods-3392.html)）

  SKU：SEN0344

## 目录

* [概述](#概述)
* [库安装](#库安装)
* [方法](#方法)
* [兼容性](#兼容性y)
* [历史](#历史)
* [创作者](#创作者)

## 概述

这个库提供了一个获取此时用户心率和血氧饱和度的例程。examples/gainHeartbeatSPO2 可在早期产品上直接使用。examples/getSignalInfo 演示新增接口，适用于高于硬件 V2.0 的产品，不包含 V2.0。

## 库安装

使用此库前，请首先下载库文件，将其粘贴到\Arduino\libraries目录中，然后打开examples文件夹并在该文件夹中运行演示。

## Methods
```C++
   /**
    * @struct sHeartbeatSPO2
    * @brief 存储心率和血氧饱和度的结构体
    */
   typedef struct
   {
     int SPO2;
     int Heartbeat;
   } sHeartbeatSPO2;
   /**
    * @struct sSignalInfo
    * @brief 高于硬件 V2.0 的产品提供的状态和信号质量
    * @n 不包含硬件 V2.0。只有 getSignalInfo 返回 true 时，这些字段才有效。
    * @n perfusion 是灌注指数，单位是 0.01%。数值 100 表示 1.00%。
    * @n SQI 是信号质量，范围 0 到 100。
    * @n ledLevel 是红外灯的驱动档位。
    */
   typedef struct
   {
     uint8_t status;
     uint8_t sequence;
     uint16_t perfusion;
     uint8_t SQI;
     uint8_t ledLevel;
   } sSignalInfo;
   /**
    * @enum ebautrate
    * @brief 波特率枚举
    */
   typedef enum
   {
     BAUT_RATE_1200 = 0,
     BAUT_RATE_2400 = 1,
     BAUT_RATE_9600 = 3,
     BAUT_RATE_19200 = 5,
     BAUT_RATE_38400 = 6,
     BAUT_RATE_57600 = 7,
     BAUT_RATE_115200 = 8
   } ebautrate; 
   DFRobot_BloodOxygen_S(void){};
   ~DFRobot_BloodOxygen_S(void){}; 
   /**
    * @fn getHeartbeatSPO2
    * @brief 获取心率和血氧饱和度并存入到结构体sHeartbeatSPO2中
    */
   void getHeartbeatSPO2(void); 
   /**
    * @fn getTemperature_C
    * @brief 获取传感器板子的温度
    * @return 当前板载温度值（单位：摄氏度）
    */
   float getTemperature_C(void); 
   /**
    * @fn setBautrate
    * @brief 设置传感器的串口波特率
    * @param bautrate 波特率
    * @n  BAUT_RATE_1200 
    * @n  BAUT_RATE_2400 
    * @n  BAUT_RATE_9600 
    * @n  BAUT_RATE_19200 
    * @n  BAUT_RATE_38400 
    * @n  BAUT_RATE_57600 
    * @n  BAUT_RATE_115200 
    */
   void setBautrate(ebautrate bautrate); 
   /**
    * @fn getBautrate
    * @brief 获取串口波特率
    * @return 串口波特率
    */
   uint32_t getBautrate(void); 
   /**
    * @fn sensorStartCollect
    * @brief 传感器开始采集数据
    */
   void sensorStartCollect(void); 
   /**
    * @fn sensorEndCollect
    * @brief 传感器结束采集数据
    */
   void sensorEndCollect(void);
   /**
    * @fn getVersion
    * @brief 读取固件版本寄存器
    * @n 早期产品和后续产品都有这个寄存器。
    * @n 0x0100 是早期固件，0x0200 是硬件 V2.0。
    * @return 版本字。读不到寄存器时返回 0。
    */
   uint16_t getVersion(void);
   /**
    * @fn getSignalInfo
    * @brief 读取测量状态、序号、PI、信号质量和红外灯亮度
    * @n 该接口适用于硬件版本高于 V2.0 的产品。
    * @n 不包含硬件 V2.0，早期产品也不能使用。
    * @n 使用 _sSignalInfo 前先看返回值。只有返回 true 时，状态 0 才表示测量已停止。
    * @return 硬件版本高于 V2.0 且数据已写入 _sSignalInfo 时返回 true
    * @return 硬件 V2.0、早期产品或读取失败时返回 false，并清空 _sSignalInfo。
    */
   bool getSignalInfo(void);

```
## 兼容性

MCU                | Work Well | Work Wrong | Untested  | Remarks
------------------ | :----------: | :----------: | :---------: | -----
Arduino Uno  |      √       |             |            | 
Leonardo  |      √       |             |            | 
Meag2560 |      √       |             |            | 
M0 |      √       |             |            | 
ESP32 |      √       |             |            | 
ESP8266 |      √       |             |            | 

## 历史

- 2019/12/13 - 1.0.0 版本
- 2026/11/05 - 1.1.0 版本。增加 getVersion 和 getSignalInfo，并增加 examples/getSignalInfo。getSignalInfo 适用于高于硬件 V2.0 的产品，不包含 V2.0。原有示例在早期产品上仍可直接使用。

## Credits
Written by Pengkaixing(kaixing.peng@dfrobot.com), 2021. (Welcome to our [website](https://www.dfrobot.com/))