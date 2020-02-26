
#include <stdint.h>

template <class T>
class Queue {
  public:
    // CONSTRUCTOR / DESTRUCTOR
    Queue(T *queueArray):queueArray(queueArray){};
    ~Queue(void){};

    // FUNCTIONS
    void push(T element);
    T pop(void);
    uint16_t getNumElements(void);
    bool isEmpty(void);
    
  
  
  private:
    T *queueArray;
    T *queueHeadPtr;
    uint16_t capacity;
    uint16_t size;
};


