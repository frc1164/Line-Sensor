#include <QTRSensors.h>

#define TIMEOUT 2500
#define EMITTER_PIN 53
#define NUM_SENSORS 40
#define SENSORS_PER_MODULE 5

QTRSensorsRC Array1Module1((unsigned char[]) {0,14,15,16,17}, 
  SENSORS_PER_MODULE, TIMEOUT, EMITTER_PIN);
  
QTRSensorsRC Array1Module2((unsigned char[]) {18,19,20,21,22}, 
  SENSORS_PER_MODULE, TIMEOUT, EMITTER_PIN);
  
QTRSensorsRC Array1Module3((unsigned char[]) {23,24,25,26,27}, 
  SENSORS_PER_MODULE, TIMEOUT, EMITTER_PIN);
  
 QTRSensorsRC Array1Module4((unsigned char[]) {28,29,30,31,32}, 
  SENSORS_PER_MODULE, TIMEOUT, EMITTER_PIN);
  
QTRSensorsRC Array2Module1((unsigned char[]) {33,34,35,36,37}, 
  SENSORS_PER_MODULE, TIMEOUT, EMITTER_PIN);

QTRSensorsRC Array2Module2((unsigned char[]) {38,39,40,41,42}, 
  SENSORS_PER_MODULE, TIMEOUT, EMITTER_PIN);
  
QTRSensorsRC Array2Module3((unsigned char[]) {43,44,45,46,47}, 
  SENSORS_PER_MODULE, TIMEOUT, EMITTER_PIN);
  
QTRSensorsRC Array2Module4((unsigned char[]) {48,49,50,51,52}, 
  SENSORS_PER_MODULE, TIMEOUT, EMITTER_PIN);

double array1_offsets[20] = {-7.875, -6.750, -5.625, -4.500, -3.375, -2.25, -1.75, -1.25, -0.75, -0.25, 0.25, 0.75, 1.25, 1.75, 2.25, 3.375, 4.500, 5.625, 6.750, 7.875};
double array2_offsets[20] = {-4.75, -4.25, -3.75, -3.25, -2.75, -1.75, -1.25, -0.75, -0.25, 0.25, 0.75, 1.25, 1.75, 2.25, 2.75, 3.25, 3.75, 4.25, 4.75};

QTRSensorsRC Array1[4] = {Array1Module1,Array1Module2,Array1Module3,Array1Module4};

QTRSensorsRC Array2[4] = {Array2Module1,Array2Module2,Array2Module3,Array2Module4};

void setup() {
  // put your setup code here, to run once:
   Serial.begin(115200);
}// end of setup

void loop() {
  // put your main code here, to run repeatedly:
  sendCalibrated(Array1[0]);

Serial.print('\n');
}// end of loop()

void sendRawData(unsigned int *A, int size){
  for(int i = 0; i < size; i++){
    Serial.print(A[i]);
    Serial.print(' ');
  }//end for
}//end sendData

void sendCalibrated(QTRSensorsRC module){
  unsigned int data[5];
  module.readCalibrated(data);

  for(int i = 0; i < 5; i++){
    Serial.print(data[i]);
    Serial.print(' ');
  }//end for
}

void calibrateArrays(){
  unsigned int calibratedMax1[4][5];
  unsigned int calibratedMin1[4][5];
  unsigned int calibratedMax2[4][5];
  unsigned int calibratedMin2[4][5];

  for(int i = 0; i < 4; i++){
    Array1[i].calibrate();
    Array2[i].calibrate();
  }//end for

  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 5; j++){
      Array1[i].calibratedMaximumOn[j] = calibratedMax1[i][j];
      Array1[i].calibratedMinimumOn[j] = calibratedMax1[i][j];      
    }
  }
  
}


//
//void sendArray1Offset(){
//  double lineDetected[40];    
//  int i, j = 0;
//  double sum = 0;
//  for(int i = 0; i < 16; i++){
//    if(sensorArray1[i] < 10){
//      lineDetected[j] = array1_offsets[i];
//      j++;
//    }//endif
//  }//end for
//  
//  for(int i = 16; i < 20; i++){
//    if(sensorArray2[i] < 10){
//      lineDetected[j] = sensorArray2[i-16];
//      j++;
//    }//end if
//  }//end for
//
//  //average the offsets of the sensors that detected white
//  for(i = 0; i < j; i++){
//    sum += lineDetected[i];
//  }//end for
//  if(j == 0) {
//    Serial.print(0);
//    return;
//  }//end if
//  Serial.print(sum/j);
//}//end sendArray1Offset
//
//void sendArray2Offset(){
//  double lineDetected[40];    
//  double sensorValues[40];
//  int i, j = 0;
//  double sum = 0;
//  for(int i = 0; i < 16; i++){
//    sensorValues[i] = sensorArray1[i];
//  }
//  for(int i = 16; i < 32; i++){
//    sensorValues[i] = sensorArray2[i-16];
//  }
//  for(int i = 32; i < 40; i++){
//    sensorValues[i] = sensorArray3[i-32];
//  }
//
//  //Find all positions where white was detected
//  for(i = 20; i < 40; i++){
//    if(sensorValues[i] == 't'){
//      lineDetected[j] = array2_offsets[i];
//      j++;
//    }//end if
//  }//end for
//
//  //average the offsets of the sensors that detected white
//  for(i = 0; i < j; i++){
//    sum += lineDetected[i];
//  }//end for 
//  if(j == 0) {
//    Serial.print(0);
//    return;
//  }//end if
//  Serial.print(sum/j);
//}//end sendArray2Offset
