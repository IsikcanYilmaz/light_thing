
#include "queue.h"
#include "buttons.h"
#include "HardwareSerial.h"
#include <string.h>

template <class T>
void Queue<T>::push(T element) {
  if (this->size < this->capacity) {
    memcpy(&queueArray[queueHeadIndex], &element, sizeof(T));
    Serial.print("Pushing element. objdump: ");
    Serial.println(((ButtonEvent *) &element)->buttonNum);
    //queueArray[queueHeadIndex] = element;
    queueHeadPtr++;
    size++;
    queueHeadIndex++;
  }
}

// POP 
// Returns pointer of the front element of the queue
// TODO should this return the object itself and not a ptr?
template <class T>
T Queue<T>::pop(void) 
{
  if (size > 0){
    queueHeadIndex -= 1;
  }
  T ret = queueArray[queueHeadIndex]; 
  size--;
  return ret;
}

template <class T>
uint16_t Queue<T>::getNumElements(void) 
{
  return size;
}

template class Queue<ButtonEvent>;
