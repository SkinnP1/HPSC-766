import numpy as np
import datetime
import imageio 
import sys


imagePath = sys.argv[1]
imageName = imagePath.split("/")[-1]

face = imageio.imread(imagePath)
if len(face.shape) == 3 :
    face = face[:,:,0]
height = face.shape[0]
width = face.shape[1]

convx = np.array([[1/16, 2/16, 1/16],
	            [2/16, 4/16, 2/16],
	            [1/16, 2/16, 1/16]])

time_start = datetime.datetime.now()

padded = np.zeros((height+2,width+2))
for i in range(0,height):
	for j in range(0,width):
		padded[i+1][j+1]=face[i][j]

res = np.zeros((height,width), dtype='uint8')
i=None
j=None

for i in range(1, height+1):
	for j in range(1, width+1):
		res[i-1][j-1] = (np.sum(np.multiply(padded[i-1:i+2, j-1:j+2], convx)))
        
time_stop = datetime.datetime.now()

imageio.imwrite("output.png",res)


print("Shape = ", height,"x",width,"\t","Time in ms = ",(time_stop-time_start).total_seconds())