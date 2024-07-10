// Initializing the array for first deriv of sliding window of 20 datapoints
int signalVals[20] = {};
int signalValsSize = 20; 

int rawPrevious = 0;
int count = 0;

int motorSpeed = 255;  

const int AIN1 = 13;
const int AIN2 = 4;
const int PWMA = 11;

void setup() {
  pinMode(12, OUTPUT); // motor cw
  pinMode(11, OUTPUT); // motor ccw
  Serial.begin(9600);

  // wait for serial terminal to open
  while (!Serial); 
}

void loop() {
  // read the input on analog pin A0
  int rawValue = analogRead(A0); 
  delay(100);
  if (rawValue > 500) {
    rawValue -= 500;
  }

  // Updating sliding window with new reading
  for (int i = 1; i < signalValsSize; i++) {
    signalVals[i - 1] = signalVals[i];
  }
  signalVals[19] = rawPrevious - rawValue;
  rawPrevious = rawValue;

  Serial.print(rawValue);
  //Serial.print(", ");
  //Serial.println(calculateStandardDeviation(signalVals));


  // Check raw reading and the std dev of the sliding window to 
  // see if the hand is clenched and motors need to be moved
  if (rawValue > 100 && calculateStandardDeviation(signalVals) > 10) {
    count++;

    if (count % 2 == 0) {
      Serial.print("CLENCH");
      spinMotor(motorSpeed);
    }
    
    if (count % 2 == 1) {
      Serial.print("UNCLENCH");
      spinMotor(-motorSpeed);
    }
    
    delay(1000);
  }

  delay(100);
}


/********************************************************************************/
// Function to calculate standard deviation
double calculateStandardDeviation(int arr[]) 
{ 
    double sum = 0.0;
    double mean;
    double standardDeviation = 0.0; 
    
    for (int i = 0; i < signalValsSize; ++i) { 
        sum += arr[i]; 
    } 
  
    mean = sum / signalValsSize; 
    Serial.print(", ");
    Serial.println(mean);
  
    // squared differences of the datapoint and the mean
    for (int i = 0; i < signalValsSize; ++i) { 
        standardDeviation += pow(arr[i] - mean, 2); 
    } 
  
    // square root of the variance to get the standard deviation 
    return sqrt(standardDeviation / signalValsSize); 
} 


/********************************************************************************/
void spinMotor(int motorSpeed)                       //function for driving the right motor
{
  if (motorSpeed > 0)                                 //if the motor should drive forward (positive speed)
  {
    digitalWrite(AIN1, HIGH);                         //set pin 1 to high
    digitalWrite(AIN2, LOW);                          //set pin 2 to low
    delay(1000);
    digitalWrite(AIN1, LOW);                          //set pin 1 to low
    digitalWrite(AIN2, LOW);                          //set pin 2 to low
    
  }
  else if (motorSpeed < 0)                            //if the motor should drive backward (negative speed)
  {
    digitalWrite(AIN1, LOW);                          //set pin 1 to low
    digitalWrite(AIN2, HIGH);                         //set pin 2 to high
    delay(1000);
    digitalWrite(AIN1, LOW);                          //set pin 1 to low
    digitalWrite(AIN2, LOW);                          //set pin 2 to low
  }
  
  analogWrite(PWMA, abs(motorSpeed));                 //now that the motor direction is set, drive it at the entered speed
}