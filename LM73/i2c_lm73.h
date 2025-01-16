#ifndef LM73_H
#define LM73_H

#include "Wire.h"
#include <Arduino.h>

class LM73 {
private:
    TwoWire& I2C_1; // Reference to TwoWire instance
    byte LM73_ADDR; // I2C address of LM73 sensor

public:
    // Constructor declaration
    LM73(TwoWire& I2C_1, byte LM73_ADDR);
    void lm73Init();         // Initialize LM73
    int lm73GetStatus();     // Get LM73 status
    float lm73GetTemperature(); // Get LM73 temperature
    void showData();
    void settingResolution(String data);
};

#endif
