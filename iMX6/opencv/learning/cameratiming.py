import cv2
from time import sleep

print("optimized cv="+str(cv2.useOptimized()))

def printTimeSinceLastMeasurement(name):
	global start_time
	print(name+" took "+str((cv2.getTickCount() - start_time)/cv2.getTickFrequency()))
	start_time=cv2.getTickCount()

### start init code
cap = cv2.VideoCapture(0)
if not cap.isOpened():
	print("No capture device")
	sys.exit()
# resolution xy
print(str(cap.get(3))+"x"+str(cap.get(4)))
ret = cap.set(3,320)
ret = cap.set(4,240)
print(" now set to "+str(cap.get(3))+"x"+str(cap.get(4)))
# framerate (will stay at 30)
ret = cap.set(5,60)
print(str(cap.get(5))+" fps")
#read one frame to init
_, img = cap.read()
### end Init code
start_time = cv2.getTickCount()
### ##


count=0
while count < 50:
	_, img = cap.read()
	count=count+1
	printTimeSinceLastMeasurement("frame "+str(count))
	# cv2.imwrite('ramdrive/Roadgray.png',img)
	# printTimeSinceLastMeasurement("sv frame")





sleep(1)
printTimeSinceLastMeasurement("Test 1 sec")
