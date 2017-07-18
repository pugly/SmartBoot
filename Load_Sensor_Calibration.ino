/*
   circuits4you.com
   2016 November 25
   Load Cell HX711 Module Interface with Arduino to measure weight in Kgs
  Arduino
  pin
  5V -> VCC
  GND -> GND

  Most any pin on the Arduino Uno will be compatible with DOUT/CLK.
  The HX711 board can be powered from 2.7V to 5V so the Arduino 5V power should be fine.
*/

#include "HX711.h"  //load cell HX711 Module interface with arduino to measure weight in Kgs

HX711 scale1(A1, A0); //Front
HX711 scale2(A5, A4); //Back
HX711 scale3(A3, A2); //Back Middle
HX711 scale4(4, 5); //Front Middle

//Change this calibration factor as per your load cell once it is found you many need to vary it in thousands
float calibration_factor = -96650; //-106600 worked for my 40Kg max scale setup

//=============================================================================================
//                         SETUP
//=============================================================================================
void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("HX711 Calibration");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press a,s,d,f to increase calibration factor by 10,100,1000,10000 respectively");
  Serial.println("Press z,x,c,v to decrease calibration factor by 10,100,1000,10000 respectively");
  Serial.println("Press t for tare");
  scale1.set_scale();
  scale2.set_scale();
  scale3.set_scale();
  scale4.set_scale();

  scale1.tare(); //Reset the scale to 0
  scale2.tare();
  scale3.tare();
  scale4.tare();

  long zero_factor1 = scale1.read_average(); //Get a baseline reading
  long zero_factor2 = scale2.read_average();
  long zero_factor3 = scale3.read_average();
  long zero_factor4 = scale4.read_average();
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor1);
  Serial.println(zero_factor2);
  Serial.println(zero_factor3);
  Serial.println(zero_factor4);
}

//=============================================================================================
//                         LOOP
//=============================================================================================
void loop() {

  scale1.set_scale(calibration_factor); //Adjust to this calibration factor
  scale2.set_scale(calibration_factor);
  scale3.set_scale(calibration_factor);
  scale4.set_scale(calibration_factor);

  Serial.print("Reading: ");
  Serial.print(scale1.get_units(1)); Serial.print(" , "); //Left Front
  Serial.print(scale2.get_units(1)); Serial.print(" , "); //Right Front
  Serial.print(scale3.get_units(1)); Serial.print(" , "); //Left Back
  Serial.print(scale4.get_units(1)); Serial.print(" , "); //Right Back
  Serial.print(" kg"); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
  Serial.print(" calibration_factor: ");
  Serial.print(calibration_factor);
  Serial.println();

  if (Serial.available())
  {
    char temp = Serial.read();
    if (temp == '+' || temp == 'a')
      calibration_factor += 10;
    else if (temp == '-' || temp == 'z')
      calibration_factor -= 10;
    else if (temp == 's')
      calibration_factor += 100;
    else if (temp == 'x')
      calibration_factor -= 100;
    else if (temp == 'd')
      calibration_factor += 1000;
    else if (temp == 'c')
      calibration_factor -= 1000;
    else if (temp == 'f')
      calibration_factor += 10000;
    else if (temp == 'v')
      calibration_factor -= 10000;
    else if (temp == 't')
      scale1.tare();  //Reset the scale to zero
      scale2.tare();
      scale3.tare();
      scale4.tare();
  }
}
//=============================================================================================


