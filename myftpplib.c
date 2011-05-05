#include "myftpplib.h"

MYPP_Port MYFTPP_interface = 0;
pthread_t MYFTPP_io_thread;

unsigned char MYFTPP_out_state = 0;
pthread_mutex_t MYFTPP_out_mutex = PTHREAD_MUTEX_INITIALIZER;

unsigned char MYFTPP_in_state  = 0;
pthread_mutex_t MYFTPP_in_mutex = PTHREAD_MUTEX_INITIALIZER;

unsigned char MYFTPP_analog_x  = 0;
unsigned char MYFTPP_analog_y  = 0;
pthread_mutex_t MYFTPP_analog_mutex = PTHREAD_MUTEX_INITIALIZER; 


unsigned char MYFTPP_getDigitalState(){
    unsigned char res = 0;
    
    pthread_mutex_lock(&MYFTPP_in_mutex);
    res = MYFTPP_in_state;
    pthread_mutex_unlock(&MYFTPP_in_mutex);
    
    return res;
}

MYPP_Bool MYFTPP_getAnalogState(unsigned char *x, unsigned char *y){
    pthread_mutex_lock(&MYFTPP_analog_mutex);
    *x = MYFTPP_analog_x;
    *y = MYFTPP_analog_y;
    pthread_mutex_unlock(&MYFTPP_analog_mutex);
    return true;
}

MYPP_Bool MYFTPP_setOutState(unsigned char state){
    pthread_mutex_lock(&MYFTPP_out_mutex);
    MYFTPP_out_state = state;
    pthread_mutex_unlock(&MYFTPP_out_mutex);
    return true;
}

MYPP_Bool MYFTPP_setMotors(char m1, char m2, char m3, char m4){
    unsigned char state;
    
    m1 = m1 & MYFTPP_M_CCW;
    m2 = m2 & MYFTPP_M_CCW;
    m3 = m3 & MYFTPP_M_CCW;
    m4 = m4 & MYFTPP_M_CCW;
    
    state = m4 | (m3 << 2) | (m2 << 4) | (m1 << 6);
    return MYFTPP_setOutState(state);
}

int MYFTPP_initWithSocket(const char *socket){
    MYPP_Port res = MYPP_open(socket);
    if (res <= 0)
        return -1;
    MYFTPP_interface = res;
    return 0;
}

int MYFTPP_exit(){
    return MYPP_close(MYFTPP_interface);
}

MYPP_Bool MYFTPP_runIOThread(){
    if (MYFTPP_interface == 0)
        return false;
    if (pthread_create(&MYFTPP_io_thread, NULL, MYFTPP_IO_threadRoutine, NULL) != 0)
        return false;
    return true;
}

MYPP_Bool MYFTPP_stopIOThread(){
    if (pthread_cancel(MYFTPP_io_thread) != 0)
        return false;
    return true;
}

MYPP_Bool MYFTPP_waitIOThread(){
    void **res = NULL;
    if (pthread_join(MYFTPP_io_thread, res) != 0)
        return false;
    if (res != NULL)
        free(res);
    return true;
}

void *MYFTPP_IO_threadRoutine(void *data){
    MYPP_Bool running = true;
    while (running){
        MYFTPP_IO_setMotors();
        MYFTPP_IO_getDigital();
        MYFTPP_IO_getAnalog();
    }
    return NULL;
}

void MYFTPP_IO_setMotors(){
    int i;
    unsigned char value, value_to_send;
    
    pthread_mutex_lock(&MYFTPP_out_mutex);
    value = MYFTPP_out_state;
    pthread_mutex_unlock(&MYFTPP_out_mutex);
    
    MYFTPP_write(MYFTPP_PIN_CLOCK|MYFTPP_PIN_LOADOUT|MYFTPP_PIN_ALWAYSON);
    for (i=0; i<8; i++){
        value_to_send = ((value >> i) & 1) * MYFTPP_PIN_DATAOUT;
        MYFTPP_write(value_to_send|MYFTPP_PIN_ALWAYSON);
		
        MYFTPP_write(MYFTPP_PIN_CLOCK|MYFTPP_PIN_ALWAYSON);
		
    }
    MYFTPP_write(MYFTPP_PIN_LOADOUT|MYFTPP_PIN_CLOCK|MYFTPP_PIN_ALWAYSON);
	
}

void MYFTPP_IO_getDigital(){
    int i;
    unsigned char res = 0;
    
    MYFTPP_write(MYFTPP_PIN_CLOCK|MYFTPP_PIN_ALWAYSON);
	
    MYFTPP_write(MYFTPP_PIN_LOADIN|MYFTPP_PIN_ALWAYSON);
    for (i=0; i<8; i++){
        MYFTPP_write(MYFTPP_PIN_CLOCK|MYFTPP_PIN_ALWAYSON);
		
        if (MYPP_isBusy(MYFTPP_interface))
            res += 1;
        res <<= 1;
    }
    
    pthread_mutex_lock(&MYFTPP_in_mutex);
    MYFTPP_in_state = res;
    pthread_mutex_unlock(&MYFTPP_in_mutex);
}

void MYFTPP_IO_getAnalog(){
    unsigned char trigger_pins[2] = {MYFTPP_PIN_TRIGGERY, MYFTPP_PIN_TRIGGERX};
    unsigned int tmp[2] = {0, 0};
    int i, j;

    for (i=0; i<2; i++){
        MYFTPP_write(MYFTPP_PIN_CLOCK|MYFTPP_PIN_ALWAYSON);
        MYFTPP_write(MYPP_PIN_DNONE|trigger_pins[i]);
        MYFTPP_write(MYFTPP_PIN_CLOCK|MYFTPP_PIN_ALWAYSON);

        j = 0;
        while (MYPP_isBusy(MYFTPP_interface) && j<500){
            tmp[i] ++;
            usleep(1000);
            j++;
        }
    }
    
    pthread_mutex_lock(&MYFTPP_analog_mutex);
    MYFTPP_analog_x = tmp[0]/2;
    MYFTPP_analog_y = tmp[1]/2;
    pthread_mutex_unlock(&MYFTPP_analog_mutex);
}



