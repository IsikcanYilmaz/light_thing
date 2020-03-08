#include <time.h> 
#include <math.h>
#include "main.h"
#include "buttons.h"
#include "player.h"
#include "Arduino.h"

#define POT_UPPER_LIMIT 713 

#define NUM_CHANNELS 3 

#define SERIAL_PRINT_BUFFER_LEN 64

const uint8_t redPin = 3;
const uint8_t greenPin = 6;
const uint8_t bluePin = 5;

const uint8_t onboardLedPin = 13;

const uint8_t pot1Pin = A0; // BLUE
const uint8_t pot2Pin = A2; // GREEN
const uint8_t pot3Pin = A3; // RED

const uint8_t btn1Pin = 10;
const uint8_t btn2Pin = 9;
const uint8_t btn3Pin = 8;
const uint8_t btn4Pin = 7;

volatile uint32_t pot1Val, pot1Normalized;
volatile uint32_t pot2Val, pot2Normalized;
volatile uint32_t pot3Val, pot3Normalized;

uint8_t btn1Val, btn2Val, btn3Val, btn4Val;

uint8_t blueVal = 0;
uint8_t greenVal = 0;
uint8_t redVal = 0;

int i = 0;

uint8_t * colors[3] = {&blueVal, &greenVal, &redVal};

int8_t direction = 1;

char printBuffer[SERIAL_PRINT_BUFFER_LEN];

LEDPlayer bluePlayer;
LEDPlayer greenPlayer;
LEDPlayer redPlayer;

LEDPlayer *ledPlayers[NUM_CHANNELS] = {&bluePlayer, &greenPlayer, &redPlayer};

uint8_t mode;



void readPots(void) {
  pot1Val = analogRead(pot1Pin);
  pot1Normalized = 100 * pot1Val / POT_UPPER_LIMIT;
  pot2Val = analogRead(pot2Pin);
  pot2Normalized = 100 * pot2Val / POT_UPPER_LIMIT;
  pot3Val = analogRead(pot3Pin);
  pot3Normalized = 100 * pot3Val / POT_UPPER_LIMIT;
}

void printBtns(void) {
  Serial.print(btn1Val);
  Serial.print(btn2Val);
  Serial.print(btn3Val);
  Serial.print(btn4Val);
  Serial.print('\n');
}

void printPots(void) {
  Serial.print("1:");
  Serial.print(pot1Val);
  Serial.print(" ");
  Serial.print(pot1Normalized);
  Serial.print("\n");

  Serial.print("2:");
  Serial.print(pot2Val);
  Serial.print(" ");
  Serial.print(pot2Normalized);
  Serial.print("\n");

  Serial.print("3:");
  Serial.print(pot3Val);
  Serial.print(" ");
  Serial.print(pot3Normalized);
  Serial.print("\n\n");
}

void setup() {
  // put your setup code here, to run once:

  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(onboardLedPin, OUTPUT);

  pinMode(btn1Pin, INPUT);
  pinMode(btn2Pin, INPUT);
  pinMode(btn3Pin, INPUT);
  pinMode(btn4Pin, INPUT);

  pinMode(pot1Pin, INPUT);
  pinMode(pot2Pin, INPUT);
  pinMode(pot3Pin, INPUT);

  Serial.begin(9600);

  initializeMode((Modes) mode);
  Serial.print("Initialized\n");
}

// MAIN LOOP //
void loop() {

  BU_ReadButtons();
  switch (mode){
  case DIRECT_PLAY:
  {
    readPots(); // TODO MOVE THIS TO ISR AND DEPRICATE THIS FUNCTION ENTIRELY
    analogWrite(bluePin, pot1Normalized);
    analogWrite(greenPin, pot2Normalized);
    analogWrite(redPin, pot3Normalized);
    delay(1);
    break;
  } 
  case RECORD_PLAY:
  {
    delay(0xffff); // DO THINGS IN THE ISR. TODO DELET DIS
    break;
  }
  case SINE_PLAY:
  {
    delay(0xffff); // DO THINGS IN THE ISR. TODO DELET DIS
    break;
  }
  defailt:
    break;
  }
}

void startRecording(uint8_t channel)
{
  // https://www.instructables.com/id/Arduino-Timer-Interrupts/
  Serial.print("START RECORDING CH ");
  Serial.println(channel);
  ledPlayers[channel]->enableRecording(true);
}

