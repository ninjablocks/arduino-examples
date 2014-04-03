/*
This program drives a unipolar or bipolar stepper motor using the Seeed studio stepper driver on the Ninja Blocks platform.
 */
 

#include <Ninja.h>
#include <DHT22.h>
#include <I2C.h>
#include <aJSON.h>
#include <MMA8453Q.h>
#include <RCSwitch.h>
#include <Wire.h>
#include <NinjaPi.h>

#define DEVICES_MAX          10
Device *devices[DEVICES_MAX + 1]; //null terminated
static void updateFromCloud(Device *pMyDevice);
int sensorPin = A0;    // analog input pin 
double myDATA;
const int INPUT_LEN = 4;
char strData[INPUT_LEN+1];
double count = 0;


//most important thing to change here is the device ID
Device myDevice = {"0", 0, 9
    , strData, INPUT_LEN, myDATA, false
    , &updateFromCloud
};              

//struct Device {
//	char * strGUID;
//	const int intVID;
//	const int intDID;
//	char * strDATA;
//	const int strDATALenMax;
//	double intDATA;
//	const bool IsString;
//	void (*didUpdate)(Device*);
//};


static void updateFromCloud(Device *pMyDevice) {
 Serial.println("received update from cloud");
}

void initDevices() {

  //your initialization function here
  
  for (int i=0; i<DEVICES_MAX + 1; i++) {
    devices[i] = (Device*)0;
  }
 
  devices[0] = &myDevice;
  updateFromCloud(&myDevice);
}



void initDefaultPins() {
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  digitalWrite(RED_LED_PIN, HIGH);            // set the RED LED  Off
  digitalWrite(GREEN_LED_PIN, HIGH);          // set the GREEN LED Off
  digitalWrite(BLUE_LED_PIN, HIGH);           // set the BLUE LED Off
  pinMode(RED_STAT_LED_PIN, OUTPUT);
  pinMode(GREEN_STAT_LED_PIN, OUTPUT);
  pinMode(BLUE_STAT_LED_PIN, OUTPUT);
  digitalWrite(RED_STAT_LED_PIN, HIGH);		// set the RED STAT LED  Off
  digitalWrite(GREEN_STAT_LED_PIN, HIGH);	        // set the GREEN STAT LED Off
  digitalWrite(BLUE_STAT_LED_PIN, LOW);	        // Power on Status
}

void setup()
{
  Serial.begin(9600);
  initDefaultPins();
  initDevices();
  nOBJECTS.connectDevices(devices);
}

void loop()
{
  
 sensorValue = analogRead(sensorPin);   
 
  nOBJECTS.sendObjects();				// Send Ninja Objects 
  nOBJECTS.doReactors();				// Receive Ninja Objects reactors  
}



