#!/usr/bin/python3

import serial
import socketserver
import threading
import binascii
import sys

# Note for newcomers to python: code which belongs to a loop or an if statement
#	is marked by indentation, so be carefull with editors wo try to correct your indentation and don't
#	know about python ;)



# configure the serial connections (the parameters differ on the device you are connecting to)
ser = serial.Serial(
		port="/dev/ttymxc1",
		baudrate=115200, # increased from 9600
		parity=serial.PARITY_NONE,
		stopbits=serial.STOPBITS_ONE,
		bytesize=serial.EIGHTBITS
)

connection = 0 # global var for accessing connection 
# we need it to send fromt he main thread

# This will handle all incoming data over the network
class MyTCPHandler(socketserver.StreamRequestHandler):
	def handle(self):
		print("Connected")
		global connection 
		connection = self
		while True:
			self.data = b''
			while True:
				lastdata = self.rfile.read(1)
				if lastdata == b'\xff':
					break
				self.data = self.data+lastdata
				if not self.data:
					print("Disconnected")
					connection = None
					self.finish()
					return
			# print the received data for debugging as hex and as string, remove to increase speed dramatically	
			print('Net: ' + binascii.hexlify(self.data).decode()+"= "+self.data.decode('ascii','ignore'))
			# write padding byte (value not important) to give FPGA time to react
			# +message + messagebreakinfo byte 255
			ser.write(b'\0x254'+self.data + b'\xff')

# a try catch so we can cleanly exit the server by pressing CTRL+C at the console
try:
	# create a thread to listen for data on port 9999
	server = socketserver.TCPServer(('0.0.0.0', 9999), MyTCPHandler)
	t = threading.Thread(target=server.serve_forever)
	t.setDaemon(True)
	t.start()

	# ser.open() nope already opened by specifying the structure above, causes error
	ser.isOpen()

	print ('Serving on Port 9999, press CTRL+C to exit.')
	# main loop
	while True :
		data = b''
		while True:
					lastdata = ser.read(1)
					if lastdata == b'\xff':
						break
					data = data+lastdata

		if data != '':
			print("232: " + binascii.hexlify(data).decode('ascii','ignore')+ "= "+data.decode('ascii','ignore'))
		
		if connection:
			connection.request.sendall(data+b'\xff')
		else:
			print("No connection, sorry")
					
	
except KeyboardInterrupt as e:
	# User pressed CTRL+C, exit gracefully
	print("\nKeyboard interrupt")
	server.server_close()
	print("quitting")
	sys.exit()
	

