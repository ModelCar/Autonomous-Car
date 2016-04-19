import cv2

import numpy as np

cap = cv2.VideoCapture(0)
kernel = np.ones((3,3),np.uint8)

while(1):

	# Take each frame
	_, frame = cap.read()

	# Convert BGR to HSV
	hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

	# define range of blue color in HSV
	lower = np.array([00,80,150])
	upper = np.array([255,255,255])

	# Threshold the HSV image to get only string colors
	mask = cv2.inRange(hsv, lower, upper)
	
	#mask = cv2.morphologyEx(mask,2,kernel)

	# Bitwise-AND mask and original image
	res = cv2.bitwise_and(frame,frame, mask= mask)

	cv2.imshow('frame',frame)
	cv2.imshow('mask',mask)
	cv2.imshow('res',res)
	k = cv2.waitKey(5) & 0xFF
	if k == 27:
		break

cv2.destroyAllWindows()