#include "myftpplib.h"
#define TITOU_DEBUG

#if defined TITOU_DEBUG
static int my_debug_var_ind = 1;
#define myDebug printf("Debug checkpoint (%s::%d): %d\n", __FILE__, __LINE__, my_debug_var_ind); my_debug_var_ind++
#else
#define myDebug
#endif

#define OFF MYFTPP_M_OFF
#define CW  MYFTPP_M_CW
#define CCW MYFTPP_M_CCW

int test1(int argc, char **argv){
  int i;
  
  MYFTPP_init();
    
  MYFTPP_setMotors(CW, OFF, OFF, OFF);
  MYFTPP_runIOThread();
  for (i=0; i<25; i++){
      MYFTPP_MM(rand(), rand(), rand(), rand());
      usleep(100000);
      printf("In: %d\n", MYFTPP_D());
  }
  MYFTPP_stopIOThread();
  MYFTPP_exit();
  
  return 0;
}

int main(int argc, char **argv){
  int res;
  
  myDebug;
  res = test1(argc, argv);
  myDebug;
  
  return res;
}


