# -*- coding: utf-8 -*
'''!
  @file DFRobot_BloodOxygen_S.py
  @brief This is the python library for the sensor that can detect human oxygen saturation and heart rate.
  @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  @license     The MIT License (MIT)
  @author      PengKaixing(kaixing.peng@dfrobot.com)
  @version  V1.0.0.0
  @date  2021-06-30
  @url https://github.com/DFRobot/DFRobot_BloodOxygen_S
'''

import serial
import time
import smbus
import os
import math
import RPi.GPIO as GPIO
import math
from DFRobot_RTU import *

I2C_MODE                  = 0x01
UART_MODE                 = 0x02
DEV_ADDRESS               = 0x0020
DEVICE_ADDRESS            = 0x20

class DFRobot_BloodOxygen_S(DFRobot_RTU):
  '''!
    @brief This is the base class of the heart rate and oximeter sensor.
  '''
  SPO2 = 0
  heartbeat = 0
  pSPO2 = 0
  pheartbeat = 0
  START_MODE = 2
  END_MODE = 3
  BAUT_RATE_1200 = 0 
  BAUT_RATE_2400 = 1
  BAUT_RATE_9600 = 3
  BAUT_RATE_19200 = 5
  BAUT_RATE_38400 = 6 
  BAUT_RATE_57600 = 7
  BAUT_RATE_115200 = 8   
    
  def __init__(self ,bus ,Baud):
    if bus != 0:
      self.i2cbus = smbus.SMBus(bus)
      self.__uart_i2c = I2C_MODE
    else:
      super(DFRobot_BloodOxygen_S, self).__init__(Baud, 8, 'N', 1)
      #self.ser = serial.Serial("/dev/ttyAMA0" ,baudrate=Baud,stopbits=1, timeout=0.5)
      self.__uart_i2c = UART_MODE
      if self.ser.isOpen == False:
        self.ser.open() 

  def begin(self):
    '''!
      @brief   Begin function, check sensor connection status
      @return  Return init status
      @retval True Init succeeded
      @retval False Init failed
    '''
    global DEV_ADDRESS
    rbuf = self.read_reg(0x04,2)
    if (rbuf[0] & 0xff << 8 | rbuf[1]) == DEV_ADDRESS:
      return True
    else:
      return False

  def sensor_start_collect(self):
    '''!
      @brief   Sensor starts to collect data
    '''
    wbuf = [0x00,0x01]
    self.write_reg(0x20, wbuf)
    
  def sensor_end_collect(self):
    '''!
      @brief   Sensor ended collecting data
    '''
    wbuf = [0x00,0x02]
    self.write_reg(0x20, wbuf)

  def set_bautrate(self,bautrate):
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
    w_buf = [0,bautrate]
    self.write_reg(0x1C, w_buf)

  def get_heartbeat_SPO2(self):
    '''!
      @brief Get heart rate and oxygen saturation and store them into the struct  sHeartbeatSPO2
    '''
    rbuf = self.read_reg(0x0C,8)
    SPO2Valid = rbuf[1]
    HeartbeatValid = rbuf[7]
    self.SPO2 = rbuf[0]
    if self.SPO2 == 0:
      self.SPO2 = -1
    self.heartbeat = int(rbuf[2]) << 24 | int(rbuf[3]) << 16 | int(rbuf[4]) << 8 | int(rbuf[5])
    if self.heartbeat == 0:
      self.heartbeat = -1

  def get_temperature_c(self):
    '''!
      @brief   Get the sensor board temp
      @return  Return board temp
    '''
    temp_buf = self.read_reg(0x14, 2)
    Temperature = temp_buf[0] * 1.0 + temp_buf[0] / 100.0
    return Temperature

  def get_bautrate(self):
    '''!
      @brief   Get serial baud rate of the sensor
      @return  Return serial baud rate of the sensor
    '''
    r_buf = self.read_reg(0x1C,2)
    baudrate_type = int(r_buf[0]) << 8 | int(r_buf[1])
    if(baudrate_type == 0):
      return 1200
    elif(baudrate_type == 1):
      return 2400 
    elif(baudrate_type == 3):
      return 9600 
    elif(baudrate_type == 5):
      return 19200 
    elif(baudrate_type == 6):
      return 38400 
    elif(baudrate_type == 7):
      return 57600 
    elif(baudrate_type == 8):
      return 115200 
    else:
      return 9600
        
class DFRobot_BloodOxygen_S_i2c(DFRobot_BloodOxygen_S):
  '''
    @brief An example of an i2c interface module
  '''
  def __init__(self ,bus ,addr):
    self.__addr = addr
    super(DFRobot_BloodOxygen_S_i2c, self).__init__(bus,0)    
    
  def write_reg(self, reg_addr, data_buf):
    '''
      @brief writes data to a register
      @param reg register address
      @param value written data
    '''
    while 1:
      try:
        self.i2cbus.write_i2c_block_data(self.__addr ,reg_addr ,data_buf)
        return
      except:
        print("please check connect!")
        time.sleep(1)

  def read_reg(self, reg_addr ,length):
    '''
      @brief read the data from the register
      @param reg register address
      @param value read data
    '''
    try:
      rslt = self.i2cbus.read_i2c_block_data(self.__addr ,reg_addr , length)
    except:
      rslt = -1
    return rslt    

'''
  @brief An example of an UART interface module
'''
class DFRobot_BloodOxygen_S_uart(DFRobot_BloodOxygen_S):
  SERIAL_DATA_BUF_MAX_SIZE = 20
  RTU_READ_REG_CMD = 0x03
  RTU_WRITE_REG_CMD = 0x06
  RTU_WRITE_MULTIPLE_REG_CMD = 0x10
  def __init__(self ,Baud):
    self.__Baud = Baud
    try:
      super(DFRobot_BloodOxygen_S, self).__init__(Baud, 8, 'N', 1)
    except:
      print ("plese get root!")  
  
  '''
    Write data to the sensor
  '''
  def write_reg(self, reg_addr, data_buf): 
    self.write_holding_register(0x20,int(math.ceil(reg_addr/2)),(data_buf[0]<<8)|data_buf[1]&0xffff)
   
  '''
    Read data from the sensor
  '''
  def read_reg(self, reg_addr ,length):
    return self.read_holding_registers(0x20,int(math.ceil(reg_addr/2)),int(math.ceil(length/2)))[1:]

       
