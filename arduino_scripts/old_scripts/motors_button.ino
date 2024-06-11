bool input;
int count = 0;

int button = 13;
int motor_cw = 12;
int motor_ccw = 11;

/*
void printPins() {
  Serial.print("")
}
*/

void setup() {
  // put your setup code here, to run once:

  pinMode(button, INPUT);
  pinMode(motor_cw, OUTPUT);
  pinMode(motor_ccw, OUTPUT);

  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

  input = digitalRead(button);
  Serial.println(input);

  if (input == HIGH) {

    Serial.println("Button Pressed!");

    digitalWrite(motor_cw, LOW);
    digitalWrite(motor_ccw, LOW);

    count++;

    if (count % 2 == 0) {
      Serial.println("Spinning CW");
      digitalWrite(motor_cw, HIGH);
      digitalWrite(motor_ccw, LOW);
      Serial.print("motor_cw: ");
      Serial.print(digitalRead(motor_cw));
      Serial.print(", motor_ccw: ");
      Serial.println(digitalRead(motor_ccw));
    }
    
    if (count % 2 == 1) {
      Serial.println("Spinning CCW");
      digitalWrite(motor_cw, LOW);
      digitalWrite(motor_ccw, HIGH);
      Serial.print("motor_cw: ");
      Serial.print(digitalRead(motor_cw));
      Serial.print(", motor_ccw: ");
      Serial.println(digitalRead(motor_ccw));
    }
    
    delay(1000);
    digitalWrite(motor_cw, LOW);
    digitalWrite(motor_ccw, LOW);
  }

  delay(100);

}
