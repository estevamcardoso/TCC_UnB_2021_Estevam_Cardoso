#ifndef _VAR_HANDLER_H
#define _VAR_HANDLER_H
#include "Arduino.h"

// Time definitions
#define SAMPLE_TIME_MICROS 500    // Sample time micros (500 = 2kHz)
#define DEBOUNCE_TIME_MILLIS 1000 // Debounce time millis

// Data definitions
#define SAMPLE_AMOUNT 2000 * 6  // Samples to save

// Files definitions
#define FILES_PER_EMAIL 1 // How many files send via email at same time
uint8_t file_count = 0;
uint8_t file_to_send = 0;
char file_name[10];

// Degub flag
#define DEBUG false

#endif