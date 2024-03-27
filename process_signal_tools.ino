int signalVals[20] = {};

// no way to get length of the array when passing array between functions
int signalValsSize = 20; 

int rawPrevious = 0;
int index = 0;

void setup() {
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

