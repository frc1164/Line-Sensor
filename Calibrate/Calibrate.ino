//Calibrate LineSensor and send claibration data to roborio
#include <QTRSensors.h>

#define TIMEOUT 2500
#define EMITTER_PIN 53
#define NUM_SENSORS 40

QTRSensorsRC qtrrc1((unsigned char[]) {48,49,50,51,52,43,44,45,46,47,32,33,34,35,36}, 
  16, TIMEOUT, EMITTER_PIN);

QTRSensorsRC qtrrc2((unsigned char[]) {38,39,40,41,42,37,31,30,29,28,27,26,25,24,23,22,21}, 
  16, TIMEOUT, EMITTER_PIN);

QTRSensorsRC qtrrc3((unsigned char[]) {20,19,18,17,16,15,14,0},
  8, TIMEOUT, EMITTER_PIN);

unsigned int sensorValues[NUM_SENSORS];
unsigned int sensorArray1[16];
unsigned int sensorArray2[16];
unsigned int sensorArray3[8];

void setup(){
  Serial.begin(9600);

  Serial.println("Beginning Calibration");

  //calibrate sensors in Array1
  for(int i = 0; i<400; i++){
    qtrrc1.calibrate(QTR_EMITTERS_ON_AND_OFF);
  }// of calibration1

  //calibrate sensors in Array2
  for(int i = 0; i<400; i++){
    qtrrc2.calibrate();
  }// of calibration2

  //calibrate sensors in Array3
  for(int i = 0; i<400; i++){
    qtrrc3.calibrate();
  }// of calibration3

  Serial.println("Calibration Finished");

  Serial.print("\t\t{");
  for(int i =0; i < 16; i++){
    Serial.print(qtrrc1.calibratedMaximumOn[i]);
    Serial.print(",");
  }//end Max output1

  for(int i =0; i < 16; i++){
    Serial.print(qtrrc2.calibratedMaximumOn[i]);
    Serial.print(",");
  }//end Max output2

  for(int i =0; i < 8; i++){
    Serial.print(qtrrc3.calibratedMaximumOn[i]);
    if(i < 7) Serial.print(",");
  }//end Max output3

  Serial.print("};\n");
  Serial.print("\t\t{");

  for(int i =0; i < 16; i++){
    if(qtrrc1.calibratedMinimumOn[i] == 2500) Serial.print('8');
    else{Serial.print(qtrrc1.calibratedMinimumOn[i]);}
    Serial.print(",");
  }//end Min output1

  for(int i =0; i < 16; i++){
    if(qtrrc2.calibratedMinimumOn[i] == 2500) Serial.print('8');
    else{Serial.print(qtrrc2.calibratedMinimumOn[i]);}
    Serial.print(",");
  }//end Min output2

  for(int i =0; i < 8; i++){
    if(qtrrc3.calibratedMinimumOn[i] == 2500) Serial.print('8');
    else{Serial.print(qtrrc3.calibratedMinimumOn[i]);}
    if(i < 7) Serial.print(",");
  }//end Min output3


  Serial.print("};\n");
  
}// of setup

void loop(){
  
}// of loop
