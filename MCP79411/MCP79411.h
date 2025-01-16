#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <time.h>
#define MCP79411_ADDR 0x6F

#define MCP79411_SECONDS_REG    0x00
#define MCP79411_MINUTES_REG    0x01
#define MCP79411_HOURS_REG      0x02
#define MCP79411_DAY_REG        0x03
#define MCP79411_DATE_REG       0x04
#define MCP79411_MONTH_REG      0x05
#define MCP79411_YEAR_REG       0x06
#define MCP79411_CONTROL_REG    0x07
#define MCP79411_CALIBRATE_REG  0x08
#define MCP79411_UNLOCK_ID_REG  0x09

#define MCP79411_ALARM0_SECONDS_REG 0x0A
#define MCP79411_ALARM0_MINUTES_REG 0x0B
#define MCP79411_ALARM0_HOURS_REG   0x0C
#define MCP79411_ALARM0_DAY_REG     0x0D
#define MCP79411_ALARM0_DATE_REG    0x0E
#define MCP79411_ALARM0_MONTH_REG   0x0F
#define MCP79411_ALARM0_YEAR_REG    0x10

#define MCP79411_ALARM1_SECONDS_REG 0x11
#define MCP79411_ALARM1_MINUTES_REG 0x12
#define MCP79411_ALARM1_HOURS_REG   0x13
#define MCP79411_ALARM1_DAY_REG     0x14
#define MCP79411_ALARM1_DATE_REG    0x15
#define MCP79411_ALARM1_MONTH_REG   0x16
#define MCP79411_ALARM1_YEAR_REG    0x18

class DateTime
{
    public:
        int seconds;
        int minutes;
        int hours;
        int day;
        int date;
        int month;
        int year;
} ;

class MCP79411_RTC{
    public:
        void init(TwoWire *wireInstance);
        int getOnBoardOscillator();
        void setDateTime();
        void setTime(int hour,int minute,int second);
        void setDate(int date,int month, int year);
        void setDay(int day);
        void set24Hour();
        void set12Hour();
        DateTime getDateTime();
        int getSeconds();
        int getMinutes();
        int getHours();
        int isAM();
        int is12Hour();
        int getDay();
        int getDate();
        int getMonth();
        int getYear();
    private:
        int writeReg(uint8_t reg,uint8_t data);
        int writeRegs(uint8_t reg,uint8_t* data,int size);
        int readRegs(uint8_t reg,uint8_t* data,int size);
        int rtc_address;
        TwoWire *_wire;
        int _isAm = 0;
        int _is12 = 0;
        int _isLeap;
};


