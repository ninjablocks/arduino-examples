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

//Stepper---------------------------------------------------------------
#define MOTOR_CLOCKWISE      0
#define MOTOR_ANTICLOCKWISE  1
//Motor Shield pins
#define MOTORSHIELD_IN1	8
#define MOTORSHIELD_IN2	11
#define MOTORSHIELD_IN3	12
#define MOTORSHIELD_IN4	13
#define CTRLPIN_A		9
#define CTRLPIN_B		10
//

struct MotorStruct
{
  int8_t speed;
  uint8_t direction;
};

MotorStruct stepperMotor;
const int stepsPerRevolution = 200;  // change this to fit your stepper
int stepCount = 0;  // number of steps the motor has taken
int numSteps = 0;
const unsigned char stepper_ctrl[]={0x27,0x36,0x1e,0x0f};
const int INPUT_LEN = 4;
char strData[INPUT_LEN+1];
int strDATALenMax = 1;
double stepIn = 0;


static void didUpdateStepper(Device *pStepperDevice) {
  Serial.print("didUpdateStepper received: "); Serial.println(pStepperDevice->intDATA);
  stepIt(pStepperDevice->intDATA);
}


Device stepperDevice = {"0", 0, 239
    , strData, INPUT_LEN, stepIn, false
    , &didUpdateStepper
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


void stepIt(int steps)
{
	int steps_left = abs(steps)*4;
	int step_number;
	int millis_delay = 60L * 1000L /stepsPerRevolution/(stepperMotor.speed + 50);
	if (steps > 0) 
	{
		stepperMotor.direction= MOTOR_ANTICLOCKWISE;
		step_number = 0; 
	}
    else if (steps < 0) 
	{
		stepperMotor.direction= MOTOR_CLOCKWISE;
		step_number = stepsPerRevolution;
	}
	else return;
	while(steps_left > 0) 
	{
		PORTB = stepper_ctrl[step_number%4];
		delay(millis_delay);
		if(stepperMotor.direction== MOTOR_ANTICLOCKWISE)
		{
			step_number++;
		    if (step_number == stepsPerRevolution)
		    	step_number = 0;
		}
		else 
		{
			step_number--;
		    if (step_number == 0)
		    	step_number = stepsPerRevolution;
		}
		steps_left --;	
	}
}


void stop()
{
	/*Unenble the pin, to stop the motor. */
    digitalWrite(CTRLPIN_A,LOW);
    digitalWrite(CTRLPIN_B,LOW);
}




void initializeStepper()
{
	pinMode(MOTORSHIELD_IN1,OUTPUT);
	pinMode(MOTORSHIELD_IN2,OUTPUT);
	pinMode(MOTORSHIELD_IN3,OUTPUT);
	pinMode(MOTORSHIELD_IN4,OUTPUT);
	pinMode(CTRLPIN_A,OUTPUT);
	pinMode(CTRLPIN_B,OUTPUT);
	stop();
	stepperMotor.speed = 25;
	stepperMotor.direction = MOTOR_CLOCKWISE;
}

//------------------------------------------------------------------------------------------------------------


void initDevices()
{
 initializeStepper();//Initialization for the stepper motor.
  
  for (int i=0; i<DEVICES_MAX + 1; i++) {
    devices[i] = (Device*)0;
  }
 
  devices[0] = &stepperDevice;
  didUpdateStepper(&stepperDevice);
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
  Serial.println("Starting stepper hex");
  initDefaultPins();
  initDevices();
  nOBJECTS.connectDevices(devices);
  stepIt(200);

}

void loop()
{
  nOBJECTS.sendObjects();				// Send Ninja Objects 
  nOBJECTS.doReactors();				// Receive Ninja Objects reactors  
}



