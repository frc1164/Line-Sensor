#include <QTRSensors.h>

#define TIMEOUT 2500
#define EMITTER_PIN 53
#define NUM_SENSORS 10
#define STATUS_LIGHT 13

QTRSensorsRC qtrrc((unsigned char[]) {52,51,50,49,48,47,46,45,44,43}, 
  NUM_SENSORS, TIMEOUT, EMITTER_PIN);

unsigned int sensorValues[NUM_SENSORS];
double offsets[] = {-2.25,-1.75,-1.25,-.75,-.25,.25,.75,1.25,1.75,2.25};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  //calibrate sensor to create calibratedMax and CalibratedMin arrays
  qtrrc.calibrate(QTR_EMITTERS_ON);

  //set saved calibration values HERE
  unsigned int savedMax[NUM_SENSORS] = {1480,1216,316,960,316,1352,1480,1432,1288,1288};
  unsigned int savedMin[NUM_SENSORS] = {196,132,132,132,132,192,192,192,192,252};

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
   offset = 0.0;}
  else{ //find offset from center by averaging the offset of each end of the white line
    offset = (offsets[z1] + offsets[z2])/2.0;
    }//end if else
    
    Serial.print(offset);
    Serial.print("\n");
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
