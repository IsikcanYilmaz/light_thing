#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 

uint8_t bluePin = 11;
uint8_t greenPin = 10;
uint8_t redPin = 9;

uint8_t blueVal = 0;
uint8_t greenVal = 0;
uint8_t redVal = 0;

int i = 0;

uint8_t * colors[3] = {&blueVal, &greenVal, &redVal};

int8_t direction = 1;

void setup() {
  // put your setup code here, to run once:
  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  
}

void loop() {
  // Pick 2 colors
  uint8_t color1 = (rand() % (2 - 0 + 1)) + 0; 
  uint8_t color2 = (rand() % (2 - 0 + 1)) + 0; 
  
  while(true)
  {
      i += direction;
      *colors[color1]+= direction;
      if (color1 != color2)
        *colors[color2]+= direction;
      
      analogWrite(bluePin, blueVal);
      analogWrite(greenPin, greenVal);
      analogWrite(redPin, redVal);

      if (i >= 255)
      {
        direction = -1;
      }
      if (i <= 0)
      {
        direction = 1;
        break;
      }
      
      delay(1);
  } 
  
}
