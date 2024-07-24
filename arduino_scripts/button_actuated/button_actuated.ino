// Initializing the array for first deriv of sliding window of 20 datapoints
int count = 0;

int motorSpeed = 255;  

const int AIN1 = 13;
const int AIN2 = 4;
const int PWMA = 11;
const int BUTTON = 8;

void setup() {
  pinMode(12, OUTPUT); // motor cw
  pinMode(11, OUTPUT); // motor ccw

  Serial.begin(9600);

  // wait for serial terminal to open
  while (!Serial); 
}

void loop() {

  pinMode(BUTTON, INPUT); // button input

  if (digitalRead(BUTTON) == HIGH) {
    //Serial.println(pressed);
    //Serial.print("count: ");
    //Serial.println(count);

    if (count % 2 == 1) {
      Serial.println("CLENCH");
      spinMotor(motorSpeed);
      //Serial.print("loop 1 - count: ");
      //Serial.println(count);
    }
    
    if (count % 2 == 0) {
      Serial.println("UNCLENCH");
      spinMotor(-motorSpeed);
      //Serial.print("loop 2 - count: ");
      //Serial.println(count);
    }

    count++;
    //Serial.println("restart loops");

    pinMode(BUTTON, OUTPUT); // button output
    digitalWrite(BUTTON, LOW); // ground the button


  }

  delay(100);
  
}


/********************************************************************************/
void spinMotor(int motorSpeed)                       //function for driving the right motor
{
  if (motorSpeed > 0)                                 //if the motor should drive forward (positive speed)
  {
    digitalWrite(AIN1, HIGH);                         //set pin 1 to high
    digitalWrite(AIN2, LOW);                          //set pin 2 to low
    delay(1500);
    digitalWrite(AIN1, LOW);                          //set pin 1 to low
    digitalWrite(AIN2, LOW);                          //set pin 2 to low
    
  }
  else if (motorSpeed < 0)                            //if the motor should drive backward (negative speed)
  {
    digitalWrite(AIN1, LOW);                          //set pin 1 to low
    digitalWrite(AIN2, HIGH);                         //set pin 2 to high
    delay(500);
    digitalWrite(AIN1, LOW);                          //set pin 1 to low
    digitalWrite(AIN2, LOW);                          //set pin 2 to low
  }
  
  delay(100);
  analogWrite(PWMA, abs(motorSpeed));                 //now that the motor direction is set, drive it at the entered speed
}