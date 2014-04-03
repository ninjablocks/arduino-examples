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

//LEDS---------------------------------------------------------------
#include "FastSPI_LED2.h"
#define NUM_LEDS 10

Device *devices[DEVICES_MAX + 1]; //null terminated
const int INPUT_LEN = 50;
char strData[INPUT_LEN+1];
CRGB leds[NUM_LEDS];
CRGB orange = CRGB(0, 21, 255);
CRGB yellow = CRGB(0,255,242);
CRGB traceBGColor = CRGB::Black;
CRGB traceColor = CRGB::Yellow;
CRGB curColor;
int numTracerPx = 15;
byte in;
const int DATAPIN = 3;
const int CLOCKPIN = 4;
double colorIn = 0;
char* colorData; 

Device lightDevice = {"0", 0, 224
    , strData, INPUT_LEN, colorIn, true
    , &didUpdateLights
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

//void setStripColor(CRGB color) {
////  LEDS.showColor(color);
//  LEDS.showColor(CRGB(color.b,color.g,color.r));
//  delay(1);
//}


  
  

void setStripColor(char* jsonStr) {
  Serial.print("Json object len: ");
  Serial.println(strlen(jsonStr));
 
  aJsonObject* root = aJson.parse(jsonStr);
  
  if (root != NULL) {
    Serial.print("SUCCESSFULLY_PARSED_OBJECT");
  } else {
    Serial.print("ERROR_PARSING_OBJECT");
    return;
  }
  
  Serial.print("printing hue: ");
  aJsonObject* item = aJson.getObjectItem(root,"hue");
  int inHue = item->valueint >> 8; //16 to 8 bit conversion
  Serial.println(inHue);
  
//  Serial.print("printing bri: ");
//  item = aJson.getObjectItem(root,"bri");
//  int inBri = item->valueint;
//  Serial.println(inBri);
  int inBri = 254;
//  Serial.print("printing sat: ");
//  item = aJson.getObjectItem(root,"sat");
//  int inSat = item->valueint;
//  Serial.println(inSat);
int inSat = 254;

//for(int i=0;i<NUM_LEDS;i++) {
//  leds[i] = CHSV(inHue,inSat,inBri);
//}

LEDS.showColor(CHSV(inHue,inSat,inBri));

  delay(1);
}

static void didUpdateLights(Device *pLightDevice) {
  Serial.print("didUpdateLights received: "); Serial.println(pLightDevice->strDATA);
  setStripColor(pLightDevice->strDATA);
  
}




void initLEDS() {
  LEDS.setBrightness(50);
  LEDS.addLeds<WS2801, DATAPIN, CLOCKPIN, RGB>(leds, NUM_LEDS);
  curColor = orange;
//  setStripColor(curColor);
  in = 0;
  setStripColor("{\"on\":true,\"hue\":20000,\"bri\":254,\"sat\":254}");
  
}

             










//------------------------------------------------------------------------------------------------------------


void initDevices()
{
 initLEDS();//Initialization for LED strip
  
  for (int i=0; i<DEVICES_MAX + 1; i++) {
    devices[i] = (Device*)0;
  }
 
  devices[0] = &lightDevice;
  didUpdateLights(&lightDevice);
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
  Serial.println("Starting LED hax");
  initDefaultPins();
  initDevices();
  nOBJECTS.connectDevices(devices);
}

void loop()
{
  nOBJECTS.sendObjects();				// Send Ninja Objects 
  nOBJECTS.doReactors();				// Receive Ninja Objects reactors  
}



