#include <Arduino.h>

// Constants
const int window_size = 10;
float normalized_signal[window_size];
int signal_index = 0;
float CALLIBRATED_MEAN = 57.9890350877193;
float CALLIBRATED_STD = 29.85482950349875;

// Logistic Regression Weights (from Python)
float weights[] = { 1.30081864, -0.07603121, 0.06451631, -0.14054752, 1.68810808, 0.63655892, 
                    0.89873033, 0.41659686, -0.58041178, 2.24831185 };

// Read data from Serial and normalize
float readAndNormalizeSignal() {
  
  float signal_value = analogRead(A0);
  float normalized_value = (signal_value - CALLIBRATED_MEAN) / CALLIBRATED_STD;
  return normalized_value;
  
  return 0;
}

// Compute Mean
float computeMean(float data[], int size) {
  float sum = 0;
  for (int i = 0; i < size; i++) {
    sum += data[i];
  }
  return sum / size;
}

// Compute Standard Deviation
float computeStdDev(float data[], int size, float mean) {
  float sum = 0;
  for (int i = 0; i < size; i++) {
    sum += pow(data[i] - mean, 2);
  }
  return sqrt(sum / size);
}

// Compute Skewness
float computeSkewness(float data[], int size, float mean, float stddev) {
  float sum = 0;
  for (int i = 0; i < size; i++) {
    sum += pow((data[i] - mean) / stddev, 3);
  }
  return sum / size;
}

// Compute Kurtosis
float computeKurtosis(float data[], int size, float mean, float stddev) {
  float sum = 0;
  for (int i = 0; i < size; i++) {
    sum += pow((data[i] - mean) / stddev, 4);
  }
  return sum / size;
}

// Compute Std Diff (difference of adjacent values)
float computeStdDiff(float data[], int size) {
  float diff[size - 1];
  for (int i = 0; i < size - 1; i++) {
    diff[i] = data[i + 1] - data[i];
  }
  return computeStdDev(diff, size - 1, 0);
}

// Feature extraction
void featureExtraction(float signal[]) {
  float mean = computeMean(signal, window_size);
  float stddev = computeStdDev(signal, window_size, mean);
  float skewness = computeSkewness(signal, window_size, mean, stddev);
  float kurtosis = computeKurtosis(signal, window_size, mean, stddev);
  float std_diff = computeStdDiff(signal, window_size);

  // Collect features
  float features[] = {mean, signal[window_size - 1], signal[0], signal[window_size - 1] - signal[0], 
                      signal[window_size - 1], signal[window_size - 2], 0, skewness, kurtosis, std_diff};

  // Make prediction
  float prediction = 0.0;
  for (int i = 0; i < 10; i++) {
    prediction += weights[i] * features[i];
  }

  prediction = 1 / (1 + exp(-prediction));

  // Output prediction
  if (prediction < 0.5) {
    Serial.println("Open");
  } else {
    Serial.println("Close");
  }
}

void setup() {
  Serial.begin(9600);
  // Initialize the signal array with zeroes
  for (int i = 0; i < window_size; i++) {
    normalized_signal[i] = 0.0;
  }
}

void loop() {
  // Get the normalized signal value
  float normalized_value = readAndNormalizeSignal();

  // Shift data in the window and add new value
  if (normalized_value != 0) {
    normalized_signal[signal_index] = normalized_value;
    signal_index++;

    if (signal_index == window_size) {
      signal_index = 0;

      // Perform feature extraction and prediction
      featureExtraction(normalized_signal);
    }
  }
}
