/*
* File com.cpp
* Author: Lu Cheng
* for serial communicating
* Notes:
* (1)After remap the Serial1, should not use /dev/ttyS0 but /dev/ttyAMA0 instead. https://openenergymonitor.org/emon/node/12311
* (2)Use "gpio readall" the check the state of each pin
* (3)both of pin 15 and 16 should work on "ALT0" mode, "gpio mode 15 ALT0"
* (4)need sudo to execute, when in the pragramm which you need create documents, you can not open them with sudo created
* 
*
*/

#include <thread>
#include <ctime>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat)
#include <opencv2/videoio/videoio.hpp>  // Video write
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include "../inc/com.hpp"


using namespace cv;
using namespace std;

const char *cmd1 = "gpio mode 15 ALT0";
const char *cmd2 = "gpio mode 16 ALT0";

int counter_send = 0;
int counter_receive = 0;

std::string exec(const char* cmd) {
    char buffer[128];
    std::string result = "";
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != NULL)
            result += buffer;
    }
    return result;
}

int SerialInit(int baudrate)
{
	
	int fd;
	
	if ((fd = serialOpen ("/dev/ttyAMA0", baudrate)) < 0)
  	{
    		cout<<"Unable to open serial device!"<<endl;
    		return -1 ;
  	}
	return fd;
}


void sendSerialString(int device, std:: string date)
{
	int size = date.length();
	for(int i = 0; i < size; i++)
	{
		serialPutchar (device, date.at(i)) ;
	}
}


void sendsteeringspeed(int device, float speed, float steering)
{
	std::string Spd = std::to_string(speed);
	std::string Str = std::to_string(steering);
	std::string ultrosonic_debug = "15.67";
	
	Spd.erase(Spd.end()-4,Spd.end());
	Str.erase(Str.end()-4,Str.end());
	
	serialPutchar (device, '|') ;
	serialPutchar (device, '|') ;
	serialPutchar (device, '|') ;
	serialPutchar (device, '|') ;
	
	sendSerialString(device,Spd);
	serialPutchar (device, '\n') ;
	sendSerialString(device,Str);
	serialPutchar (device, '\n') ;
	if(DATE_TRANSFER>2)
	{
		sendSerialString(device,ultrosonic_debug);
		serialPutchar (device, '\n') ;
	}
}

int getdatefromstr(std::string str, int numberofdate,float *value)
{
	
	int j;
	int length = str.length();
	for(j=0;j<length;j++)
	{
		if(str.at(j)=='|')
		break;
	}
	std::string::size_type sz;
	
	j+=4;
	if(j>=length)
	{
		cout<<"the startsign of dates are incorrect"<<endl;
		return -1;
	}
	
	std::string startsign = str.substr(j-4,j);
	if(startsign.compare("||||")!=0)
	{
		cout<<"the startsign of dates are incorrect"<<endl;
		return -1;
	}
	
	std::string str_value[numberofdate];
	for(int i = 0; i< numberofdate;i++)
	{
		
		while(str.at(j)!='\n')
		{
			str_value[i].append(1u,str.at(j));
			j++;
			if(j>=length) break;
		}
		j++;
		//cout<<str_value[i]<<endl;
		value[i] = std::stof(str_value[i],&sz);
		if(j>=length) break;
	}
	return 1;
}

void SerialReceive(int device, float *receivalue)
{
	bool isDatareceived = false;
	std::string Str_receive;
	char result;
	if(serialDataAvail(device)) 
	{
		isDatareceived = true;
		Str_receive = "";
	}
	     
		 while(serialDataAvail(device))
    	 {
      		result = (char) serialGetchar(device);
      		Str_receive.append(1u, result);
      		cout<<result;
      		
    	 }
    	 if(isDatareceived)
    	 {
			 getdatefromstr(Str_receive, DATE_RECEIVE,receivalue);
			 cout<<"the received steering is: "<< receivalue[1]<<" the received speed is: "<<receivalue[2]<<" the received ultrosonic is: "<<receivalue[0]<<endl;
			 isDatareceived = false;
		 }
}
