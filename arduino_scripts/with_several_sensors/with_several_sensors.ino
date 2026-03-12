#define NUM_SENSORS 8
#define WINDOW_SIZE 128
#define SAMPLE_FREQUENCY 2000

unsigned long startTime;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  startTime = micros();

  Serial.println(micros() - startTime);

}

void loop() {
  
  Serial.print(analogRead(A0));
  Serial.print(",");

  Serial.println(micros() - startTime);

  delay(10);

}
