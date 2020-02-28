
#include "Arduino.h"
#include "main.h"
#include "buttons.h"
#include "queue.h"
#include "HardwareSerial.h"


ButtonMap_t buttonMap[] = {
  {7, PRESSED, NULL},
  {7, RELEASED, NULL},
  {8, PRESSED, NULL},
  {8, RELEASED, NULL},
  {9, PRESSED, NULL},
  {9, RELEASED, NULL},
  {10, PRESSED, NULL},
  {10, RELEASED, NULL}
};

uint8_t buttonPins[NUM_BUTTONS] = {10, 9, 8, 7};
uint16_t buttonsBitField = 0;

ButtonEvent _buttonEventQueue[BUTTON_EVENT_QUEUE_SIZE];
Queue<ButtonEvent> buttonEventQueue = Queue<ButtonEvent>(&_buttonEventQueue[0], BUTTON_EVENT_QUEUE_SIZE);

void BU_Init(void){
}


void BU_EnqueueButtonEvent(uint16_t buttonNum, ButtonEventType_e event) {
  ButtonEvent e;
  e.buttonNum = buttonNum;
  e.eventType = event;
  buttonEventQueue.push(&e);
}

// THIS IS TO RUN EVERY 1ms
void BU_ReadButtons(void) {
  uint16_t tmpButtonsBitField = 0x0;
  // Go thru all buttons, write current state to $tmpButtonsBitField
  for (int i = 0; i < NUM_BUTTONS; i++){
    uint8_t currentReading = digitalRead(buttonPins[i]);
    uint8_t pastReading = (uint8_t) (buttonsBitField & (1 << i)) > 0; 

    if (currentReading != pastReading){
      // Find out if this is a press or a release event
      ButtonEventType_e eventType = (currentReading > pastReading) ? PRESSED : RELEASED;
      
      // Enqueue event to be processed after this loop
      BU_EnqueueButtonEvent(i, eventType);
    }
    tmpButtonsBitField |= currentReading << i;
  }
  buttonsBitField = tmpButtonsBitField;

  // Go thru queued button events
  while(buttonEventQueue.getNumElements()) {
    ButtonEvent e = buttonEventQueue.pop();
    BU_HandleButtonEvent(&e);
  }
}

void BU_HandleButtonEvent(ButtonEvent *event) {
  Serial.print("Button: ");
  Serial.print(event->buttonNum);
  Serial.print(" Type: ");
  Serial.print((uint8_t) event->eventType);
  switch (event->eventType) {
  case PRESSED: 
    {
      // TODO // HANDLE DEBOUNCE
      Serial.println(" PRESSED");
    }
    break;
  case RELEASED:
    {
      // TODO // HANDLE DEBOUNCE
      // TODO // DO THIS THRU THE LOOK UP TABLE
      // TODO // GENERAL BUTTONS POLISH
      if (event->buttonNum > 0)
      {
        Serial.println(" RELEASED");
        toggleRecording(0);
      } 
      else
      {
        Serial.println("\nCHANGING MODE");
        changeMode();
      }
    }
    break;
  default:
    break;
  }
}

uint8_t BU_GetButtons(void) {
  return buttonsBitField;
}
