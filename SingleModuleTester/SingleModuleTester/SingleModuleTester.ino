#include <QTRSensors.h>
#include <EEPROM.h>

#define NUM_SENSORS 5
#define TIMEOUT 3000
#define EMITTER_PIN 2
#define BAUDRATE 9600
#define NUM_SENSORS 5
#define BUTTON 8
#define CALIBRATION_CONSTANT 200

QTRDimmableRC qtrrc((unsigned char[]) {3,4,5,6,7},
  NUM_SENSORS, TIMEOUT, EMITTER_PIN);

unsigned int sensorValues[NUM_SENSORS];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(BAUDRATE);

  qtrrc.calibrate();
  updateCalibration();
  for(int i = 0; i < NUM_SENSORS * 2; i++){
    Serial.print(EEPROM[i]);
    Serial.print(' ');
  }//end for

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() { 
  // put your main code here, to run repeatedly:
  
  qtrrc.readCalibrated(sensorValues);
  for(int i = 0; i < NUM_SENSORS; i++){
    Serial.print(sensorValues[i]);
    Serial.print(' ');
  }//end for 

  Serial.println(); 

  if(digitalRead(BUTTON) == HIGH){
    while(digitalRead(BUTTON) == HIGH){
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
}

void saveCalibration(){
  for(int i = 0; i < NUM_SENSORS; i++){
    EEPROM.update(i, qtrrc.calibratedMaximumOn[i]);
    EEPROM.update(i + NUM_SENSORS, qtrrc.calibratedMinimumOn[i] - CALIBRATION_CONSTANT);
  }//end save step

  
}//end saveCalibration

void updateCalibration(){
  for(int i = 0; i < NUM_SENSORS; i++){
    qtrrc.calibratedMaximumOn[i] = EEPROM[i];
    qtrrc.calibratedMinimumOn[i] = EEPROM[i + NUM_SENSORS];
  }//end save step
}//end updateCalibration
