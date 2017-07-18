//-----SmartBoot.Gen.3.
//-----Collin Patterson, Advisor: Brian Knarr
#include <Adafruit_NeoPixel.h>
#include <HX711.h>
#include <SPI.h>
#include <SD.h>
#define PIN 8 //Digital Pin for LEDs
Sd2Card card;
SdVolume volume;
SdFile root;
File dataFile;
File myFile;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(3, PIN, NEO_GRB + NEO_KHZ800);
//
HX711 scale1(A1, A0); //Left Front
HX711 scale2(A4, A5); //Left Back
HX711 scale3(A3, A2); //Right Front
HX711 scale4(4, 6); //Right Back
const int chipSelect = 10;
float calibration_factor = 12055; 
int totalGRF;
float AccLow;
float AccHigh;
int currentValue = 0;
int currentMillis = 0;
int peakValue;
int BodyWeight;
int PartialPercent;
float collectLimit;
int countThresh;
boolean collThresh = false;
float Threshold;
int i;
int TotalMillis = 0;
int frontGRF;
int backGRF;


//---------------------------------------------------------------------
//
void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  /*while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  */
  Serial.print("Initializing SD card...");
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  // re-open the file for reading:
  myFile = SD.open("Inputs.txt");
  if (myFile) {
    Serial.println("test2.txt:");
    // read from the file until there's nothing else in it:
//    while (myFile.available()) {
      BodyWeight = myFile.parseInt();
      PartialPercent = myFile.parseInt();
      Serial.println(PartialPercent);
      Serial.println(BodyWeight);
//    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
  

  //collectLimit = (0.5*PartialPercent/100) *BodyWeight;
  collectLimit = 1; // just going to attempt this to see if it gets rid of the extra peaks
  AccLow = ((PartialPercent - 10)/100.0) * BodyWeight;
  AccHigh = ((PartialPercent + 10)/100.0) * BodyWeight;
  Threshold = 7.0; //5.0;
    Serial.print(AccLow);
  Serial.print("---");
  Serial.print(AccHigh);
    Serial.print("---");
  Serial.print(PartialPercent);
    Serial.print("---");
  Serial.print(BodyWeight);
//  
  strip.begin();
  strip.show();
//  
  scale1.set_scale();
  scale2.set_scale();
  scale3.set_scale();
  scale4.set_scale();
//
  scale1.tare(); 
  scale2.tare();
  scale3.tare();
  scale4.tare();
// 
  long zero_factor1 = scale1.read_average(); //Baseline readinhg
  long zero_factor2 = scale2.read_average(); 
  long zero_factor3 = scale3.read_average();
  long zero_facror4 = scale4.read_average();
  Serial.print("setup end");
}

void loop() {

  scale1.set_scale(calibration_factor); 
  scale2.set_scale(calibration_factor);
  scale3.set_scale(calibration_factor);
  scale4.set_scale(calibration_factor);
 
  Serial.print(scale1.get_units(1)); Serial.print(" , "); //Left Front
  Serial.print(scale3.get_units(1)); Serial.print(" , "); //Right Front
  Serial.print(scale2.get_units(1)); Serial.print(" , "); //Left Back
  Serial.print(scale4.get_units(1)); Serial.print(" , "); //Right Back
  Serial.println();
  
  totalGRF = (scale1.get_units(1)) + (scale2.get_units(1)) + (scale3.get_units(1)) + (scale4.get_units(1));
  
  File dataFile = SD.open("TEST.txt", FILE_WRITE);
  dataFile.println(String(millis()) + " , " + scale1.get_units(1) + "," + scale2.get_units(1) + "," + scale3.get_units(1) + "," + scale4.get_units(1));
  dataFile.close();

  if (totalGRF > collectLimit) {
    collect();
    collThresh = true;
    countThresh += 1;
  }
  
  else {
    if (collThresh == true) {
      if (countThresh > 10); {
        /*
        File dataFile = SD.open("TEST.txt", FILE_WRITE);
        dataFile.println(String(millis()) + " , " + peakValue);
        dataFile.close();
        */
    }
      collThresh = false;
      countThresh = 0;
      peakValue = 0;
    }
    else {
      collThresh = false;
      countThresh = 0;
      peakValue = 0;
    }
  }
 
 if (totalGRF > Threshold) {
     if (totalGRF < AccLow) {
       tooLittle();
     }
     else {
       if (totalGRF < AccHigh) {
         justRight();
       }
       else {
         tooMuch();
       }
     } 
   } 
   
 else {noColor(); }
   strip.show();
   delay(5); 
}

//-----Auxilliary Loops-------------------------------------

void collect() {  
  currentValue = totalGRF;
  if (currentValue > peakValue){
      peakValue = currentValue;  
  }
}

//-------------------------------------------------------------
void tooLittle() {
   strip.setPixelColor(0, strip.Color(0, 0, 127)); //Blue
   strip.setPixelColor(1, strip.Color(0, 0, 127)); 
   strip.setPixelColor(2, strip.Color(0, 0, 127));
   strip.show(); 
  }

void justRight() {
    strip.setPixelColor(0, strip.Color(0, 127, 0)); //Green
    strip.setPixelColor(1, strip.Color(0, 127, 0)); 
    strip.setPixelColor(2, strip.Color(0, 127, 0));
    strip.show();
  }

void tooMuch() {
    strip.setPixelColor(0, strip.Color(127, 0, 0)); //Red
    strip.setPixelColor(1, strip.Color(127, 0, 0));
    strip.setPixelColor(2, strip.Color(127, 0, 0)); 
    strip.show();
  }

void noColor() {
    strip.setPixelColor(0, strip.Color(0, 0, 0));
    strip.setPixelColor(1, strip.Color(0, 0, 0));
    strip.setPixelColor(2, strip.Color(0, 0, 0)); 
    strip.show();
  }
