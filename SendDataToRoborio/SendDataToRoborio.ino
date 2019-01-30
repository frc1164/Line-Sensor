#include <QTRSensors.h>

#define TIMEOUT 2500
#define EMITTER_PIN 53
#define NUM_SENSORS 5
#define STATUS_LIGHT 13

QTRSensorsRC qtrrc((unsigned char[]) {52,51,50,49,48}, 
  NUM_SENSORS, TIMEOUT, EMITTER_PIN);

unsigned int sensorValues[NUM_SENSORS];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  //calibrate sensor to create calibratedMax and CalibratedMin arrays
  qtrrc.calibrate(QTR_EMITTERS_ON);

  //set saved calibration values HERE
  unsigned int savedMax[NUM_SENSORS] = {2500,1816,2488,1720,2484};
  unsigned int savedMin[NUM_SENSORS] = {1220,740,1024,768,1024};
  for(int i = 0; i < NUM_SENSORS; i++){
    qtrrc.calibratedMaximumOn[i] = savedMax[i];
    qtrrc.calibratedMinimumOn[i] = savedMin[i]; 
  }//end for

}// end of setup

void loop() {
  // put your main code here, to run repeatedly:
  qtrrc.readCalibrated(sensorValues);
  //sendRawData();
  roughTuning();
  delay(250);
}// end of loop()

void roughTuning(){
  //initial, quick and dirty Line sensing step
  int z1 = -1;
  int z2 = -1;
  double offset;

  //locate first zero
  for(int i = 0; i < NUM_SENSORS; i++){
    if(sensorValues[i] == 0){
      z1 = i;
      break;
    }//end if
  }//end for

  //locate last zero in array
  for(int i = NUM_SENSORS - 1; i >= 0; i-- ){
    if(sensorValues[i] == 0){
      z2 = i;
      break;
    }//end if
  }//end for

  if(z1 == -1 && z2 == -1){// test if no line was found
    Serial.println('N');
  }
  else {
    //find offset from center by averaging indeces an taking distance from center
    offset = (z1 + z2)/2.0 - (NUM_SENSORS / 2.0);
    Serial.println(offset, 2);
  }
}//end roughTuning

void sendRawData(){
  //send the sensor data to serial

  String dataLine = "";
  //Output data to serial
  for(int i = 0; i < NUM_SENSORS; i++){
    dataLine += (String)sensorValues[i];
    dataLine += '\t';
  }// of serial output 
  Serial.println(dataLine);

  //reset dataline
  dataLine = "";
}//end sendData

