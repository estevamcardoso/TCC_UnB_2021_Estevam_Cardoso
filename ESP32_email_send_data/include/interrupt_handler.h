#ifndef _INTERRUPT_HANDLER_H
#define _INTERRUPT_HANDLER_H

#include <Arduino.h>
#include "pin_handler.h"
#include "var_handler.h"


// Interruption Variables
// Sample
volatile int16_t ADC_read;
hw_timer_t * ADC_timer = NULL;

// DATA
volatile uint16_t data[SAMPLE_AMOUNT];
volatile uint16_t data_index=0;

// Interface
volatile bool button_activated = false;
uint32_t debounce = 0;          // For BUTTON debounce time

// --------------------------------------------------------

// ADC sample interruption
void IRAM_ATTR onADC_timer() {
  ADC_read = analogRead(ADC_PIN);
  data[data_index]=ADC_read;
  data_index++;
  if (data_index==SAMPLE_AMOUNT){
    timerAlarmDisable(ADC_timer);
  }
}

// Button interruption function
void IRAM_ATTR onBUTTON_press() {
  detachInterrupt(digitalPinToInterrupt(BUTTON_PIN1));
  button_activated = true;
}

// --------------------------------------------------------
// Initialize interruptions
void interrupt_init(){
  // Button interruption
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN1), &onBUTTON_press, FALLING);
  
  // SAMPLING TIMER
  ADC_timer = timerBegin(1, 80, true);
  timerAttachInterrupt(ADC_timer, &onADC_timer, true);
  timerAlarmWrite(ADC_timer, SAMPLE_TIME_MICROS, true);

  while(millis()<5000);
  
  //timerAlarmEnable(ADC_timer);
}

#endif