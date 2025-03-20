from collections import deque
import numpy as np
import serial
import time
ser = serial.Serial("com3")

window_size = 10
raw_signal = deque(maxlen=150)
normalized_signal = deque(maxlen=window_size)

# Extract features from window size
def feature_extraction(arr):
    arr = np.array(list(arr))
    mean = np.mean(arr)
    arr_min = np.min(arr)
    arr_max = np.max(arr)
    percentile = np.percentile(arr, 20)
    std = np.std(arr)

    features = []   
    features.append(mean)                 # Mean
    features.append(arr_max)              # Max
    features.append(arr_min)              # Min
    features.append(max(arr) - min(arr))  # Range
    features.append(arr[-1])              # Last Value
    features.append(arr[-2])              # Second to last value
    features.append(percentile)           # Twentieth percentile

    ### skew
    top = np.sum((arr - mean)**3)
    bottom = (np.size(arr)-1)*(std**3)
    features.append(top/(bottom + 0.0000001))

    #### kurt
    top = np.sum((arr - mean)**4)
    bottom = np.sum((arr - (mean)**2)**2)
    features.append(top/(bottom + 0.00000001))

    ###stdiff
    difference = np.diff(arr)
    std = np.std(difference)
    features.append(std)

    return features

# Make a prediction (1 is open, 0 is closed)
def model_predict(features):
    # Weights were pulled from logistic regression model
    weights = [1.1168033,  -0.39353045, -0.51548955,  0.1219591,   1.58287895,  0.50244405, 1.61716643,  0.5224903,  -0.69309384,  3.79517261]
    prediction = 0.3398020130182364
    
    for w, f in zip(weights, features):
        prediction += (w * f)
    
    return  1 / (1 + np.exp(-prediction))

### TODO: Figure out if raw signal queue also needs to be cleared after a prediction is made
###       There might be a significant gap in the data collected in the raw signal because of the 
###       time it takes to make a prediction


while True:
    # Read the line from arduino
    s = ser.readline().decode("utf-8", errors='ignore')
    if s != "":
        try:
            signal_value = float(str(s).replace("\n", ""))
        except Exception:
            print("Throw zero")
            signal_value = 0
        normalized_value = None

        # Need to make sure we have enough data to normalize
        if len(raw_signal) == raw_signal.maxlen:
            # Generate normalized value from raw signals
            normalized_value = (signal_value - np.mean(list(raw_signal))) / np.std(list(raw_signal)) if np.std(list(raw_signal)) != 0 else 0
            
            # Add new value to circular queue
            raw_signal.pop()
            raw_signal.rotate(-1)
            raw_signal.append(signal_value)
        else:
            raw_signal.append(signal_value)
        
        if normalized_value:
            # Need to make sure we have enough data to make a prediction (the window size)
            if len(normalized_signal) == window_size:
                normalized_signal.pop()
                normalized_signal.rotate(-1)
                normalized_signal.append(normalized_value)
                
                # Make prediction
                features = feature_extraction(normalized_signal)
                prediction = model_predict(features)
                print(prediction)
                if prediction < .5:
                    print("Open")
                else:
                    print("Close")

                # Clear the queue (we only want to make predictions every so often)
                normalized_signal.clear()
                raw_signal.clear()
            
            elif normalized_value:
                normalized_signal.append(normalized_value)

