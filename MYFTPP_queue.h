#ifndef DEF_MYFTPPQUEUE_HEADER
#define DEF_MYFTPPQUEUE_HEADER

#include <stdlib.h>

//Toutes les files sont terminees par un NULL !!!


/* File d'etat des moteurs */
/**/
typedef struct MYFTPP_MStateQueue MYFTPP_MStateQueue;
struct MYFTPP_MStateQueue {
  unsigned char state;
  MYFTPP_MStateQueue *next;
};

//cree un nouvel element de file
//renvoie NULL en cas d'erreur
MYFTPP_MStateQueue *MYFTPP_MStateQueue_new(unsigned char state);

//ajoute un element a la file et renvoie le premier element de la file
MYFTPP_MStateQueue *MYFTPP_MStateQueue_push(MYFTPP_MStateQueue *queue, MYFTPP_MStateQueue *elem);

//vide la file et renvoie le nombre d'elements supprimes
int MYFTPP_MStateQueue_empty(MYFTPP_MStateQueue *queue);
/**/



/*File d'etat des entrees (digitales et analogues)*/
/**/
typedef struct MYFTPP_IStateQueue MYFTPP_IStateQueue;
struct MYFTPP_IStateQueue {
  unsigned char digital;
  unsigned char analog1;
  unsigned char analog2;
  MYFTPP_IStateQueue *next;
};

//cree un nouvel element de file
//renvoie NULL en cas d'erreur
MYFTPP_IStateQueue *MYFTPP_IStateQueue_new(unsigned char digital, unsigned char analog1, unsigned char analog2);

//ajoute un element a la file et renvoie le premier element de la file
MYFTPP_IStateQueue *MYFTPP_IStateQueue_push(MYFTPP_IStateQueue *queue, MYFTPP_IStateQueue *elem);

//vide la file et renvoie le nombe d'elements liberes
int MYFTPP_IStateQueue_empty(MYFTPP_IStateQueue *queue);
/**/

#endif


