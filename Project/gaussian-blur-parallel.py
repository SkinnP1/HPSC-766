import numpy as np
import pymp
import datetime
import imageio 
import sys


imagePath = sys.argv[1]
threads = int(sys.argv[2])
imageName = imagePath.split("/")[-1]

face = imageio.imread(imagePath)
if len(face.shape) == 3 :
    face = face[:,:,0]
height = face.shape[0]
width = face.shape[1]


time_start = datetime.datetime.now()

convx = np.array([[1/16, 2/16, 1/16],
	            [2/16, 4/16, 2/16],
	            [1/16, 2/16, 1/16]])

padded = pymp.shared.array((height+2,width+2), dtype='uint8')
pymp.config.nested = True
i=None
j=None
with pymp.Parallel(threads) as p1:
	with pymp.Parallel(threads) as p2: 
		for i in p1.range(0,height):
			for j in p2.range(0,width):
				padded[i+1, j+1]= face[i, j]

res = pymp.shared.array((height,width), dtype='uint8')
with pymp.Parallel(threads) as p1:
  with pymp.Parallel(threads) as p2: 
    for i in p1.range(1,height+1):
      for j in p2.range(1,width+1):
        res[i-1][j-1] = (np.sum(np.multiply(padded[i-1:i+2, j-1:j+2], convx)))
        
time_stop = datetime.datetime.now()

imageio.imwrite("output.png",res)

print("Shape = ", height,"x",width,"\t","Threads = ",threads,"\t","Time in ms = ",(time_stop-time_start).total_seconds())