

/*************************************************** 
  This is a library for the Adafruit PT100/P1000 RTD Sensor w/MAX31865

  Designed specifically to work with the Adafruit RTD Sensor
  ----> https://www.adafruit.com/products/3328

  This sensor uses SPI to communicate, 4 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/
#include <Arduino.h>
#include <Adafruit_MAX31865.h>

// Use software SPI: CS, DI, DO, CLK
//Adafruit_MAX31865 thermo = Adafruit_MAX31865(10, 11, 12, 13);
// use hardware SPI, just pass in the CS pin
Adafruit_MAX31865 thermo = Adafruit_MAX31865(5);

// The value of the Rref resistor. By default, use 430.0 for PT100 and 4300.0 for PT1000
// However for the green module, it uses 402.0 Ohm Rref resistor
#define RREF      402.0
// The 'nominal' 0-degrees-C resistance of the sensor
// 100.0 for PT100, 1000.0 for PT1000
#define RNOMINAL  100.0

// Setup function to set up the MAX31865 module
void setup() {
  Serial.begin(115200);
  Serial.println("Adafruit MAX31865 PT100 Sensor Test!");
  
  // Uncomment either one of them to set to either 
  // 2 Wire, 3 Wire, or 4 Wire configuration according 
  // to the number of leads on your PT100 RTD 
  thermo.begin(MAX31865_3WIRE);  
  //thermo.begin(MAX31865_2WIRE);
  //thermo.begin(MAX31865_4WIRE);
}

// Loop function to read temperature
void loop() {
  // read raw unsigned 16-bit integer RTD value from the module
  uint16_t rtd = thermo.readRTD();
  Serial.print("RTD raw: ");
  Serial.print(rtd);
  Serial.print(" | ");

  // Convert the raw unsigned 16-bit integer RTD value to resistance
  // The following code is provided by Adafruit

  // Find the resistance ratio
  float ratio = rtd;
  ratio /= 32768; // 
  Serial.print("Ratio = ");
  Serial.print(ratio,8);
  Serial.print(" | ");

  // multiply the ratio by the Rref resistor value
  float resistance = RREF * ratio;
  Serial.print("Resistance = ");
  Serial.print(resistance,8);
  Serial.print(" | ");

  // Obtain the temperature
  float tempC = thermo.temperature(RNOMINAL, RREF);
  Serial.print("Temp = ");
  Serial.print(tempC);
  Serial.println(" °C");


  // Uncomment this section of code below to check or modify
  // The RTD raw value minimum and maximum thresholds
  /*
  thermo.setThresholds(0, 17000);
  Serial.print("Low threshold: ");
  Serial.print(thermo.getLowerThreshold());
  Serial.print(" | ");
  Serial.println(thermo.getUpperThreshold());
  */

  // Check and print any faults
  // read raw unsigned 8-bit integer fault value
  uint8_t fault = thermo.readFault();

  // Check which fault(s) is/are raised by the IC
  // There can be multiple faults raised at the same time
  if (fault) {
    Serial.print("Fault 0x"); Serial.println(fault, HEX);
    if (fault & MAX31865_FAULT_HIGHTHRESH) {
      Serial.println("RTD High Threshold"); 
    }
    if (fault & MAX31865_FAULT_LOWTHRESH) {
      Serial.println("RTD Low Threshold"); 
    }
    if (fault & MAX31865_FAULT_REFINLOW) {
      Serial.println("REFIN- > 0.85 x Bias"); 
    }
    if (fault & MAX31865_FAULT_REFINHIGH) {
      Serial.println("REFIN- < 0.85 x Bias - FORCE- open"); 
    }
    if (fault & MAX31865_FAULT_RTDINLOW) {
      Serial.println("RTDIN- < 0.85 x Bias - FORCE- open"); 
    }
    if (fault & MAX31865_FAULT_OVUV) {
      Serial.println("Under/Over voltage"); 
    }
    // clear the fault(s) for the next consecutive loops
    thermo.clearFault();
  }
  Serial.println();

  // Wait for 100 milliseconds before taking the next reading
  //delay(100);
}