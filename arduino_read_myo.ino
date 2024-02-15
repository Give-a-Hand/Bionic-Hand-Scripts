/** void setup()
{
  Serial.begin(115200);
  while (!Serial); // optionally wait for serial terminal to open
  Serial.println("MyoWare Example_01_analogRead_SINGLE");
}
void loop()
{
  int sensorValue = analogRead(A0); // read the input on analog pin A0
  Serial.println(sensorValue); // print out the value you read
  delay(50); // to avoid overloading the serial terminal
}
**/