#include "i2c_lm73.h"
#include "Wire.h"

LM73::LM73(TwoWire &I2C_1, byte LM73_ADDR)
    : I2C_1(I2C_1), LM73_ADDR(LM73_ADDR) {}

void LM73::lm73Init()
{
    I2C_1.beginTransmission(LM73_ADDR);
    I2C_1.write(4);
    I2C_1.endTransmission();
    I2C_1.requestFrom(LM73_ADDR, 1);
    int data;
    while (I2C_1.available())
    {
        data = I2C_1.read();
    }
    Serial.println((0b11 << 6), BIN);
    data |= (0b11 << 6);
    Serial.println(data, BIN);
    I2C_1.beginTransmission(LM73_ADDR);
    I2C_1.write(4);
    I2C_1.write(data);
    I2C_1.endTransmission();
}

int LM73::lm73GetStatus()
{
    I2C_1.beginTransmission(LM73_ADDR);
    I2C_1.write(4);
    I2C_1.endTransmission();
    I2C_1.requestFrom(LM73_ADDR, 1);
    int data;
    while (I2C_1.available())
    {
        data = I2C_1.read();
    }
    return data;
}

float LM73::lm73GetTemperature()
{
    uint8_t tempBuff[2];
    I2C_1.beginTransmission(LM73_ADDR);
    I2C_1.write(0);
    I2C_1.endTransmission();
    I2C_1.requestFrom(LM73_ADDR, 2);
    while (I2C_1.available())
    {
        I2C_1.readBytes(tempBuff, 2);
    }
    float tempHigh = (tempBuff[0] & 127) * 2.0;
    float tempLow = (tempBuff[1] >> 2) * 0.03125;
    return tempHigh + tempLow;
}

void LM73::showData()
{
    float temp = lm73GetTemperature();
    Serial.printf("Temperature : %f\r\n", temp);
    delay(1000);
}

void LM73::settingResolution(String data)
{
    int resolution;
    sscanf(data.c_str(), "Resolution : %d", &resolution);
    int resolutionbit = lm73GetStatus();
    Serial.print("before : ");
    Serial.println(resolutionbit, BIN);
    switch (resolution)
    {
    case 0:
        resolutionbit &= 159; // 10011111
        break;
    case 1:
        resolutionbit |= 32;  // 00100000
        resolutionbit &= 191; // 10111111
        break;
    case 10:
        resolutionbit |= 64;  // 01000000
        resolutionbit &= 223; // 11011111
        break;
    case 11:
        resolutionbit |= 96; // 01100000
        break;
    default:
        Serial.println("Invalid resolution");
        break;
    }
    Serial.print("after : ");
    Serial.println(resolutionbit, BIN);
    I2C_1.beginTransmission(LM73_ADDR);
    I2C_1.write(4);
    I2C_1.write(resolutionbit);
    I2C_1.endTransmission();
}

