
#include "queue.h"
#include "buttons.h"
#include "HardwareSerial.h"
#include <string.h>

// PUSH
// Takes pointer to object of template class T
// Copies the contents to head of queueArray
template <class T>
void Queue<T>::push(T *element) {
  if (this->size < this->capacity) {
    memcpy(queueArray[queueHeadIndex], &element, sizeof(T));
    Serial.print("Pushing element. objdump: ");
    Serial.println(((ButtonEvent *) &element)->buttonNum);
    //queueArray[queueHeadIndex] = element;
    queueHeadPtr++;
    size++;
    queueHeadIndex++;
  }
}

// POP 
// Returns the element at the head of the queueArray
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

// Returns current size of the queue
template <class T>
uint16_t Queue<T>::getNumElements(void) 
{
  return size;
}

template class Queue<ButtonEvent>;
