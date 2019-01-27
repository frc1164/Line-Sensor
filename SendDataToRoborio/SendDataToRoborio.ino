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
  unsigned int savedMax[NUM_SENSORS] = {2500,1716,2160,1660,2020};
  unsigned int savedMin[NUM_SENSORS] = {604,480,520,448,456};
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
  int z1, z2;
  char dir;
  z1 = z2 = -1;

  //locate white line from sensorValues
  for(int i = 0; i < NUM_SENSORS; i++){
    if(sensorValues[i] == 0 && z1 == -1) z1 = i;
    else if(sensorValues[i] == 0 && z2 == -1) {
      z2 = i; 
      break;
    }//end else if
  }// end for

  //determine direction
  if(z2 == -1){
    if(z1 > 2) dir = 'R';

    else if(z1 < 2) dir = 'F'; 

    else dir = 'N';
  }//end if

  else{
    double avg = ((double)z1 + z2)/2;
    if(avg > 2) dir = 'R';
    else if (avg < 2) dir = 'F';
  }//end else

  //send drive signal
  Serial.println(dir);

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

