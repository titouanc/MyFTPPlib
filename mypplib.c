#include "mypplib.h"

#if defined linux

MYPP_Port MYPP_open(const char *socket){
	MYPP_Port fd;
	int errno;
	
	if ((fd = open(socket, O_RDWR)) <= 0) {
		snprintf(MYPP_error_string, MYPP_ERRORBUFFER_LENGTH, "Open Error : %s (%d)\n", strerror(errno), errno);
		return -1;
	}
	if (ioctl(fd, PPCLAIM) < 0) {
		snprintf(MYPP_error_string, MYPP_ERRORBUFFER_LENGTH, "PPCLAIM ioctl Error : %s (%d)\n", strerror(errno), errno);
		return -2;
	}
	
	return fd;
}


int MYPP_close(MYPP_Port port){
	int errno;
	
	if (ioctl(port, PPRELEASE) < 0) {
		snprintf(MYPP_error_string, MYPP_ERRORBUFFER_LENGTH, "PPRELEASE ioctl Error : %s (%d)\n", strerror(errno), errno);
		return -2;
	}

	if (close(port) < 0) {
		snprintf(MYPP_error_string, MYPP_ERRORBUFFER_LENGTH, "Close Error : %s (%d)\n", strerror(errno), errno);
		return -1;
	}
	
  return 0;
}


int MYPP_write(MYPP_Port port, int value){
	unsigned char valout = value & 0xff;
	int errno;
	
	if (ioctl(port, PPWDATA, &valout) < 0) {
		snprintf(MYPP_error_string, MYPP_ERRORBUFFER_LENGTH, "PPWDATA ioctl Error : %s (%d)\n", strerror(errno), errno);
		return -1;
	}
	
	return 0;
}


int MYPP_writeDimmed(MYPP_Port port, int value, float duration){
	int i, res=0;
	
	for (i=0; i<(duration/2*10000); i++){
		res = MYPP_write(port, value);
		if (res != 0)
			return res;
		usleep(100);
		MYPP_reset(port);
		usleep(100);
	}
	
	MYPP_reset(port);
	
	return res;
}


int MYPP_impulses(MYPP_Port port, int value, unsigned int period, float ratio_on, unsigned int repeat){
	unsigned int i, j;
	int res=0;
	
	for (j=0; j<repeat; j++){
		for (i=0; i<period; i++){
			if (i <= period*ratio_on)
				res = MYPP_write(port, value);
			else
				res = MYPP_reset(port);
			if (res != 0)
				return res;
			usleep(100);
		}
	}
	
	return res;
}


int MYPP_read(MYPP_Port port){
  unsigned char status;
  int errno;
  
  if (ioctl(port, PPRSTATUS, &status) < 0){
    snprintf(MYPP_error_string, MYPP_ERRORBUFFER_LENGTH, "PPRSTATUS ioctl Error : %s (%d)\n", strerror(errno), errno);
    return -1;
  } else {
    return status & 0xff;
  }
}


MYPP_Bool MYPP_isBusy(MYPP_Port port){
  if ((MYPP_read(port) & PARPORT_STATUS_BUSY) == 0)
    return true;
  return false;
}


#endif

