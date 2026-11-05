/*!
 * @file  getSignalInfo.ino
 * @n Demonstrates getVersion and getSignalInfo.
 * @n getSignalInfo can be used on hardware versions above V2.0. Hardware V2.0 is not supported.
 * @n On V2.0 and earlier products this sketch still runs. Heart rate and SpO2 are printed, and getSignalInfo reports that it is not supported.
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author      PengKaixing(kaixing.peng@dfrobot.com)
 * @version     V1.1.0
 * @date        2026-11-05
 * @url         https://github.com/DFRobot/DFRobot_BloodOxygen_S
 */
#include "DFRobot_BloodOxygen_S.h"

#define I2C_COMMUNICATION  //use I2C for communication, but use the serial port for communication if the line of codes were masked

#ifdef  I2C_COMMUNICATION
#define I2C_ADDRESS    0x57
  DFRobot_BloodOxygen_S_I2C MAX30102(&Wire ,I2C_ADDRESS);
#else
/* ---------------------------------------------------------------------------------------------------------------
 *    board   |             MCU                | Leonardo/Mega2560/M0 |    UNO    | ESP8266 | ESP32 |  microbit  |
 *     VCC    |            3.3V/5V             |        VCC           |    VCC    |   VCC   |  VCC  |     X      |
 *     GND    |              GND               |        GND           |    GND    |   GND   |  GND  |     X      |
 *     RX     |              TX                |     Serial1 TX1      |     5     |   5/D6  |  D2   |     X      |
 *     TX     |              RX                |     Serial1 RX1      |     4     |   4/D7  |  D3   |     X      |
 * ---------------------------------------------------------------------------------------------------------------*/
#if defined(ARDUINO_AVR_UNO) || defined(ESP8266)
SoftwareSerial mySerial(4, 5);
DFRobot_BloodOxygen_S_SoftWareUart MAX30102(&mySerial, 9600);
#else
DFRobot_BloodOxygen_S_HardWareUart MAX30102(&Serial1, 9600);
#endif
#endif

static const char *signalStatusText(uint8_t status)
{
  switch (status)
  {
    case DFRobot_BloodOxygen_S::eStatusStopped:
      return "stopped";
    case DFRobot_BloodOxygen_S::eStatusNoFinger:
      return "no finger";
    case DFRobot_BloodOxygen_S::eStatusAcquiring:
      return "acquiring";
    case DFRobot_BloodOxygen_S::eStatusValid:
      return "valid";
    case DFRobot_BloodOxygen_S::eStatusPoor:
      return "poor";
    default:
      return "unknown";
  }
}

void setup()
{
  uint16_t version;
  Serial.begin(115200);
  while (false == MAX30102.begin())
  {
    Serial.println("init fail!");
    delay(1000);
  }
  Serial.println("init success!");
  version = MAX30102.getVersion();
  Serial.print("firmware version: 0x");
  Serial.println(version, HEX);
  if (version <= 0x0200)
  {
    Serial.println("getSignalInfo needs hardware above V2.0. Hardware V2.0 is not supported.");
  }
  Serial.println("start measuring...");
  MAX30102.sensorStartCollect();
}

void loop()
{
  MAX30102.getHeartbeatSPO2();
  Serial.print("SPO2 is : ");
  Serial.print(MAX30102._sHeartbeatSPO2.SPO2);
  Serial.println("%");
  Serial.print("heart rate is : ");
  Serial.print(MAX30102._sHeartbeatSPO2.Heartbeat);
  Serial.println(" Times/min");

  if (MAX30102.getSignalInfo())
  {
    Serial.print("status : ");
    Serial.println(signalStatusText(MAX30102._sSignalInfo.status));
    Serial.print("sequence : ");
    Serial.println(MAX30102._sSignalInfo.sequence);
    Serial.print("PI : ");
    Serial.print(MAX30102._sSignalInfo.perfusion / 100);
    Serial.print(".");
    if ((MAX30102._sSignalInfo.perfusion % 100) < 10)
    {
      Serial.print("0");
    }
    Serial.print(MAX30102._sSignalInfo.perfusion % 100);
    Serial.println("%");
    Serial.print("SQI : ");
    Serial.println(MAX30102._sSignalInfo.SQI);
    Serial.print("IR LED level : ");
    Serial.println(MAX30102._sSignalInfo.ledLevel);
  }
  else
  {
    Serial.println("getSignalInfo is not supported on this hardware.");
  }
  Serial.println();
  delay(1000);
}
