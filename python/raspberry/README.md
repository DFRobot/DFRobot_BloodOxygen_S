# DFRobot_BloodOxygen_S

- [中文版](./README_CN.md)

The DFRobot MAX30102 heart rate and oximeter sensor integrates the Maxim MAX30102 chip and an MCU with heart rate and oximetry algorithm, which can help output heart rate and oximeter values directly. The MAX30102 uses PPG(PhotoPlethysmoGraphy) to measure data, which will be converted into heart rate and oximetry values when processed by the MCU, then output through I2C or UART, making the sensor easy to use and greatly reducing resource occupation of main controller. Meanwhile, the corresponding upper computer is provided to allow you to conveniently read data by a PC.

![正反面svg效果图](../../resources/images/SEN0344.png)

## Product Link（[https://www.dfrobot.com/product-2164.html](https://www.dfrobot.com/product-2164.html)）

  SKU：SEN0344

## Table of Contents

* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)

## Summary

This library provides routines to get the current heart rate and oxygen saturation of users.

## Installation

Download the library to Raspberry Pi before use, then open the routine folder. Input python gain_heartbeat_SPO2.py into the command line to execute a routine gain_heartbeat_SPO2.py.

## Methods

```python
   '''!
     @brief   Begin function, check sensor connection status
     @return  Return init status
     @retval True Init succeeded
     @retval False Init failed
   '''
   def begin(self):

   '''!
     @brief   Sensor starts to collect data
   '''
   def sensor_start_collect(self):

   '''!
     @brief   Sensor ended collecting data
   '''
   def sensor_end_collect(self):

   '''!
     @brief   Change serial baud rate
     @param bautrate
     @n     BAUT_RATE_1200 
     @n     BAUT_RATE_2400
     @n     BAUT_RATE_9600
     @n     BAUT_RATE_19200
     @n     BAUT_RATE_38400
     @n     BAUT_RATE_57600
     @n     BAUT_RATE_115200
   '''
   def set_bautrate(self,bautrate):

   '''!
     @brief Get heart rate and oxygen saturation and store them into the struct  sHeartbeatSPO2
   '''
   def get_heartbeat_SPO2(self):

   '''!
     @brief   Get the sensor board temp
     @return  Return board temp
   '''
   def get_temperature_c(self):

   '''!
     @brief   Get serial baud rate of the sensor
     @return  Return serial baud rate of the sensor
   '''
   def get_bautrate(self):

```
## Compatibility

| MCU         | Work Well | Work Wrong | Untested | Remarks |
| ------------ | :--: | :----: | :----: | :--: |
| RaspberryPi2 |      |        |   √    |      |
| RaspberryPi3 |      |        |   √    |      |
| RaspberryPi4 |  √   |        |        |      |

* Python version

| Python  | Work Well | Work Wrong | Untested | Remarks |
| ------- | :--: | :----: | :----: | ---- |
| Python2 |  √   |        |        |      |
| Python3 |  √   |        |        |      |

## History

- date 13-12-2021- version V1.0.0

## Credits

Written by Pengkaixing(kaixing.peng@dfrobot.com), 2021. (Welcome to our [website](https://www.dfrobot.com/))
