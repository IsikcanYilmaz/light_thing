#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 

uint8_t redPin = 3;
uint8_t greenPin = 6;
uint8_t bluePin = 5;

uint32_t pot1Val, pot1Normalized;
uint32_t pot2Val, pot2Normalized;
uint32_t pot3Val, pot3Normalized;

uint8_t pot1Pin = A0;
uint8_t pot2Pin = A2;
uint8_t pot3Pin = A3;

uint8_t blueVal = 0;
uint8_t greenVal = 0;
uint8_t redVal = 0;

#define POT_UPPER_LIMIT 713 

int i = 0;

uint8_t * colors[3] = {&blueVal, &greenVal, &redVal};

int8_t direction = 1;

void readPots(void) {
  pot1Val = analogRead(pot1Pin);
  pot1Normalized = 200 * pot1Val / POT_UPPER_LIMIT;
  pot2Val = analogRead(pot2Pin);
  pot2Normalized = 200 * pot2Val / POT_UPPER_LIMIT;
  pot3Val = analogRead(pot3Pin);
  pot3Normalized = 200 * pot3Val / POT_UPPER_LIMIT;
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
    Serial.print("\n");
}

void setup() {
  // put your setup code here, to run once:
  
  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  
  pinMode(pot1Pin, INPUT);
  pinMode(pot2Pin, INPUT);
  pinMode(pot3Pin, INPUT);


  Serial.begin(9600);
  
}

void loop() {
  // Pick 2 colors
  uint8_t color1 = (rand() % (2 - 0 + 1)) + 0; 
  uint8_t color2 = (rand() % (2 - 0 + 1)) + 0; 
  
  while(true)
  {
    readPots();
    printPots();
    
    i += direction;
    *colors[color1]+= direction;
    if (color1 != color2)
    *colors[color2]+= direction;

    /*
    analogWrite(bluePin, 0);
    analogWrite(greenPin, 0);
    analogWrite(redPin, 0);
    */
    
    analogWrite(bluePin, pot1Normalized);
    analogWrite(greenPin, pot2Normalized);
    analogWrite(redPin, pot3Normalized);
    
    uint32_t pot1Normalized = 100 * pot1Val / POT_UPPER_LIMIT;

    
    if (i >= 100)
    {
      direction = -1;
    }
    if (i <= 0)
    {
      direction = 1;
      break;
    }
    
    delay(50);
  } 
  
}