void stopRecording(uint8_t channel)
{
  Serial.print("STOP RECORDING CH ");
  Serial.println(channel);
  ledPlayers[channel]->enableRecording(false);
}

void toggleRecording(uint8_t channel)
{
  if (ledPlayers[channel]->isRecording())
  {
    stopRecording(channel);
  }
  else
  {
    startRecording(channel);
  }
}

/*
 *
 * MODE RELATED
 */

// Currently there are two modes.
// Mode 0, DIRECT_PLAY just reads the knobs and lights the rgbs accordingly
// Mode 1, RECORD_PLAY is where you get to record and play the recording
// Mode 2, SINE_PLAY record a bunch of sine waves and play them
void changeMode(void)
{
  noInterrupts();
  deinitializeMode((Modes) mode);
  mode++;
  mode = mode % NUM_MODES;
  initializeMode((Modes) mode);
  Serial.print("Mode changed to ");
  Serial.println(mode);
  interrupts();
}

void initializeMode(enum Modes m)
{
  switch(m)
  {
    case DIRECT_PLAY:
    {
      break;
    }
    case RECORD_PLAY:
    {
      initializeTimer1(PLAYER_SAMPLE_RATE);
      startTimer1();
      for (int i = 0; 0 < NUM_CHANNELS; i++)
      {
        ledPlayers[i]->reset();
        ledPlayers[i]->enableRecording(true);
      }
      break;
    }
    case SINE_PLAY:
    {
      initializeTimer1(PLAYER_SAMPLE_RATE);
      startTimer1();
      break;
    }
    case RANDOM_PLAY:
    {
      initializeTimer1(PLAYER_SAMPLE_RATE);
      startTimer1();
      break;
    }
  }
}

void deinitializeMode(enum Modes m)
{
  switch(m)
  {
    case DIRECT_PLAY:
    {
      break;
    }
    case SINE_PLAY:
    case RANDOM_PLAY:
    case RECORD_PLAY:
    {
      stopTimer1();
      break;
    }
  }
}


/*
 *
 * TIMER RELATED
 */
  
// Arduino clock runs at 16MHz
// At 16MHz each tick of the counter is 1/16000000 (~63 ns)
// In many cases, 16MHz is too fast. 
// This chip has 2 8 bit timers and a 16 bit timer
//
// When counter reaches maximum, it rolls back to zero,
// called an overflow. Change the speed of this counting up 
// via a prescaler. 
//
// timer speed in Hz = 16MHz / prescaler
//
// So a 1 prescaler will increment the counter at 16MHz, 8 will
// increment at 2, and so on. Prescaler can only be a power of 8.
//
// interrupt frequency in Hz = 16MHz / (prescaler * (compare match register + 1))
// the +1 is there because the compare match register is zero indexed
//
// Rearranging this equation will get you the value needed in the 
// compare match register = [ 16MHz / (prescaler * interrupt frequency) ] - 1

void initializeTimer1(uint16_t freq)
{
  // Initialize timers
  noInterrupts();
  uint16_t prescaler = 1024;
  uint16_t compMatchReg = (16000000 / (prescaler * freq)) - 1; 
  
  // Initialize Timer 1, the 16 bit timer
  // Since the compMatchReg value is larger than 255, we'll need to use the 16bit 
  // timer to do both the recording and the playing
  TCCR1A = 0; // Set the entire timer control register A to 0
  TCCR1B = 0; // same for B
  TCNT1  = 0; // counter = 0
  OCR1A  = compMatchReg; // set compare match register for 1Hz increments
  TCCR1B |= (1 << WGM12); // turn on ctc mode (clear timer on compare)
  //TCCR1B |= (1 << CS12) | (1 << CS10); // set cs10 and cs12 bits for 1024 prescaler
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  interrupts();
}

void startTimer1(void)
{
  TCCR1B |= (1 << CS12) | (1 << CS10); // set cs10 and cs12 bits for 1024 prescaler
}

void stopTimer1(void)
{
  TCCR1B = 0; // unset clock source setting of timer 1 
}

