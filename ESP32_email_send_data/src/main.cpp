/* 
*   Engenharia Mecatrônica UnB - 2021
*   Trabalho de conclusão de curso
*   Estevam Cardoso Lopes da Silva
*   
*   Código implementado para efetuar 
*   leitura dos sinal eletromiográficos
*   e persistência dos dados via email
*   
*   Estrutura de código e comentários
*   escritos em inglês
*   
*/
#include <Arduino.h>
#include <LITTLEFS.h>
#include "save_samples.h"
#include "pin_handler.h"
#include "interrupt_handler.h"
#include "email_handler.h"
#include "var_handler.h"

void setup()
{
  // Pin mode assignment
  pinAssignment();
#if DEBUG == true
  // Initialize Serial
  Serial.begin(115200);
  while (!Serial)
    ;
#endif
  // Initialize LITTLEFS
  if (!FS_init())
  {
#if DEBUG == true
    Serial.println("LFS problem, stoping execution");
#endif

    while (true)
    {
      led2_toggle = !led2_toggle;
      digitalWrite(LED_PIN2, led2_toggle);
      delay(200);
    }
  }
  // Initialize interruptions
  interrupt_init();
  digitalWrite(LED_PIN1, false);
  digitalWrite(LED_PIN2, true);
#if DEBUG == true
  Serial.println("Press Button 1 to start/stop sampling!!...");
#endif
}

void loop()
{

  if (button_activated == true)
  { // Button debounce
    debounce = millis();
    button_activated = false;
#if DEBUG == true
    Serial.println("Button pressed, start/stop sampling in 1 sec...");
#endif
  }
  if (debounce > 0)
  { // Reattaching Button Interruption
    if (millis() - debounce > DEBOUNCE_TIME_MILLIS)
    {
      attachInterrupt(digitalPinToInterrupt(BUTTON_PIN1), &onBUTTON_press, FALLING);
      debounce = 0;
      if (!timerAlarmEnabled(ADC_timer)) // If timer not Enabled
      {
        data_index = 0;
        digitalWrite(LED_PIN1, true);
        digitalWrite(LED_PIN2, false);
        timerAlarmEnable(ADC_timer); // Enable it
      }
      else
      {
        data_index = 0;
        digitalWrite(LED_PIN1, false);
        digitalWrite(LED_PIN2, true);
        timerAlarmDisable(ADC_timer); // Else, disable it
      }
    }
  }
  // When enough samples to store in file, then do it
  if (data_index == SAMPLE_AMOUNT)
  {
    digitalWrite(LED_PIN1, false);
    digitalWrite(LED_PIN2, true);
    // To prevent error, button disabled
    detachInterrupt(digitalPinToInterrupt(BUTTON_PIN1));
    FS_save_samples(data); // Save samples to file system
    data_index = 0;
    if (file_count % FILES_PER_EMAIL == 0) // Send emails
    {
      digitalWrite(LED_PIN1, true);
      if (!connect_and_send()) // Try to send emails
      {
        while (true) // If something wrong, stop execution and blink red
        {
          digitalWrite(LED_PIN2, !digitalRead(LED_PIN2));
          delay(200);
        }
      }
      for (uint8_t i = 0; i < 7; i++)
      {
        digitalWrite(LED_PIN1, !digitalRead(LED_PIN1));
        delay(100);
      }
      digitalWrite(LED_PIN2, true);
    }
    else
    {
      timerAlarmEnable(ADC_timer); // start sampling again
      digitalWrite(LED_PIN1, true);
    }
    // Button enabled again
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN1), &onBUTTON_press, FALLING);
  }
}
