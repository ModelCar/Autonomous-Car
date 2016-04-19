// THIS IS BROKEN

#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

/*
 * 'open_port()' - Open serial port 1.
 *
 * Returns the file descriptor on success or -1 on error.
 */
static int s_fd; /* File descriptor for the serial port */

int
sopen_port(void)
{
  struct termios options;
  
  
  s_fd = open("/dev/ttymxc1", O_RDWR | O_NOCTTY | O_NDELAY);
  if (s_fd == -1)
  {
    /*
     * Could not open the port.
     */
    
    perror("open_port: Unable to open /dev/ttyf1 - ");
  }
  else
    fcntl(s_fd, F_SETFL, 0);
  
  
  
  /*
   * Get the current options for the port...
   */
  
  tcgetattr(s_fd, &options);
  
  /*
   * Set the baud rates to 19200...
   */
  
  cfsetispeed(&options, B115200);
  cfsetospeed(&options, B115200);
  
  /*
   * Enable the receiver and set local mode...
   */
  
  options.c_cflag |= (CLOCAL | CREAD);
  
  options.c_cflag &= ~CSIZE; /* Mask the character size bits */
  options.c_cflag |= CS8;    /* Select 8 data bits */
  
  /* set no parity */
  options.c_cflag &= ~PARENB;
  options.c_cflag &= ~CSTOPB;
  options.c_cflag &= ~CSIZE;
  options.c_cflag |= CS8;
  
  /* disable flow controll */
  //options.c_cflag &= ~CNEW_RTSCTS;
  
  /*
   * Set the new options for the port...
   */
  
  tcsetattr(s_fd, TCSANOW, &options);
  
  // enable non blocking read
  //fcntl(s_fd, F_SETFL, FNDELAY);
  
  return (s_fd);
}

int swrite_string(char ch[])
{
  int n;
  n= write(s_fd, ch, strlen(ch));
}

void main()
{
  int bytes;
  int success;
  char buffer[5];
  
  success = sopen_port();
  if (success >= 0)
  {
    swrite_string("\nHello World function :)");
    swrite_string("\nSend please: ");
  } else
    printf("Oh no, could not open serial port!\n");
  
  while (buffer[0] != 'q')
  {
    bytes = read(s_fd, &buffer, sizeof(buffer));
    if (bytes>0)
    {
      swrite_string(buffer);
    }
    
  }
}
