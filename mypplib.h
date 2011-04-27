/*! \file */

#ifndef DEFINE_MYPPLIB_HEADER
#define DEFINE_MYPPLIB_HEADER

#define MYPP_DEFPORT "/dev/parport0"

//Valeur des sorties Data du port parallele
#define MYPP_PIN_DNONE 0x00
#define MYPP_PIN_D0    0x01
#define MYPP_PIN_D1    0x02
#define MYPP_PIN_D2    0x04
#define MYPP_PIN_D3    0x08
#define MYPP_PIN_D4    0x10
#define MYPP_PIN_D5    0x20
#define MYPP_PIN_D6    0x40
#define MYPP_PIN_D7    0x80
#define MYPP_PIN_DALL  0xff

#define MYPP_getError() MYPP_error_string
#define MYPP_reset(port) MYPP_write(port, MYPP_PIN_DNONE)

#define MYPP_ERRORBUFFER_LENGTH 1024

#if defined linux

#include <sys/ioctl.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/errno.h>
#include <linux/ppdev.h>
#include <linux/parport.h>

static char MYPP_error_string[MYPP_ERRORBUFFER_LENGTH] = {0};

typedef int MYPP_Port;

typedef enum {false=0, true} MYPP_Bool;

/* OUVERTURE ET FERMETURE DU PORT */
/**/
//renvoie un port
//renvoie un negatif en cas d'erreur
MYPP_Port MYPP_open(const char *socket);

//ferme le port
//renvoie 0 en cas de reussite ou un negatif en cas d'erreur
int MYPP_close(MYPP_Port port);
/**/


/* ECRITURE SUR LE PORT */
/**/
//ecrit un nombre sur le port
//renvoie 0 en cas de reussite ou un negatif en cas d'erreur
int MYPP_write(MYPP_Port port, int value);


//allume le port en impulsions de 1/100 de seconde pendant +duration+ secondes
//renvoie 0 en cas de succes ou un negatif en cas d'erreur
int MYPP_writeDimmed(MYPP_Port port, int value, float duration);


//envoie des impulsions sur le port.
//+value+ est la valeur a ecrire sur le port
//+period+ est exprime en 1/10000 de seconde
//+ratio_on+ est exprime en fraction de la periode, et est le temps pendant lequel le port est sur on
//+repeat+ est le nombre de fois qu'il faut repeter la periode
//renvoie 0 en cas de succès ou un negatif en cas d'erreur
/*
 
1|----------+      +----------+      +- etc....
 |          |      |          |      |
0|          +------+          +------+
 +------------------------------------------------------------> temps
 
 |-------- P ------| period
 |--- ON ---|        ratio_on
*/
int MYPP_impulses(MYPP_Port port, int value, unsigned int period, float ratio_on, unsigned int repeat);
/**/


/* LECTURE DU PORT */
/**/
//Lit les registres d'etat du port parallele. 
//Renvoie la valeur du registre d'etat
//Renvoie un negatif en cas d'erreur
int MYPP_read(MYPP_Port port);

//Lit les registres d'etat et renvoie un booleen indiquant le statut de la ligne Busy
//renvoie true s'il y a du courant, false sinon
MYPP_Bool MYPP_isBusy(MYPP_Port port);

/**/

#else /* if defined linux */

#include <stdio.h>
#include <stdlib.h>

static char MYPP_error_string[MYPP_ERRORBUFFER_LENGTH] = "MYPPlib en mode simulation (hors linux)";

typedef int MYPP_Port;

typedef enum {false=0, true} MYPP_Bool;

#define MYPP_open(socket) 1

#define MYPP_close(port)  0

#define MYPP_write(port, value) fprintf(stdout, "Ecriture de %d\n", value)

#define MYPP_writeDimmed(port, value, duration) fprintf(stdout, "Ecriture de %d a 50%% pendant %fs\n", value, duration)

#define MYPP_impulses(port, value, period, ratio_on, repeat) fprintf(stdout, "Ecriture de %d a %f\n", value, ratio_on)

#define MYPP_read(port) (1)

#define MYPP_isBusy(port) (MYPP_Bool) true

#endif /* if defined linux */

#endif /* ifndef DEFINE_MYPPLIB_HEADER */

