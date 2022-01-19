# DFRobot_BloodOxygen_S

- [中文版](./README_CN.md)

The DFRobot MAX30102 heart rate and oximeter sensor integrates the Maxim MAX30102 chip and an MCU with heart rate and oximetry algorithm, which can help output heart rate and oximeter values directly. The MAX30102 uses PPG(PhotoPlethysmoGraphy) to measure data, which will be converted into heart rate and oximetry values when processed by the MCU, then output through I2C or UART, making the sensor easy to use and greatly reducing resource occupation of main controller. Meanwhile, the corresponding upper computer is provided to allow you to conveniently read data by a PC.

![正反面svg效果图](./resources/images/SEN0344.png)

## Product Link（[https://www.dfrobot.com/product-2164.html](https://www.dfrobot.com/product-2164.html)）

  SKU：SEN0344

## Table of Contents

* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)

## Summary
This library provides sample codes to get the current heart rate and oxygen saturation of users.

## Installation

To use this library, first download the library file, paste it into the \Arduino\libraries directory, then open the examples folder and run the demo in the folder.

## Methods
```C++
   /**
    * @struct sHeartbeatSPO2
    * @brief The struct for storing heart rate and oxygen saturation
    */
   typedef struct
   {
     int SPO2;
     int Heartbeat;
   } sHeartbeatSPO2; 
   /**
    * @enum ebautrate
    * @brief Baud rate enum
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
    * @brief Get heart rate and oxygen saturation and store them into the struct sHeartbeatSPO2
    */
   void getHeartbeatSPO2(void); 
   /**
    * @fn getTemperature_C
    * @brief Get the sensor board temp
    * @return The current onboard temp (unit: ℃)
    */
   float getTemperature_C(void); 
   /**
    * @fn setBautrate
    * @brief Set serial baud rate of the sensor
    * @param bautrate baud rate
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
    * @brief Get serial port baud rate
    * @return Serial port baud rate
    */
   uint32_t getBautrate(void); 
   /**
    * @fn sensorStartCollect
    * @brief Data collecting starts
    */
   void sensorStartCollect(void); 
   /**
    * @fn sensorEndCollect
    * @brief Data collecting stops
    */
   void sensorEndCollect(void);

```
## Compatibility

MCU                | Work Well | Work Wrong | Untested  | Remarks
------------------ | :----------: | :----------: | :---------: | -----
Arduino Uno  |      √       |             |            | 
Leonardo  |      √       |             |            | 
Meag2560 |      √       |             |            | 
M0 |      √       |             |            | 
ESP32 |      √       |             |            | 
ESP8266 |      √       |             |            | 

## History

- 2019/12/13 - 1.0.0 version

## Credits

Written by Pengkaixing(kaixing.peng@dfrobot.com), 2021. (Welcome to our [website](https://www.dfrobot.com/))
