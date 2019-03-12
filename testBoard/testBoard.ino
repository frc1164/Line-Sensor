
#include <QTRSensors.h>
#include <EEPROM.h>

#define TIMEOUT 2500
#define EMITTER_PIN 53
#define NUM_SENSORS 20
#define BUTTON 2
#define COUNTS_TO_AVERAGE 10


QTRDimmableRC array1((unsigned char[]){33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52},
  NUM_SENSORS, TIMEOUT, EMITTER_PIN);

unsigned int sensorValues[NUM_SENSORS] = {0};
unsigned int count = 0;
unsigned long avg = 0;
unsigned int storedVals[COUNTS_TO_AVERAGE] = {0};

void setup() {
  Serial.begin(9600);
// put your setup code here, to run once:
  pinMode(BUTTON, INPUT_PULLUP);
  array1.calibrate();
  updateCalibration();  
}

void loop() {
  // put your main code here, to run repeatedly:

  if(count == COUNTS_TO_AVERAGE){
    for(int i = 0; i < COUNTS_TO_AVERAGE; i++){
      avg += storedVals[i];
    }//end for
    avg = avg/count;
    Serial.print(avg);
    Serial.print('\n');

    avg = 0;
    count = 0;

    
  }//end if

  storedVals[count] = array1.readLine(sensorValues, QTR_EMITTERS_ON, true);
  count++;
//  array1.readCalibrated(sensorValues);
//  for(int i = 0; i < NUM_SENSORS; i++){
//    Serial.print(sensorValues[i]);
//    Serial.print(' ');
//  }
//  Serial.print("\n");
  
  if(digitalRead(BUTTON) == LOW){
    //Print calibration arrays
    
    Serial.println("Old Calibration");
    for(int i = 0; i < NUM_SENSORS; i++){
        Serial.print(array1.calibratedMaximumOn[i]);
        Serial.print(' ');
      }//end for
      Serial.print('\n');
    for(int i = 0; i < NUM_SENSORS; i++){
        Serial.print(array1.calibratedMinimumOn[i]);
        Serial.print(' ');
      }//end for
    Serial.print('\n');   
     
    array1.resetCalibration();
    
    Serial.println("Reset Calibration");
    for(int i = 0; i < NUM_SENSORS; i++){
      Serial.print(array1.calibratedMaximumOn[i]);
      Serial.print(' ');
    }//end for
    Serial.print('\n');
    for(int i = 0; i < NUM_SENSORS; i++){
      Serial.print(array1.calibratedMinimumOn[i]);
      Serial.print(' ');
    }//end for
    Serial.print('\n');
    
    while(digitalRead(BUTTON) == LOW){
      array1.calibrate();
    }// end while
    
    Serial.println("New Calibration");
    for(int i = 0; i < NUM_SENSORS; i++){
        Serial.print(array1.calibratedMaximumOn[i]);
        Serial.print(' ');
      }//end for
      Serial.print('\n');
      for(int i = 0; i < NUM_SENSORS; i++){
        Serial.print(array1.calibratedMinimumOn[i]);
        Serial.print(' ');
      }//end for
    Serial.print('\n');
      
    saveCalibration();
    Serial.print("\n\n");

    Serial.println("EEPROM contents");
    for(int i = 0; i < NUM_SENSORS * 4; i++){
      Serial.print(EEPROM[i]);
      Serial.print(' ');
    }//end for
    Serial.print("\n\n");

  }//end if

  
}
void saveCalibration(){
  for(int i = 0; i < NUM_SENSORS; i++){
    EEPROM.update(i*2, array1.calibratedMaximumOn[i] / 10);
    EEPROM.update((i*2)+1, array1.calibratedMaximumOn[i] % 10);
    EEPROM.update((i + NUM_SENSORS)*2, array1.calibratedMinimumOn[i] / 10);
    EEPROM.update((i+NUM_SENSORS)*2 + 1, array1.calibratedMinimumOn[i] % 10);
  } // end for loop
}//end saveCalibration

void updateCalibration(){

  for(int i = 0; i < NUM_SENSORS; i++){

    array1.calibratedMaximumOn[i] = EEPROM[i*2] * 10 + EEPROM[(i*2)+1];

    array1.calibratedMinimumOn[i] = EEPROM[(i + NUM_SENSORS)*2] * 10 + EEPROM[(i + NUM_SENSORS)*2 + 1];

  }//end save step

}//end updateCalibration
