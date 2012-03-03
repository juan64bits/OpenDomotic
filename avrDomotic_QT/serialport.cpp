#include "serialport.h"

SerialPort::SerialPort()
{
	portfd=0;	
}

int SerialPort::OpenPort(char *portfile, speed_t baudRate)
{
	int n;
	/*Abriendo el puerto serial*/
		portfd = open(portfile, O_RDWR|O_NDELAY|O_NOCTTY);
		
		if(portfd >= 0) 
		{
			// Cancelamos O_NDELAY.
			n = fcntl(portfd, F_GETFL, 0);
			fcntl(portfd, F_SETFL, n & ~O_NDELAY);
		}
		else if(portfd==-1)
		{
			printf ("Error opening the serial device: %s\r\n",portfile);
			perror("OPEN");
			return 0;
		}
		else
		{
			printf ("The serial device has been opened, fd:%#x \n",portfd);
			fcntl(portfd, F_SETFL, FNDELAY);
		}

		/*Configurando el puerto serial*/	
		struct termios tty;
		/*	Establaciendo la velocidad I/O 				*/
		cfsetispeed(&tty, baudRate);
		cfsetospeed(&tty, baudRate);
		/*  Banderas de control 						*/
		tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
		tty.c_cflag |= CLOCAL | CREAD;
		tty.c_cflag &= ~CRTSCTS;  //Sin control por hw.
		tty.c_cflag &= ~(PARENB | PARODD); 
		tty.c_cflag &= ~CSTOPB;
		/*	Banderas de entrada: RAW y non-echo			*/	
		tty.c_iflag =  IGNBRK;
		/*	Banderas de salida y locales				*/
		tty.c_lflag = 0;
		tty.c_oflag = 0;
		/* Time-out y mínima cantidad de caracteres   	*/
		tty.c_cc[VMIN] = 1;
        tty.c_cc[VTIME] = 2;
		
		//tty.c_iflag |= IXON | IXOFF;
        tty.c_iflag &= ~(IXON|IXOFF|IXANY);
			
		if (tcsetattr(portfd, TCSANOW, &tty) < 0) 
		{
			printf ("Call to tcsettty failed");
		    return 0;
		}
		
		return 1;
}

void SerialPort::ClosePort()
{
	close(portfd);
	portfd = 0;
}

int SerialPort::CheckIO(int fd1, int fd2, int tmout, char *buf, int *buflen)
{
	int n = 0, i, io_pending = 0;
	struct timeval tv;
	fd_set fds;

	tv.tv_sec = tmout / 1000;
	tv.tv_usec = (tmout % 1000) * 1000L;

	i = fd1;
	if (fd2 > fd1)
		i = fd2;

		FD_ZERO(&fds);
		if (fd1 >= 0)
			FD_SET(fd1, &fds); else fd1 = 0;
		if (fd2 >= 0)
			FD_SET(fd2, &fds); else fd2 = 0;

	if (fd2 == 0 && io_pending)
		n = 2;
	else if (select(i+1, &fds, NULL, NULL, &tv) > 0)
		n = 1 * (FD_ISSET(fd1, &fds) > 0) + 2 * (FD_ISSET(fd2, &fds) > 0);

  	/* Si hay datos son puestos en el bufer. */
	if (buf) {
		i = 0;
		if ((n & 1) == 1) 
			i = read(fd1, buf, 63);
	
		buf[i > 0 ? i : 0] = 0;
		if (buflen)
			*buflen = i;
	}

  return n;
}

int SerialPort::ReadPort()
{ 
	return CheckIO(portfd, 0, 0, buf, &bytesRead);
}

int SerialPort::WritePort(char *data,  int size)
{
	return write(portfd, data, size);
}

void SerialPort::Flush()
{
    do
    {
     CheckIO(portfd, 0, 0, buf, &bytesRead);
    }while(bytesRead);
}

