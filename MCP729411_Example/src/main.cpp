#include <Arduino.h>
#include <Wire.h>
#include "MCP79411.h"
#define I2C1_SCL 5
#define I2C1_SDA 4

TwoWire I2C_1 = TwoWire(1);
MCP79411_RTC rtc;

void setup()
{
  I2C_1.begin(I2C1_SDA, I2C1_SCL, 50000);
  // MCP729411_Init(&I2C_1);
  Serial.begin(115200);
  Serial.println("\033cMCP79411 Reading Lab");
  rtc.init(&I2C_1);
  Serial.printf("Get on board OSC %d\r\n", rtc.getOnBoardOscillator());
}
unsigned long timeNow;
void loop()
{
  if (millis() - timeNow > 1000)
  {
    DateTime currentTime = rtc.getDateTime();
    Serial.printf("Day %d Date %02d/%02d/%d %02d:%02d:%02d 12?%d AM?%d\r\n",currentTime.day, currentTime.date, currentTime.month, currentTime.year, currentTime.hours, currentTime.minutes, currentTime.seconds, rtc.is12Hour(), rtc.isAM());
    // Serial.println(rtc.getHours());
    timeNow = millis();
  }
  
  while (Serial.available()){
    String serialVal = Serial.readStringUntil('\r');
    Serial.println(serialVal);
    if (serialVal.indexOf("Date") != -1){
      int date,month,year;
      sscanf(serialVal.c_str(),"Date = %d/%d/%d",&date,&month,&year);
      Serial.printf("Date Set command = %d/%d/%d\r\n",date,month,year);
      rtc.setDate(date,month,year);
    }
    if (serialVal.indexOf("Time") != -1){
      int second,minute,hour;
      sscanf(serialVal.c_str(),"Time = %d:%d:%d",&hour,&minute,&second);
      Serial.printf("Time Set command = %d:%d:%d\r\n",hour,minute,second);
      rtc.setTime(hour,minute,second);
    }
    if (serialVal.indexOf("Day") != -1){
      int day;
      sscanf(serialVal.c_str(),"Day = %d",&day);
      Serial.printf("Day Set command = %d\r\n",day);
      rtc.setDay(day);
    }
    if (serialVal.indexOf("Format") != -1){
      int format;
      sscanf(serialVal.c_str(),"Format = %d",&format);
      Serial.printf("Set time format to %s\r\n",format ? "12 Hour": "24 Hour");
      if (format){
        rtc.set12Hour();
      }
      else {
        rtc.set24Hour();
      }
    }
  }
}