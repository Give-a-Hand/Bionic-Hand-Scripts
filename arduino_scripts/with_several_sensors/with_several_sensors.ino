#include <arduinoFFT.h>

#define NUM_SENSORS 8
#define WINDOW_SIZE 128
#define SAMPLE_FREQUENCY 2000

int sensorPorts[NUM_SENSORS] = {A0, A1, A2, A3, A4, A5, A6, A7};

unsigned long startTime;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  startTime = micros();

  for(int i = 0; i < NUM_SENSORS; i++) {

    Serial.print("Sensor");
    Serial.print(i + '1');
    Serial.print(",");
  }
  
  Serial.print("Time");

}

void loop() {
  

  for(int i = 0; i < NUM_SENSORS; i++) {

    Serial.print(analogRead(sensorPorts[i]));
    Serial.print(",");

  }

  Serial.print(micros() - startTime);

}
