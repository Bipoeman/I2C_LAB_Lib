#include "i2c_lm73.h"
#include "Wire.h"

#define I2C1_SCL 5
#define I2C1_SDA 4
#define LM73_ADDR 0x4D

TwoWire I2C_1 = TwoWire(1);
LM73 lm73(I2C_1,LM73_ADDR);

void setup() {
  I2C_1.begin(I2C1_SDA, I2C1_SCL, 50000);
  Serial.begin(115200);
  Serial.print("\033c");
  Serial.println("LM73 Reading Lab");
  lm73.lm73Init();
}

void loop() {
  if (Serial.available())
  {
    String data = Serial.readStringUntil('\r\n');
    lm73.settingResolution(data);
  }
  float temperature = lm73.lm73GetTemperature();
  Serial.printf("Temperature %f\r\n",temperature);
  delay(200);
}