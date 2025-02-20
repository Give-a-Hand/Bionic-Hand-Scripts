import pandas as pd
import matplotlib.pyplot as plt


# Read CSV file into pandas DataFrame
csv_file = "..."
df = pd.read_csv(csv_file)

df = df.groupby('Time').mean()

START_TIME = 1729122960

df.plot()
plt.xlim(START_TIME, START_TIME + 40)
plt.show()