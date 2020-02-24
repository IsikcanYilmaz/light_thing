#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
#include "Arduino.h"

uint8_t redPin = 3;
uint8_t greenPin = 6;
uint8_t bluePin = 5;

uint32_t pot1Val, pot1Normalized;
uint32_t pot2Val, pot2Normalized;
uint32_t pot3Val, pot3Normalized;

uint8_t pot1Pin = A0;
uint8_t pot2Pin = A2;
uint8_t pot3Pin = A3;

uint8_t btn1Pin = 10;
uint8_t btn2Pin = 9;
uint8_t btn3Pin = 8;
uint8_t btn4Pin = 7;

uint8_t btn1Val, btn2Val, btn3Val, btn4Val;

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

void readBtns(void) {
  btn1Val = digitalRead(btn1Pin);
  btn2Val = digitalRead(btn2Pin);
  btn3Val = digitalRead(btn3Pin);
  btn4Val = digitalRead(btn4Pin);
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

void buttonHandler(void) {
  
}

void setup() {
  // put your setup code here, to run once:
  
  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);
/*
  pinMode(btn1Pin, INPUT);
  pinMode(btn2Pin, INPUT);
  pinMode(btn3Pin, INPUT);
  pinMode(btn4Pin, INPUT);
  */
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
    readBtns();
    //printPots();
    //printBtns();
    
    analogWrite(bluePin, pot1Normalized);
    analogWrite(greenPin, pot2Normalized);
    analogWrite(redPin, pot3Normalized);
    
    delay(50);
  } 
  
}
