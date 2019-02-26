#include <QTRSensors.h>
#include <EEPROM.h>

#define TIMEOUT 2500
#define EMITTER_PIN 53
#define NUM_SENSORS 40
#define SENSORS_PER_MODULE 5
#define SENSORS_PER_ARRAY 20

QTRSensorsRC A1Module1((unsigned char[]) {0,14,15,16,17}, 
  SENSORS_PER_MODULE, TIMEOUT, EMITTER_PIN);
  
QTRSensorsRC A1Module2((unsigned char[]) {18,19,20,21,22}, 
  SENSORS_PER_MODULE, TIMEOUT, EMITTER_PIN);
  
QTRSensorsRC A1Module3((unsigned char[]) {23,24,25,26,27}, 
  SENSORS_PER_MODULE, TIMEOUT, EMITTER_PIN);
  
 QTRSensorsRC A1Module4((unsigned char[]) {28,29,30,31,32}, 
  SENSORS_PER_MODULE, TIMEOUT, EMITTER_PIN);
  
QTRSensorsRC A2Module1((unsigned char[]) {33,34,35,36,37}, 
  SENSORS_PER_MODULE, TIMEOUT, EMITTER_PIN);

QTRSensorsRC A2Module2((unsigned char[]) {38,39,40,41,42}, 
  SENSORS_PER_MODULE, TIMEOUT, EMITTER_PIN);
  
QTRSensorsRC A2Module3((unsigned char[]) {43,44,45,46,47}, 
  SENSORS_PER_MODULE, TIMEOUT, EMITTER_PIN);
  
QTRSensorsRC A2Module4((unsigned char[]) {48,49,50,51,52}, 
  SENSORS_PER_MODULE, TIMEOUT, EMITTER_PIN);

QTRSensorsRC dummy((unsigned char[]) {48,49,50,51,52}, 
  SENSORS_PER_MODULE, TIMEOUT, EMITTER_PIN);

double array1_offsets[20] = {-7.875, -6.750, -5.625, -4.500, -3.375, -2.25, -1.75, -1.25, -0.75, -0.25, 0.25, 0.75, 1.25, 1.75, 2.25, 3.375, 4.500, 5.625, 6.750, 7.875};
double array2_offsets[20] = {-4.75, -4.25, -3.75, -3.25, -2.75, -1.75, -1.25, -0.75, -0.25, 0.25, 0.75, 1.25, 1.75, 2.25, 2.75, 3.25, 3.75, 4.25, 4.75};

QTRSensorsRC Array1[4] = {A1Module1,A1Module2,A1Module3,A1Module4};

QTRSensorsRC Array2[4] = {A2Module1,A2Module2,A2Module3,A2Module4};

unsigned int A1CalMax[4][5] = {0};
unsigned int A1CalMin[4][5] = {0};





void setup() {
  // put your setup code here, to run once:
   Serial.begin(115200);
   saveCalibration();
   for(int i = 0; i < NUM_SENSORS*2; i++){
    if(i%5 == 0) Serial.print('\n');
    Serial.print(EEPROM[i]);
    Serial.print(' ');
   }//end for
}// end of setup

void loop() {
  // put your main code here, to run repeatedly:
  
//  sendRaw(Array1[0]);
//  
//  Serial.print('\n');
}// end of loop()






void sendRaw(QTRSensorsRC module){
  unsigned int data[5];
  module.read(data);
  
  for(int i = 0; i < 5; i++){
    Serial.print(data[i]);
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

void updateCalibration(){
  for(int module = 0; module < 4; module++){
    for(int sensor = 0; sensor < 5; sensor++){
      Array1[module].calibratedMaximumOn[sensor] = A1CalMax[module][sensor];
    }//end inner for
  }//end outer for
  
}//end calibrateArrays

void saveCalibration(){
  for(int i = 0; i < 80; i++){
    for(int module = 0; module < 4; module++){
      Array1[module].calibrate();
      Array2[module].calibrate();
    }//end for
  }//end for

  for(int module = 0; module < 4; module++){
    for(int sensor = 0; sensor < 5; sensor++){
       EEPROM.update(sensor + module, Array1[module].calibratedMaximumOn[sensor]);
       EEPROM.update(sensor + module + SENSORS_PER_ARRAY, Array1[module].calibratedMinimumOn[sensor]);
       EEPROM.update(sensor + module + NUM_SENSORS, Array2[module].calibratedMaximumOn[sensor]);
       EEPROM.update(sensor + module + NUM_SENSORS + SENSORS_PER_ARRAY, Array2[module].calibratedMinimumOn[sensor]);
    }//end inner for
  }//end outer for

  
  
}//end calibrateArrays

void readAll(unsigned int *data){
  unsigned int tmp[5];
  for(int module = 0; module < 4; module++){
    Array1[module].readCalibrated(tmp);
    for(int sensor = 0; sensor < 5; sensor++){
      data[module + sensor] = tmp[sensor];
    }//end inner for
  }//end outer for
}
