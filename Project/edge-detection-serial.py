import numpy as np
import datetime
import imageio
import sys
import pymp

imagePath = sys.argv[1]
threads = 4
imageName = imagePath.split("/")[-1]

face = imageio.imread(imagePath)
if len(face.shape) == 3 :
    face = face[:,:,0]
height = face.shape[0]
width = face.shape[1]
pymp.config.nested = True

time_start = datetime.datetime.now()   
padded = pymp.shared.array((height+2,width+2), dtype='uint8')
convx = np.array([[-1, 0, 1], [-2, 0, 2], [-1, 0, 1]])
i=None
j=None
with pymp.Parallel(threads) as p1:
	with pymp.Parallel(threads) as p2: 
		for i in p1.range(0,height):
			for j in p2.range(0,width):
				padded[i+1][j+1]=face[i][j]


res = pymp.shared.array((height,width), dtype='uint16')
i=None
j=None
with pymp.Parallel(threads) as p1:
	with pymp.Parallel(threads) as p2: 
		for i in p1.range(1,height+1):
			for j in p2.range(1,width+1):
				res[i-1, j-1] = (np.sum(np.multiply(padded[i-1:i+2, j-1:j+2], convx)))**2


resy = pymp.shared.array((height,width), dtype='uint16')
i=None
j=None
convy = [[1, 2, 1],
	[0, 0, 0],
	[-1, -2, -1]
	]
with pymp.Parallel(threads) as p1:
  with pymp.Parallel(threads) as p2:
    for i in p1.range(1,height+1):
     for j in p2.range(1,width+1):
       resy[i-1, j-1] = (np.sum(np.multiply(padded[i-1:i+2, j-1:j+2], convy)))**2

res2 = pymp.shared.array((height,width), dtype='uint16')
for i in range(0,height):
	for j in range(0,width):
		res2[i][j] = int((res[i-1][j-1]+resy[i-1][j-1]))
     
time_stop = datetime.datetime.now()

imageio.imwrite("output.png",res2)

print("Shape = ", height, "x", width, "\t", "Time in ms = ",
      (time_stop-time_start).total_seconds())
