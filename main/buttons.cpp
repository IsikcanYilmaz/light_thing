
#include "Arduino.h"
#include "main.h"
#include "buttons.h"
#include "queue.h"


uint8_t buttonPins[NUM_BUTTONS] = {10, 9, 8, 7};
uint16_t buttonsBitField = 0;

ButtonEvent _buttonEventQueue[BUTTON_EVENT_QUEUE_SIZE];
Queue<ButtonEvent> buttonEventQueue = Queue<ButtonEvent>(&_buttonEventQueue[0]);

void BU_Init(void){
}

void BU_EnqueueButtonEvent(uint16_t buttonNum, ButtonEventType_e event) {
  ButtonEvent e;
  e.buttonNum = buttonNum;
  e.event     = event;
  buttonEventQueue.push(e);
}

// THIS IS TO RUN EVERY 1ms
void BU_ReadButtons(void) {
  uint16_t tmpButtonsBitField = 0x0;
  // Go thru all buttons, write current state to $tmpButtonsBitField
  for (int i = 0; i < NUM_BUTTONS; i++){
    tmpButtonsBitField |= digitalRead(buttonPins[i]) << i;
    if (tmpButtonsBitField << i != buttonsBitField << i) {
      // Find out if this is a press or a release event
      ButtonEventType_e eventType = (tmpButtonsBitField << i);
      
      // Enqueue event to be processed after this loop
      BU_EnqueueButtonEvent(i, eventType);
    }
  }
  
  // Go thru queued button events
  while(buttonEventQueue.getNumElements()) {
    ButtonEvent e = buttonEventQueue.pop();

  }
}

uint8_t BU_GetButtons(void) {
  return buttonsBitField;
}
