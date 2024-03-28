int signalVals[20] = {};

// no way to get length of the array when passing array between functions
int signalValsSize = 20; 

int rawPrevious = 0;
bool clenched = false;

bool input;
int count = 0;

int button = 13;
int motor_cw = 12;
int motor_ccw = 11;

void setup() {
  Serial.begin(9600);

  pinMode(button, INPUT);
  pinMode(motor_cw, OUTPUT);
  pinMode(motor_ccw, OUTPUT);

  Serial.begin(9600);


  while (!Serial); // wait for serial terminal to open
}

void loop() {
  
  // read the input on analog pin A0
  int rawValue = analogRead(A0); 

  for (int i = 1; i < signalValsSize; i++) {
    signalVals[i - 1] = signalVals[i];
  }
  signalVals[9] = rawPrevious - rawValue;
  
  rawPrevious = rawValue;

  double currentSTD = calculateStandardDeviation(signalVals);
  
  //Serial.print(25);
  //Serial.print(", ");
  //Serial.print(-25);
  //Serial.print(", ");
  Serial.println(currentSTD); 

  /* 
    The determines if there has been a state change
  */

  if (rawValue > 100 && currentSTD > 10) {
    clenched = !clenched;
    motorMoving = true;

    if (clenched == HIGH) {
      digitalWrite(motor_cw, LOW);
      digitalWrite(motor_ccw, LOW);

      count++;

      if (count % 2 == 0) {
        digitalWrite(motor_cw, HIGH);
        digitalWrite(motor_ccw, LOW);
      }
      
      if (count % 2 == 1) {
        digitalWrite(motor_cw, LOW);
        digitalWrite(motor_ccw, HIGH);
      }
      
      delay(1000);
      digitalWrite(motor_cw, LOW);
      digitalWrite(motor_ccw, LOW);
    }

    delay(100);


  }
  delay(50); // to avoid overloading the serial terminal
}

double calculateStandardDeviation(int arr[]) 
{ 
    double sum = 0.0;
    double mean;
    double standardDeviation = 0.0; 
    
    for (int i = 0; i < signalValsSize; ++i) { 
        sum += arr[i]; 
    } 
  
    mean = sum / signalValsSize; 
  
    // squared differences of the datapoint and the mean
    for (int i = 0; i < signalValsSize; ++i) { 
        standardDeviation += pow(arr[i] - mean, 2); 
    } 
  
    // square root of the variance to get the standard deviation 
    return sqrt(standardDeviation / signalValsSize); 
} 





