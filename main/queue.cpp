
#include "queue.h"
#include "buttons.h"
#include <string.h>

template <class T>
void Queue<T>::push(T element) {
  if (this->size < this->capacity) {
    memcpy((void *) queueHeadPtr, (const void *) &element, sizeof(T));
    queueHeadPtr++;
    size++;
  }
}

template <class T>
T Queue<T>::pop(void) 
{
  if (size > 0){
    queueHeadPtr -= 1;
  }
  T ret = *queueHeadPtr;
  return ret;
}

template <class T>
uint16_t Queue<T>::getNumElements(void) 
{
  return size;
}




template class Queue<ButtonEvent>;
