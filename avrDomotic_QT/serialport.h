#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>

class SerialPort
{
public:
    SerialPort();
	~SerialPort(){};

    int OpenPort(char *portfile, speed_t baudRate);
    void ClosePort();
    int ReadPort();
    int WritePort(char *data,  int size);
    char* GetBufer(){return buf;};
    int GetBytesRead(){return bytesRead;};    
    int GetPortFD(){return portfd;};
    void Flush();
private:
	int CheckIO(int fd1, int fd2, int tmout, char *buf, int *buflen);
	
	char *portfile;
	int portfd;
	char buf[64];
	int bytesRead;
	speed_t baudRate;
};

#endif
