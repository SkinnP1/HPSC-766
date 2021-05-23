### Running the scripts

The scripts developed for the project are edge-detection-parallel.py, edge-detection-serial.py, gaussian-blur-serial.py, gaussian-blur-parallel.py.

** Description and executing file : edge-detection-parallel.py

This script uses Sobel Edge detection algorithm to detect edges in the image parallely. 
The script is executed using command line : python3 edge-detection-parallel.py #image #threads
#image refers to the path of the image
#threads refers to number of threads used for computation. 

After running the above script, the time will be printed in seconds. 

The image generated is stored in file "output.png". 

**Description and executing file : edge-detection-serial.py

This script uses Sobel Edge detection algorithm to detect edges in the image serially. 
The script is executed using command line : python3 edge-detection-parallel.py #image
#image refers to the path of the image

After running the above script, the time will be printed in seconds. 

The image generated is stored in file "output.png". 

**Description and executing file : gaussian-blur-parallel.py

This script uses Gaussian Blur to blur the image parallely. 
The script is executed using command line : python3 edge-detection-parallel.py #image #threads
#image refers to the path of the image
#threads refers to number of threads used for computation. 

After running the above script, the time will be printed in seconds. 

The image generated is stored in file "output.png". 

**Description and executing file : edge-detection-serial.py

This script uses Gaussian Blur to blur the image serially. 
The script is executed using command line : python3 edge-detection-parallel.py #image
#image refers to the path of the image

After running the above script, the time will be printed in seconds. 

The image generated is stored in file "output.png". 

### Install Libraries 
pymp
imageio
Pillow 

### Configuration of the machine on which observations are made

Memory : 15.5 GiB
Processor : Intel® CoreTM i7-6700HQ CPU @ 2.60GHz × 8
GPU : GeForce GTX 960M (4GiB)
Number of Cores per CPU : 4