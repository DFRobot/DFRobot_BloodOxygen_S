/*!
 * @file DFRobot_BloodOxygen_S.h
 * @brief This is the header of the library for the sensor that can detect human oxygen saturation and heart rate.
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author      PengKaixing(kaixing.peng@dfrobot.com)
 * @version  V1.1.0
 * @date  2026-11-05
 * @url https://github.com/DFRobot/DFRobot_BloodOxygen_S
 */
#ifndef __DFRobot_BloodOxygen_S_H__
#define __DFRobot_BloodOxygen_S_H__
#include "Arduino.h"
#include <Wire.h>
#include "DFRobot_RTU.h"

#if defined(ARDUINO_AVR_UNO) || defined(ESP8266)
#include "SoftwareSerial.h"
#else
#include "HardwareSerial.h"
#endif 

// Open this macro to see the program running in detail
#define ENABLE_DBG

#ifdef ENABLE_DBG
#define DBG(...)                 \
  {                              \
    Serial.print("[");           \
    Serial.print(__FUNCTION__);  \
    Serial.print("(): ");        \
    Serial.print(__LINE__);      \
    Serial.print(" ] ");         \
    Serial.println(__VA_ARGS__); \
  } 
#else
#define DBG(...)
#endif

#define MAX_SIZE 40
#define DEVICE_ADDRESS 0x20
#define BROADCAST_ADDRESS 0x00
#define RTU_READ_REG_CMD 0x03
#define RTU_WRITE_REG_CMD 0x06
#define RTU_WRITE_MULTIPLE_REG_CMD 0x10
#define SERIAL_DATA_BUF_MAX_SIZE 20
#define DEV_ADDRESS 0x0020

class DFRobot_BloodOxygen_S
{
  public:
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
     * @enum eSignalStatus
     * @brief Measurement status carried by getSignalInfo
     */
    typedef enum
    {
      eStatusStopped = 0,   ///< Measurement is stopped
      eStatusNoFinger = 1,  ///< No finger is detected
      eStatusAcquiring = 2, ///< Collecting data, no result yet
      eStatusValid = 3,     ///< Heart rate and SpO2 are valid
      eStatusPoor = 4       ///< Signal quality is too low
    } eSignalStatus;

    /**
     * @struct sSignalInfo
     * @brief Status and signal quality from hardware above V2.0
     * @n Hardware V2.0 is not supported. The fields are valid only after getSignalInfo returns true.
     * @n perfusion is the perfusion index in units of 0.01%. A value of 100 means 1.00%.
     * @n SQI is the signal quality, 0 to 100.
     * @n ledLevel is the infrared LED drive level.
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

    /**
     * @fn getVersion
     * @brief Read the firmware version register
     * @n This register exists on the earlier products as well as later ones.
     * @n 0x0100 is the earlier firmware. 0x0200 is hardware V2.0.
     * @return Version word. 0 means the register could not be read.
     */
    uint16_t getVersion(void);

    /**
     * @fn getSignalInfo
     * @brief Read measurement status, sequence, PI, signal quality and infrared LED level
     * @n This API can be used on hardware versions above V2.0.
     * @n Hardware V2.0 is not supported, and neither are earlier products.
     * @n Check the return value before using _sSignalInfo. A status of 0 is a real stopped state only when this function returns true.
     * @return true when the connected hardware is above V2.0 and the data has been stored in _sSignalInfo
     * @return false on hardware V2.0, earlier products, or a failed read. _sSignalInfo is cleared.
     */
    bool getSignalInfo(void);
    sHeartbeatSPO2 _sHeartbeatSPO2;
    sSignalInfo _sSignalInfo;
    ebautrate _bautrate;

  protected:
    /**
     * @fn writeReg
     * @brief Write data to the specified register of the sensor
     * @param reg_addr Register address to be written
     * @param data_buf Data to be written to register
     * @param len Length of data to be written
     */
    virtual void writeReg(uint16_t reg_addr, uint8_t *data_buf, uint8_t len) = 0;

    /**
     * @fn readReg
     * @brief Get the data with specified length from the specified sensor
     * @param reg_addr Register address to be read
     * @param data_buf The position storing the register data to be read
     * @param len Length of the data to be read
     */
    virtual int16_t readReg(uint16_t reg_addr, uint8_t *data_buf, uint8_t len) = 0;
};

class DFRobot_BloodOxygen_S_I2C : public DFRobot_BloodOxygen_S
{
  public:
    DFRobot_BloodOxygen_S_I2C(TwoWire *pWire = &Wire, uint8_t addr = 0x57);
    ~DFRobot_BloodOxygen_S_I2C(){};
    bool begin();
  protected:
    void writeReg(uint16_t reg_addr, uint8_t *data_buf, uint8_t len);
    int16_t readReg(uint16_t reg_addr, uint8_t *data_buf, uint8_t len);
  private:
    TwoWire *_pWire;
    uint8_t _I2C_addr;
};

//communication
#if defined(ARDUINO_AVR_UNO) || defined(ESP8266)
class DFRobot_BloodOxygen_S_SoftWareUart : public DFRobot_BloodOxygen_S
{
  public:
    DFRobot_BloodOxygen_S_SoftWareUart(SoftwareSerial *psoftUart, uint32_t baudRate);
    ~DFRobot_BloodOxygen_S_SoftWareUart(){};
    bool begin();
  protected:
    void writeReg(uint16_t reg_addr, uint8_t *data_buf, uint8_t len);
    int16_t readReg(uint16_t reg_addr, uint8_t *data_buf, uint8_t len);
private:
    SoftwareSerial *_psoftUart;
    uint32_t _baudRate;
    DFRobot_RTU _modbus;
};
#else
class DFRobot_BloodOxygen_S_HardWareUart : public DFRobot_BloodOxygen_S
{
  public:
    DFRobot_BloodOxygen_S_HardWareUart(HardwareSerial *phardUart, uint32_t baudRate);
    ~DFRobot_BloodOxygen_S_HardWareUart(){};
    bool begin();
  protected:
    void writeReg(uint16_t reg_addr, uint8_t *data_buf, uint8_t len);
    int16_t readReg(uint16_t reg_addr, uint8_t *data_buf, uint8_t len);
  private:
    HardwareSerial *_pharduart;
    uint32_t _baudRate;
    DFRobot_RTU _modbus;
};
#endif
#endif
