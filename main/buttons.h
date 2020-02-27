
#include <stdint.h>

#define NUM_BUTTONS 4
#define BUTTON_EVENT_QUEUE_SIZE 16

enum ButtonEventType_e {
  PRESSED,
  RELEASED,
};

class ButtonEvent {
  public:
    uint8_t buttonNum;
    ButtonEventType_e eventType;
};

typedef struct ButtonMap {
  uint8_t buttonNum;
  ButtonEventType_e event;
  void * func;
}ButtonMap_t;

class Button {
  public:
    uint8_t buttonNum;
    bool pressed;
    ButtonEventType_e lastEvent;
    uint32_t lastEventTimestamp;
};

void BU_ReadButtons(void);
void BU_HandleButtonEvent(ButtonEvent *event);
uint8_t BU_GetButtons(void);
