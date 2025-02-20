import numpy as np
import matplotlib.pyplot as plt


### Load Data
data = np.genfromtxt("lk_pressed.csv", delimiter=',')
Y = data[:,1]
X = data[:, 0]


def sliding_window_normalization(signal, L_norm):
    signal = np.array(signal)
    normalized_signal = np.zeros_like(signal)
    
    for t in range(len(signal)):
        if t >= L_norm - 1:
            # the sliding window
            window = signal[t - L_norm + 1:t + 1]
            mean = np.mean(window)
            std = np.std(window)
            
            # Normalize the current value
            normalized_signal[t] = (signal[t] - mean) / std if std != 0 else 0
        else:
            # For the initial values, normalization is skipped or handled differently
            normalized_signal[t] = signal[t]  

    return normalized_signal

Y = sliding_window_normalization(Y, 200)
Y = np.reshape(Y, (len(Y), 1)) 

### Clean up data
running_list = []
running_list_y = []
final_x = []
final_y = []
prev = 0
total = 0
for x, y in zip(X, Y):
    # if the times are the same, then we append the vals to a running list
    if x == prev:
        running_list.append(x)
        running_list_y.append(y[0])

    # if the times are different, then we can break up the 
    # millisecond based on how many sensor readings we got
    else:
        for index, i in enumerate(running_list):
            final_x.append(min(running_list) + (index + 1) / len(running_list))
            final_y.append(running_list_y[index])
        running_list = []
        running_list_y = []
    prev = x

### Plot Data
plt.figure(figsize=(30, 3))
plt.plot(final_x[200:], final_y[200:])
plt.show()