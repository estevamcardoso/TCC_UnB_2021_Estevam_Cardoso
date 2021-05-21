#ifndef _PIN_HANDLER_H
#define _PIN_HANDLER_H

#include <Arduino.h>

// PIN definitions
#define LED_PIN1 2     // Built-in and blue LED
#define LED_PIN2 16    // Red LED
#define ADC_PIN 34     // ADC_1 channel 6
#define BUTTON_PIN1 22 // Button 1 for interruption
#define BUTTON_PIN2 23 // Button 2 for interruption

bool led1_toggle = false;
bool led2_toggle = false;

void pinAssignment()
{
  pinMode(BUTTON_PIN1, INPUT_PULLUP);
  pinMode(BUTTON_PIN2, INPUT_PULLUP);
  pinMode(ADC_PIN, INPUT);
  //analogSetAttenuation(ADC_6db);
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  digitalWrite(LED_PIN1, true);
  digitalWrite(LED_PIN2, true);
}

#endif