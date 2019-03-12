
#include <QTRSensors.h>
#include <EEPROM.h>

#define TIMEOUT 2500
#define EMITTER_PIN 53
#define NUM_SENSORS 20
#define BUTTON 2


QTRDimmableRC array1((unsigned char[]){33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52},
  NUM_SENSORS, TIMEOUT, EMITTER_PIN);

unsigned int sensorValues[NUM_SENSORS] = {0};

void setup() {
  Serial.begin(9600);
// put your setup code here, to run once:
  pinMode(BUTTON, INPUT_PULLUP);
  array1.calibrate();
  updateCalibration();
  
}

void loop() {
  // put your main code here, to run repeatedly:
//  Serial.print(array1.readLine(sensorValues, QTR_EMITTERS_ON, true));
//  Serial.print('\n');

//  array1.readCalibrated(sensorValues);
//  for(int i = 0; i < NUM_SENSORS; i++){
//    Serial.print(sensorValues[i]);
//    Serial.print(' ');
//  }
//  Serial.print("\n");
  
  if(digitalRead(BUTTON) == LOW){
    //Print calibration arrays
    
//    printCalibration(array1, "Old Calibration");
//    array1.resetCalibration();
//    printCalibration(array1, "Reset Calibration");
    
    while(digitalRead(BUTTON) == LOW){
      array1.calibrate();
    }// end while
    
//    printCalibration(array1, "New Calibration");
    
    saveCalibration();
    Serial.print("\n\n");

//    Serial.println("EEPROM contents");
//    for(int i = 0; i < NUM_SENSORS * 2; i++){
//      Serial.print(EEPROM[i]);
//      Serial.print(' ');
//    }//end for
//    Serial.print("\n\n");

  }//end if

  
}
void saveCalibration(){
  for(int i = 0; i < NUM_SENSORS; i++){
    EEPROM.update(i, array1.calibratedMaximumOn[i] / 10);
    EEPROM.update(i + NUM_SENSORS, array1.calibratedMinimumOn[i] / 10);
  } // end for loop
}//end saveCalibration

void updateCalibration(){

  for(int i = 0; i < NUM_SENSORS; i++){

    array1.calibratedMaximumOn[i] = EEPROM[i] * 10;

    array1.calibratedMinimumOn[i] = EEPROM[i + NUM_SENSORS] * 10;

  }//end save step

}//end updateCalibration

void printCalibration(QTRSensors sensor, String message){
  Serial.println(message);
  for(int i = 0; i < NUM_SENSORS; i++){
      Serial.print(sensor.calibratedMaximumOn[i]);
      Serial.print(' ');
    }//end for
    Serial.print('\n');
    for(int i = 0; i < NUM_SENSORS; i++){
      Serial.print(sensor.calibratedMinimumOn[i]);
      Serial.print(' ');
    }//end for
    Serial.print('\n');
}//end printCalibration
