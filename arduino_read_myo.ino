void setup()
{
  Serial.begin(9600);
  while (!Serial); // optionally wait for serial terminal to open
}
void loop()
{
  int sensorValue = analogRead(A0); // read the input on analog pin A0
  Serial.println(sensorValue); // print out the value you read
  delay(50); // to avoid overloading the serial terminal
}
