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
   sendData();
  delay(250);
}// end of loop()

void sendData(){
  //send the sensor data to serial
   qtrrc.readCalibrated(sensorValues);

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
