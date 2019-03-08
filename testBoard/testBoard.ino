
#include <QTRSensors.h>
#include <EEPROM.h>

#define TIMEOUT 2500
#define EMITTER_PIN 53
#define NUM_SENSORS 20
#define BUTTON 2


QTRDimmableRC array1((unsigned char[]){50,51,52,53,54,43,44,45,46,47,38,39,40,41,42,33,34,35,36,37},
  NUM_SENSORS, TIMEOUT, EMITTER_PIN);

unsigned int sensorValues[16] = {0};

void setup() {
  Serial.begin(9600);
// put your setup code here, to run once:
  pinMode(BUTTON, INPUT_PULLUP);
  array1.calibrate();
  updateCalibration();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(array1.readLine(sensorValues, QTR_EMITTERS_ON, true));
  Serial.print('\n');

  if(digitalRead(BUTTON) == LOW){
    while(digitalRead(BUTTON) == LOW){
      array1.calibrate();
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
    EEPROM.update(i, array1.calibratedMaximumOn[i]);
    EEPROM.update(i + NUM_SENSORS, array1.calibratedMinimumOn[i]);
  } // end for loop
}//end saveCalibration

void updateCalibration(){

  for(int i = 0; i < NUM_SENSORS; i++){

    array1.calibratedMaximumOn[i] = EEPROM[i];

    array1.calibratedMinimumOn[i] = EEPROM[i + NUM_SENSORS];

  }//end save step

}//end updateCalibration
