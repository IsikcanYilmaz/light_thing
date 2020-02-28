#include <time.h> 
#include "main.h"
#include "buttons.h"
#include "player.h"
#include "Arduino.h"

#define POT_UPPER_LIMIT 713 

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

uint32_t pot1Val, pot1Normalized;
uint32_t pot2Val, pot2Normalized;
uint32_t pot3Val, pot3Normalized;

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

LEDPlayer *ledPlayers[3] = {&bluePlayer, &greenPlayer, &redPlayer};

uint8_t mode;



void readPots(void) {
  pot1Val = analogRead(pot1Pin);
  pot1Normalized = 200 * pot1Val / POT_UPPER_LIMIT;
  pot2Val = analogRead(pot2Pin);
  pot2Normalized = 200 * pot2Val / POT_UPPER_LIMIT;
  pot3Val = analogRead(pot3Pin);
  pot3Normalized = 200 * pot3Val / POT_UPPER_LIMIT;
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

  Serial.print("Initialized\n");
}

// MAIN LOOP //
void loop() {

  BU_ReadButtons();
  switch (mode){
  case DIRECT_PLAY:
  {
    readPots();
    analogWrite(bluePin, pot1Normalized);
    analogWrite(greenPin, pot2Normalized);
    analogWrite(redPin, pot3Normalized);
    delay(1);
    break;
  } 
  case RECORD_PLAY:
  {
    delay(0xffff);
    break;
  }
  defailt:
    break;
  }
}

void doRecording(void)
{
  
}

void startRecording(uint8_t channel)
{
  // https://www.instructables.com/id/Arduino-Timer-Interrupts/
  Serial.print("START RECORDING CH ");
  Serial.println(channel);
  ledPlayers[channel]->setRecording(true);
}

void stopRecording(uint8_t channel)
{
  Serial.print("STOP RECORDING CH ");
  Serial.println(channel);
  ledPlayers[channel]->setRecording(false);
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
// 
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
      bluePlayer.setRecording(true);
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

// TIMER 1 ISR
ISR(TIMER1_COMPA_vect)
{
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
