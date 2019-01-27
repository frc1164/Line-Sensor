//Calibrate LineSensor and send claibration data to roborio
#include <QTRSensors.h>

#define TIMEOUT 2500
#define EMITTER_PIN 53
#define NUM_SENSORS 5

unsigned int sensorValues[NUM_SENSORS];

unsigned int calibratedMax[NUM_SENSORS];
unsigned int calibratedMin[NUM_SENSORS];

QTRSensorsRC qtrrc((unsigned char[]) {52,51,50,49,48}, 
  NUM_SENSORS, TIMEOUT, EMITTER_PIN);

void setup(){
  Serial.begin(9600);

  Serial.println("Beginning Calibration");

  //calibrate sensors
  for(int i = 0; i<400; i++){
    qtrrc.calibrate(QTR_EMITTERS_ON);
  }// of calibration

  Serial.println("Calibration Finished");

  Serial.print('{');
  for(int i =0; i < NUM_SENSORS; i++){
    Serial.print(qtrrc.calibratedMaximumOn[i]);
    if(i < NUM_SENSORS-1) Serial.print(",");
  }
  Serial.print("}\n");
  Serial.print('{');
  for(int i =0; i < NUM_SENSORS; i++){
    Serial.print(qtrrc.calibratedMinimumOn[i]);
    if(i < NUM_SENSORS-1) Serial.print(",");
  }
  Serial.print("}\n");

  for(int i = 0; i < NUM_SENSORS; i++){
    calibratedMax[i] = qtrrc.calibratedMaximumOn[i];
    calibratedMin[i] = qtrrc.calibratedMinimumOn[i];
  }// of calibration data output

  qtrrc.resetCalibration();

  for(int i = 0; i < NUM_SENSORS; i++){

    qtrrc.calibratedMaximumOn[i] = calibratedMax[i];
    qtrrc.calibratedMinimumOn[i] = calibratedMin[i]; 
  }
  
}// of setup

void loop(){

  /*
  //qtrrc.read(sensorValues);
  //uncomment the next line to read line position estimate
  pos = qtrrc.readLine(sensorValues,QTR_EMITTERS_ON, true);
  
  //Output data to serial
  for(int i = 0; i < NUM_SENSORS; i++){
    
    Serial.print(sensorValues[i]);
    Serial.print('\t');
    
  }// of serial output 

  Serial.print(pos);
  
  Serial.println();
  */
  
}// of loop
