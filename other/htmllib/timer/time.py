import numpy as np
#import matplotlib.pyplot as animation
from matplotlib import pyplot as plt
j = np.arange(0,1440*np.pi,np.pi/30)
x1 = 224 + 112*np.cos(j/720) + 56*np.sin(j/60)+56*np.sin(j)
y1 = 224 + 112*np.sin(j/720) + 56*np.cos(j/60)+56*np.cos(j)
x2 = 224 + 112*np.cos(j/720) + 112*np.sin(j/60)
y2 = 224 + 112*np.sin(j/720) + 112*np.cos(j/60)
x3 = 224 + 224*np.cos(j/720)
y3 = 224 + 224*np.sin(j/720)

fig = plt.figure()
axes = fig.add_subplot(111)
axes = plt.plot(x1,y1)
axes = plt.plot(x2,y2)
axes = plt.plot(x3,y3)
plt. show()