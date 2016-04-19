import numpy as np
import cv2
import sys

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

while(True):
	# Capture frame-by-frame
	ret, frame = cap.read()
	frame=cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
	# Our operations on the frame come here
	# gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
	# print(frame.size.width)
	# Display the resulting frame
	if ret:
		cv2.imshow('frame',frame)
		if cv2.waitKey(1) & 0xFF == 27: # Escape
			break
	else:
		print("no frame")

# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()