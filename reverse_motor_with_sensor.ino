// Reverses the motor direction if sensor data is above a certain threshold.


// motor pins
int enablePin = 11;
int in1Pin = 10;
int in2Pin = 9;
bool reverse = false;

//sensor pin
int myoPin = A0;


void setup() {
  Serial.begin(9600);

  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(enablePin, OUTPUT);
}

void loop() {
  int sensorValue = analogRead(myoPin); // read sensor input data
  int thresh; // set threshold for sensor data that is considered a "closed hand"

  if(sensorValue > thresh){
    reverse = true; // reverse motor direction when sensor data is above threshold
  }
  else{
    reverse = false; // original motor direction when sensor data is below threshold
  }
  
  // set motor speed
  int speed = 150;
  analogWrite(enablePin, speed);

  // run motor in appropriate direction
  digitalWrite(in1Pin, reverse);
  digitalWrite(in2Pin, !reverse);
}
