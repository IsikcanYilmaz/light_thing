#include <stdio.h> 
#include <stdlib.h> 
#include <stdint.h>

enum Modes {
  DIRECT_PLAY = 0,
  RECORD_PLAY,
  NUM_MODES
};


void startRecording(uint8_t channel);
void stopRecording(uint8_t channel);
void toggleRecording(uint8_t channel);

// MODE STUFF
void changeMode(void);
void initializeMode(enum Modes m);
void deinitializeMode(enum Modes m);

// HARDWARE TIMER STUFF
void initializeTimer1(uint16_t freq);
void startTimer1(void);
void stopTimer1(void);
