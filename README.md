# Raspberry
Advisor: Dr. Yongqiang Wang, 
Implemented by: Muaz Ahmad.
Privacy Preserving Average Concensus Algorithm in C++ 
The code was developed for the XBEE S1 module and runs on the Raspberry PI.
Simply connect the Xbee to the Pi using a USB adapter, or manually connect the UART pins of the XBEE to the UART GPIO of the PI.
To manage the number of nodes in the communication, you can increase the value of NEIGHBORS in communication.h #define NEIGHBORS 2 to the desired number and then you need to assign addresses of the neighbors in test01.cpp 
 a.address = 1;
 b.address = 2;
 c.address = 3; //These are addresses of the neighboring XBEE modules you wish to converge with
Also make sure you are using the righ serial port address in communication.h 
if((fd=serialOpen("/dev/ttyACM0",9600)) < 0) //dev/ttyACM0 is commonly assigned to serial devices on the raspberry pi.
Once done, simply download the Average Concensus code on to the PI, compile and run on all nodes
