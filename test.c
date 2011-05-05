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
  int i,j;
  
  MYFTPP_init();
    
  MYFTPP_setMotors(CW, OFF, OFF, OFF);
  MYFTPP_runIOThread();
  printf("  ");
  for (j=1; j<=8; j++)
		printf(" |  E%d", j);
  printf("\n");
  for (j=0; j<50; j++)
		printf("-");
  printf("\n");
  for (i=0; i<25; i++){
      usleep(100000);
	  printf("%2d", i);
	  for (j=1; j<=8; j++){
		if (MYFTPP_ISON_E(j))
			printf(" | \033[32mON!\033[0m");
		else
			printf(" | \033[31mOFF\033[0m");
	  }
	  printf("\n");
  }
  MYFTPP_stopIOThread();
  MYFTPP_exit();
  
  return 0;
}

int test2(int argc, char **argv){
	return 0;
}

int main(int argc, char **argv){
  return test1(argc, argv) + test2(argc, argv);
}


