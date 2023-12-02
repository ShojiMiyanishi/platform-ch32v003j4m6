#include <Arduino.h>

/* PC1 as blinky LED, change here as needed */
#define LED1 PC2
#define LED2 PC4
//#define LED3 PD6
#define LED4 PA2
#define button PD6

int buttonState=0;

void setup() {
  //pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
//  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(button,INPUT);
}

void loop() {
  buttonState = digitalRead(button);
  if ( buttonState == HIGH ){
    //digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    //digitalWrite(LED3, HIGH);
    //digitalWrite(LED4, HIGH);
  }else{
    //digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    //digitalWrite(LED3, LOW);
    //digitalWrite(LED4, LOW);
  }
  delay(1000);
}