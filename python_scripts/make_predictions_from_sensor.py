from collections import deque
import numpy as np
import serial
import time
ser = serial.Serial("com3")

window_size = 10
normalized_signal = deque(maxlen=window_size)

CALLIBRATED_MEAN = 57.9890350877193
CALLIBRATED_STD = 29.85482950349875

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
    weights =  [ 1.30081864, -0.07603121,  0.06451631, -0.14054752,  1.68810808,  0.63655892, 0.89873033,  0.41659686, -0.58041178,  2.24831185]
    prediction =.9908183563050299 
    
    for w, f in zip(weights, features):
        prediction += (w * f)
    
    return  1 / (1 + np.exp(-prediction))


callibrating_complete = False
callibration_list = []
while True:
    # Read the line from arduino
    s = ser.readline().decode("utf-8", errors='ignore')
    if s != "":
        try:
            signal_value = float(str(s).replace("\n", ""))
        except Exception:
            print("Throw zero")
            signal_value = 0
        

        if not callibrating_complete:
            if len(callibration_list) < 750:
                print("Please keep your hand open")
                callibration_list.append(signal_value)
            elif 1500 >= len(callibration_list) >= 750:
                print("Please keep your hand closed")
                callibration_list.append(signal_value)
            else:
                print("FINISHED CALLIBRATION")
                callibrating_complete = True
                CALLIBRATED_MEAN = np.mean(callibration_list)
                CALLIBRATED_STD = np.std(callibration_list)

        else:
            normalized_value = (signal_value - CALLIBRATED_MEAN) / CALLIBRATED_STD

            # Need to make sure we have enough data to make a prediction (the window size)
            if len(normalized_signal) == window_size:
                normalized_signal.pop()
                normalized_signal.rotate(-1)
                normalized_signal.append(normalized_value)
                
                # Make prediction
                features = feature_extraction(normalized_signal)
                prediction = model_predict(features)
                if prediction < .5:
                    print(f"Open \t\t ({prediction})")
                else:
                    print(f"Close \t\t ({prediction})")

                # Clear the queue (we only want to make predictions every so often)
                normalized_signal.clear()
            
            elif normalized_value:
                normalized_signal.append(normalized_value)

    else:
        print("delay")