#define USE_AMPLITUDE_CHUNK 1
// TIMER 1 ISR
ISR(TIMER1_COMPA_vect)
{
  readPots();
  if (mode == RANDOM_PLAY)
  {
    static int amplitudeChunk = 899;
    static float amplitude[3]  = {0, 0, 0}; // amplitude (0 - 100)
    static float rate[3]       = {0, 0, 0}; // Hz
    static float target[3]     = {0, 0, 0}; // Hz
    static uint32_t  wait[3]   = {0, 0, 0}; // Samples (Seconds * sampleRate)

    for (int i = 0; i < 3; i++)
    {
      // Increment or decrement our amplitude with the rate of change (time step)
      (amplitude[i] < target[i]) ? (amplitude[i] += rate[i]) : (amplitude[i] -= rate[i]);

      // Check if we're done. if we're done, find new target and rate
      if (abs(amplitude[i] - target[i]) > rate[i])
      {
        continue;
      }

      // If there's waiting to be done, wait
      if (wait[i])
      {
        wait[i]--;
        continue;
      }

      // Target reached. find/set new target and rate
      #if USE_AMPLITUDE_CHUNK
      amplitudeChunk += target[i];
      float newTarget = ((rand() % ((999 - amplitudeChunk) - 0 + 1) + 0) / 10.0); //(rand() % (upper - lower + 1)) + lower;
      amplitudeChunk -= newTarget;
      #else
      float newTarget = ((rand() % (999 - 0 + 1) + 0) / 10.0); //(rand() % (upper - lower + 1)) + lower;
      #endif
      float newRate = ((rand() % (1500 - 100 + 1) + 100) / 1000.0); //(rand() % (upper - lower + 1)) + lower;
      uint32_t newWait = ((rand() % (3 - 0 + 1) + 0) * PLAYER_SAMPLE_RATE);
      wait[i] = newWait; 
      target[i] = newTarget;
      rate[i] = newRate;
    }

    analogWrite(bluePin, (uint8_t) amplitude[0]);
    analogWrite(redPin, (uint8_t) amplitude[1]);
    analogWrite(greenPin, (uint8_t) amplitude[2]);
    return;
  }
  if (mode == SINE_PLAY)
  {
    const float TIME_STEP = 1.0f / (float) PLAYER_SAMPLE_RATE;

    static float bluePhase = 0;
    static float greenPhase = 0;
    static float redPhase = 0;

    float blueFreq = 0.05;
    float greenFreq = 0.25;
    float redFreq = 0.25;

    float overallFreqMultiplier = 4;
    uint8_t blueVal  = (uint8_t) (sin(2 * PI * blueFreq * bluePhase * overallFreqMultiplier) * 50 + 50) + 1;
    uint8_t greenVal = (uint8_t) (sin(2 * PI * greenFreq * greenPhase * overallFreqMultiplier) * 50 + 50);
    uint8_t redVal   = (uint8_t) (sin(2 * PI * redFreq * redPhase * overallFreqMultiplier) * 50 + 50);

    bluePhase += TIME_STEP;
    greenPhase += TIME_STEP;
    redPhase += TIME_STEP;

    analogWrite(bluePin, blueVal);
    //analogWrite(redPin, redVal);
    analogWrite(greenPin, greenVal);
    return;
  }
  //static bool toggle = false;
  //digitalWrite(onboardLedPin, (toggle ? HIGH : LOW));
  //toggle = !toggle;
  static uint16_t blueIndex = 0;
  static uint16_t redIndex = 0;
  static uint16_t greenIndex = 0;
  BU_ReadButtons();

  digitalWrite(onboardLedPin, HIGH);
  if (!bluePlayer.isFull()) 
  {
    readPots();
    bluePlayer.recordSample((uint16_t)pot1Normalized);
    greenPlayer.recordSample((uint16_t)pot2Normalized);
    redPlayer.recordSample((uint16_t)pot3Normalized);
    analogWrite(bluePin, pot1Normalized);
    analogWrite(greenPin, pot2Normalized);
    analogWrite(redPin, pot3Normalized);
    digitalWrite(onboardLedPin, LOW);
  }
  else
  {
    /*
       stopTimer1();
       Serial.println("RECORD FINISHED");
       Serial.println(bluePlayer.isRecording());
       Serial.println(bluePlayer.getCurrentLength());
     */
    analogWrite(bluePin, bluePlayer.getSample(blueIndex));
    analogWrite(greenPin, greenPlayer.getSample(blueIndex));
    analogWrite(redPin, redPlayer.getSample(blueIndex));
    blueIndex = (blueIndex + 1) % PLAYER_RECORDING_CAPACITY;
  }
}
