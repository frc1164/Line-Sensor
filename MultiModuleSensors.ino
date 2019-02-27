// MultiModuleSensors
// Written Ian Rankin Febuary 2019
//
// Reads

#include <QTRSensors.h>

#include <EEPROM.h>



#define NUM_SENSORS 5

#define TIMEOUT 3000

#define EMITTER_PIN 53

#define BAUDRATE 9600

#define NUM_SENSORS 5

#define BUTTON 2

#define CALIBRATION_CONSTANT 0



// list of sensor values.
unsigned char gpioPins[][5] = { {0,14,15,16,17},
                              {18,19,20,21,22},
                              {23,24,25,26,27},
                              {28,29,30,31,32},
                              {33,34,35,36,37},
                              {38,39,40,41,42},
                              {43,44,45,46,47},
                              {48,49,50,51,52},
                              {4,5,6,7,8},
                              {9,10,11,12,13}};

unsigned int sensorValues[NUM_SENSORS];

#define MODULE 10

QTRDimmableRC qtrrc(gpioPins[MODULE - 1],
  NUM_SENSORS, TIMEOUT, EMITTER_PIN);


void setup() {
  Serial.begin(BAUDRATE);
  
  qtrrc.calibrate();
  updateCalibration();

  for(int i = 0; i < NUM_SENSORS * 2; i++){
    Serial.print(EEPROM[i]);
    Serial.print(' ');
  }//end for

  // set pin mode for single button
  pinMode(BUTTON, INPUT_PULLUP);
}

void loop() {
  // read a single module.
  qtrrc.readCalibrated(sensorValues);
  for(int i = 0; i < NUM_SENSORS; i++){

    Serial.print(sensorValues[i]);

    Serial.print(' ');

  }//end for 
  Serial.println();


  //////////////////// Check button and perform calibration.
  if(digitalRead(BUTTON) == LOW){
    while(digitalRead(BUTTON) == LOW){
      qtrrc.calibrate();
    }// end while
    
    saveCalibration();
    Serial.print("\n\n");
    
    for(int i = 0; i < NUM_SENSORS * 2; i++){
      Serial.print(EEPROM[i]);
      Serial.print(' ');
    }//end for
    Serial.print("\n\n");

  }//end if
  
} //





void saveCalibration(){
  for(int i = 0; i < NUM_SENSORS; i++){
    EEPROM.update(i, qtrrc.calibratedMaximumOn[i]);
    EEPROM.update(i + NUM_SENSORS, qtrrc.calibratedMinimumOn[i] - CALIBRATION_CONSTANT);
  } // end for loop
}//end saveCalibration


void updateCalibration(){

  for(int i = 0; i < NUM_SENSORS; i++){

    qtrrc.calibratedMaximumOn[i] = EEPROM[i];

    qtrrc.calibratedMinimumOn[i] = EEPROM[i + NUM_SENSORS];

  }//end save step

}//end updateCalibration


