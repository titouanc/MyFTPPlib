#include "MYFTPP_queue.h"

MYFTPP_MStateQueue *MYFTPP_MStateQueue_new(unsigned char state){
  MYFTPP_MStateQueue *res = NULL;
  
  res = malloc(sizeof(MYFTPP_MStateQueue));
  if (res != NULL){
    res->next = NULL;
    res->state = state;
  }
    
  return res;
}

MYFTPP_MStateQueue *MYFTPP_MStateQueue_push(MYFTPP_MStateQueue *queue, MYFTPP_MStateQueue *elem){
  MYFTPP_MStateQueue *current = queue;
  
  while (current->next != NULL)
    current = current->next;
  current->next = elem;
  
  return queue;  
}

int MYFTPP_MStateQueue_empty(MYFTPP_MStateQueue *queue){
  int res = 0;
  MYFTPP_MStateQueue *cur = queue;
  
  while (cur->next != NULL){
    queue = cur;
    cur = cur->next;
    free(queue);
    res++;
  }
  
  free(cur);
  res ++;
  
  return res;
}

MYFTPP_IStateQueue *MYFTPP_IStateQueue_new(unsigned char digital, unsigned char analog1, unsigned char analog2){
  MYFTPP_IStateQueue *res = NULL;
  
  res = malloc(sizeof(MYFTPP_IStateQueue));
  if (res != NULL){
    res->next = NULL;
    res->digital = digital;
    res->analog1 = analog1;
    res->analog2 = analog2;
  }
    
  return res;
}

MYFTPP_IStateQueue *MYFTPP_IStateQueue_push(MYFTPP_IStateQueue *queue, MYFTPP_IStateQueue *elem){
  MYFTPP_IStateQueue *current = queue;
  
  while (current->next != NULL)
    current = current->next;
  current->next = elem;
  
  return queue;
}

int MYFTPP_IStateQueue_empty(MYFTPP_IStateQueue *queue){
  int res = 0;
  MYFTPP_IStateQueue *cur = queue;
  
  while (cur->next != NULL){
    queue = cur;
    cur = cur->next;
    free(queue);
    res++;
  }
  
  free(cur);
  res ++;
  
  return res;
}

