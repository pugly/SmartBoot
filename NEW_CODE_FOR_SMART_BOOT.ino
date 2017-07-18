#include <DS1307RTC.h>

#include <HX711.h>

#include <SD.h>
#include <SPI.h>

#include <TimeAlarms.h>

#include <Time.h>
#include <TimeLib.h>

int step_count;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect
  }

  
  setTime(RTC.get()); //set time to current time
  Alarm.alarmRepeat(0, 0, 0, SD_Write); //performs the SD_Write function at 0:00 everyday 

  Serial.print("Initializing SD card...");
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
}

void loop() {
  // put your main code here, to run repeatedly:

}

void SD_Write() {
  File dataFile = SD.open("TEST.txt", FILE_WRITE); //Opens file for read and write 
  if (dataFile) {
    dataFile.println(String(RTC.get()) + ": " + String(step_count)); //Writes the step time and the current time
    dataFile.close(); //closes the file 
  }
  // if the file isn't open, pop up an error:
  else {
  dataFile.println(String(RTC.get()) + "Error"); //Writes the step time and the current time
  dataFile.close(); //closes the file   
  }
}

