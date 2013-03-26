// TODO! http://qgroundcontrol.org/dev/mavlink_arduino_integration_tutorial

#define DEBUG_V

#include <Wire.h>
#include <DebugUtils.h>
#include <MS561101BA.h>


#define MOVAVG_SIZE 32

MS561101BA baro = MS561101BA();

float movavg_buff[MOVAVG_SIZE];
int movavg_i=0;

const float sea_press = 1013.25;
float press, temperature;

void setup() {
  Wire.begin();
  Serial.begin(57600);
  delay(1000);

  // Suppose that the CSB pin is connected to GND.
  // You'll have to check this on your breakout schematics
  baro.init(MS561101BA_ADDR_CSB_LOW); 
  delay(100);
  
  // populate movavg_buff before starting loop
  for(int i=0; i<MOVAVG_SIZE; i++) {
    movavg_buff[i] = baro.getPressure(MS561101BA_OSR_4096);
  }
}

void loop() {
  Serial.print(" temp: ");
  temperature = baro.getTemperature(MS561101BA_OSR_4096);
  Serial.print(temperature);
  Serial.print(" degC pres: ");
  
  press = baro.getPressure(MS561101BA_OSR_4096);
  pushAvg(press);
  press = getAvg(movavg_buff, MOVAVG_SIZE);
  Serial.print(press);
  Serial.print(" mbar altitude: ");
  Serial.print(getAltitude(press, temperature));
  Serial.println(" m");
}

float getAltitude(float press, float temp) {
  //return (1.0f - pow(press/101325.0f, 0.190295f)) * 4433000.0f;
  return ((pow((sea_press / press), 1/5.257) - 1.0) * (temp + 273.15)) / 0.0065;
}

void pushAvg(float val) {
  movavg_buff[movavg_i] = val;
  movavg_i = (movavg_i + 1) % MOVAVG_SIZE;
}

float getAvg(float * buff, int size) {
  float sum = 0.0;
  for(int i=0; i<size; i++) {
    sum += buff[i];
  }
  return sum / size;
}
