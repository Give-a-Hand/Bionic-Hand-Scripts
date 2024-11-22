import numpy as np
data = [2,4,6,3,2,5]

def percentile20(data):
    return np.percentile(data,20)
    
x = percentile20(data)
print (x)

def skew(data):
    mean = np.mean(data)
    std = np.std(data)
    N = np.size(data)
    top = np.sum((data - mean)**3)
    bottom = (N-1)*(std**3)
    total = top/bottom
    return total
s = skew(data)
print (s)

def kurtosis(data):
    mean = np.mean(data)
    top = np.sum((data - mean)**4)
    bottom = np.sum((data - (mean)**2)**2)
    total = top/bottom
    return total
k = kurtosis(data)
print (k)

def stdfd(data):
    difference = np.diff(data)
    std = np.std(difference)
    return std
d = stdfd(data)
print (d)