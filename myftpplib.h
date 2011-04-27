/*! \file */

#ifndef DEFINE_MYNEWFTPPLIB_HEADER
#define DEFINE_MYNEWFTPPLIB_HEADER

#include <pthread.h>
#include <unistd.h>
#include "mypplib.h"

#define MYFTPP_PIN_LOADOUT  MYPP_PIN_D0
#define MYFTPP_PIN_DATAOUT  MYPP_PIN_D2
#define MYFTPP_PIN_CLOCK    MYPP_PIN_D3
#define MYFTPP_PIN_TRIGGERX MYPP_PIN_D4
#define MYFTPP_PIN_TRIGGERY MYPP_PIN_D5
#define MYFTPP_PIN_LOADIN   MYPP_PIN_D1

#define MYFTPP_PIN_ALWAYSON MYFTPP_PIN_TRIGGERX|MYFTPP_PIN_TRIGGERY

#define MYFTPP_M_OFF 0
#define MYFTPP_M_CW  1
#define MYFTPP_M_CCW 3

#define MYFTPP_FPS 30

/*!
 * Recupere les valeurs des entrees binaires dans le tampon
 * @return L'etat des entrees
 */
unsigned char MYFTPP_getDigitalState();
#define MYFTPP_D() MYFTPP_getDigitalState() 

/*!
 * Recupere les valeurs des entrees analogues dans le tampon
 * \param[out] x l'etat de l'entree analogue X
 * \param[out] y l'etat de l'entree analogue Y
 * @return true si l'operation a reussi, false sinon
 */
MYPP_Bool MYFTPP_getAnalogState(unsigned char *x, unsigned char *y);
#define MYFTPP_A(x, y) MYFTPP_getAnalogState(x, y)

/*!
 * Enregistre l'etat des moteurs dans le tampon
 * \param[in] state L'etat des moteurs 
 * @return true si l'operation a reussi, false sinon
 */
MYPP_Bool MYFTPP_setOutState(unsigned char state);
#define MYFTPP_M(s) MYFTPP_setOutState(s)

/*!
 * Enregistre l'etat des moteurs dans le tampon a partir de valeurs individuelles
 * NB: Pratique pour utiliser MYFTPP_M_OFF, MYFTPP_M_CW et MYFTPP_M_CCW
 * \param[in] m1 Etat du moteur 1
 * \param[in] m2 Etat du moteur 2
 * \param[in] m3 Etat du moteur 3
 * \param[in] m4 Etat du moteur 4
 * @return Comme MYFTPP_setOutState
 */
MYPP_Bool MYFTPP_setMotors(char m1, char m2, char m3, char m4);
#define MYFTPP_MM(m1, m2, m3, m4) MYFTPP_setMotors(m1, m2, m3, m4)

/*!
 * \def MYFTPP_init()
 * Initialise la bibliotheque avec la socket par defaut de MYPP
 * @return 0 si l'initialisation a reussi ou un autre nombre en cas d'echec
 */
#define MYFTPP_init() MYFTPP_initWithSocket(MYPP_DEFPORT)

/*! 
 * Initialise la bibliotheque et ouvre l'interface
 * \param[in] socket La socket a utiliser pour ouvrir l'interface (voir MYPP)
 * @return 0 si l'initialisation a reussi ou un autre nombre en cas d'echec
 */
int MYFTPP_initWithSocket(const char *socket);

/*!
 * Ferme l'interface et decharge la bibliotheque
 * @return 0 en cas de reussite ou un autre nombre en cas d'echec
 */
int MYFTPP_exit();

/*!
 * Lance le thread de communication avec l'interface
 */
MYPP_Bool MYFTPP_runIOThread();

/*!
 * Stoppe le thread de communication avec l'interface
 */
MYPP_Bool MYFTPP_stopIOThread();

/*!
 * Attend la fin du thread de communication avec l'interface
 */
MYPP_Bool MYFTPP_waitIOThread();

/*!
 * \private Routine du thread de communication avec l'interface
 */
void *MYFTPP_IO_threadRoutine(void *data);

/*!
 * \private Utilise par le Thread de communication avec l'interface
 * Envoie l'etat des moteurs vers l'interface
 */
void MYFTPP_IO_setMotors();

/*!
 * \private Utilise par le Thread de communication avec l'interface
 * Recupere l'etat des entrees binaires
 */
void MYFTPP_IO_getDigital();

/*!
 * \private Utilise par le Thread de communication avec l'interface
 * Recupere l'etat des entrees analogues
 */
void MYFTPP_IO_getAnalog();


#endif /* ifndef DEFINE_MYNEWFTPPLIB_HEADER */
