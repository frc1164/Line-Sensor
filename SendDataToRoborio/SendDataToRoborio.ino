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
  
QTRSensorsRC dummy((unsigned char[]) {43,44,45,46,47}, 5, TIMEOUT, EMITTER_PIN);
unsigned int dummyArray[5] = {0};

unsigned int sensorValues[NUM_SENSORS];
unsigned int sensorArray1[16] = {0};
unsigned int sensorArray2[16] = {0};
unsigned int sensorArray3[8] = {0};
double array1_offsets[20] = {-7.875, -6.750, -5.625, -4.500, -3.375, -2.25, -1.75, -1.25, -0.75, -0.25, 0.25, 0.75, 1.25, 1.75, 2.25, 3.375, 4.500, 5.625, 6.750, 7.875};
double array2_offsets[20] = {-4.75, -4.25, -3.75, -3.25, -2.75, -1.75, -1.25, -0.75, -0.25, 0.25, 0.75, 1.25, 1.75, 2.25, 2.75, 3.25, 3.75, 4.25, 4.75};

void setup() {
  // put your setup code here, to run once:
   Serial.begin(115200);
   calibrateArrays();
}// end of setup

void loop() {
  // put your main code here, to run repeatedly:
  
//  qtrrc1.readCalibrated(sensorArray1);
//  qtrrc2.readCalibrated(sensorArray2);
//  qtrrc3.readCalibrated(sensorArray3);
//  
//  sendData(sensorArray1, 16);
//  sendData(sensorArray2, 16);
//  sendData(sensorArray3, 8);

//  qtrrc1.read(sensorArray1);
//  qtrrc2.read(sensorArray2);
//  qtrrc3.read(sensorArray3);
//  
//  sendRawData(sensorArray1, 16);
//  sendRawData(sensorArray2, 16);
//  sendRawData(sensorArray3, 8);

dummy.read(dummyArray);
for(int i = 0; i < 5;i++){
  Serial.print(dummyArray[i]);
  Serial.print(' ');
}//end for
  
//  sendArray1Offset();
//  Serial.print(' ');
//  sendArray2Offset();

Serial.print('\n');
}// end of loop()

//send array of true or false values for each sensor, as chars t and f
void sendData(unsigned int *A, int size){
  for(int i = 0; i < size; i++){
    if(A[i] < 10) Serial.print('t');
    else Serial.print('f');
  }//end for
}//end sendData

void sendRawData(unsigned int *A, int size){
  for(int i = 0; i < size; i++){
    Serial.print(A[i]);
    Serial.print(' ');
  }//end for
}//end sendData

void calibrateArrays(){
  qtrrc1.calibrate();
  qtrrc2.calibrate();
  qtrrc3.calibrate();

  unsigned int savedMaxOn[NUM_SENSORS] = {
2500,8,96,284,1436,660,188,188,96,376,2500,860,2500,1540,284,2500,88,88,88,172,92,1444,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500
};
  unsigned int savedMinOn[NUM_SENSORS] ={ 
2400,8,8,196,956,376,100,100,96,196,8,568,1936,1148,192,8,8,8,8,92,88,1020,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8
};
                
  for(int i = 0; i < 16; i++){
    qtrrc1.calibratedMaximumOn[i] = savedMaxOn[i];
    qtrrc1.calibratedMinimumOn[i] = savedMinOn[i];
  }//end for1

  for(int i = 16; i < 32; i++){
    qtrrc2.calibratedMaximumOn[i] = savedMaxOn[i];
    qtrrc2.calibratedMinimumOn[i] = savedMinOn[i];
  }//end for2

  for(int i = 32; i < 40; i++){
    qtrrc3.calibratedMaximumOn[i] = savedMaxOn[i];
    qtrrc3.calibratedMinimumOn[i] = savedMinOn[i];
  }//end for3

  
}//end calibrateArrays

void sendArray1Offset(){
  double lineDetected[40];    
  int i, j = 0;
  double sum = 0;
  for(int i = 0; i < 16; i++){
    if(sensorArray1[i] < 10){
      lineDetected[j] = array1_offsets[i];
      j++;
    }//endif
  }//end for
  
  for(int i = 16; i < 20; i++){
    if(sensorArray2[i] < 10){
      lineDetected[j] = sensorArray2[i-16];
      j++;
    }//end if
  }//end for

  //average the offsets of the sensors that detected white
  for(i = 0; i < j; i++){
    sum += lineDetected[i];
  }//end for
  if(j == 0) {
    Serial.print(0);
    return;
  }//end if
  Serial.print(sum/j);
}//end sendArray1Offset

void sendArray2Offset(){
  double lineDetected[40];    
  double sensorValues[40];
  int i, j = 0;
  double sum = 0;
  for(int i = 0; i < 16; i++){
    sensorValues[i] = sensorArray1[i];
  }
  for(int i = 16; i < 32; i++){
    sensorValues[i] = sensorArray2[i-16];
  }
  for(int i = 32; i < 40; i++){
    sensorValues[i] = sensorArray3[i-32];
  }

  //Find all positions where white was detected
  for(i = 20; i < 40; i++){
    if(sensorValues[i] == 't'){
      lineDetected[j] = array2_offsets[i];
      j++;
    }//end if
  }//end for

  //average the offsets of the sensors that detected white
  for(i = 0; i < j; i++){
    sum += lineDetected[i];
  }//end for 
  if(j == 0) {
    Serial.print(0);
    return;
  }//end if
  Serial.print(sum/j);
}//end sendArray2Offset
