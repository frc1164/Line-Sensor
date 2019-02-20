#include <QTRSensors.h>

#define NUM_SENSORS 5
#define TIMEOUT 2500
#define EMITTER_PIN 2
#define BAUDRATE 9600

QTRSensorsRC qtrrc((unsigned char[]) {3,4,5,6,7},
  NUM_SENSORS, TIMEOUT, EMITTER_PIN);

unsigned int sensorValues[NUM_SENSORS];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(BAUDRATE);
}

void loop() {
  // put your main code here, to run repeatedly:
  qtrrc.read(sensorValues);
  for(int i = 0; i < NUM_SENSORS; i++){
    Serial.print(sensorValues[i]);
    Serial.print(' ');
  }//end for 

  Serial.println(); 
}
