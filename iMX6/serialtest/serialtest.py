import serial
import time
import string
import binascii

# configure the serial connections (the parameters differs on the device you are connecting to)
ser = serial.Serial(
		port="/dev/ttymxc1",
		baudrate=115200,
		parity=serial.PARITY_NONE,
		stopbits=serial.STOPBITS_ONE,
		bytesize=serial.EIGHTBITS
)

def readlineLF(port):
		rv = ""
		while True:
				ch = port.read()
				rv += ch
				if ch=='\l' or ch=='':
						return rv

# ser.open()
ser.isOpen()

print ('Enter your commands below.\r\nInsert "exit" to leave the application.')

count=9
input=1

speed = 50
while 1 :
		# get keyboard input
		input = "\x01"+chr(speed+100)# raw_input(">> ")
				# input = input(">> ")
		if input == 'exit':
				ser.close()
				exit()
		else:
				# send the character to the device
				# (note that I append a \r\n carriage return and line feed to the characters - this is requested by my device)
				#ser.write(input + '\xff')
				out = ''
				# let's wait 0.2 seconds before reading output (let's give device time to answer)
				time.sleep(0.001)
				out = ser.read(1)
	#data=b''
	#lastdata= b''
				#while True:
				#	lastdata = ser.read(1)
				#	if lastdata == b'\n':
				#		break
				#	data = data+lastdata
	#out=data

		#if int(out)<100:
		#	speed = 0
		#else:
		#	speed = 00
		
				#while ser.inWaiting() > 0:
				#		out += ser.read(1)

	if out != b'':
					#print(out)
		if out == b'\xff' :
						print("Linebreak!")
					else:
						print(">>" + binascii.hexlify(out).decode('ascii','ignore')+ "	("+out.decode('ascii','ignore')+")")
					
