#include "Arduino.h"
#include "RCBSI.h"


RCBSI::RCBSI(String name)
{
 _name = name;
 pinMode(LED_REAR_LEFT_RED, OUTPUT);
 pinMode(LED_REAR_RIGHT_RED, OUTPUT);
 
}

void RCBSI::blinkRearLEDs(bool value)
{
  if (value){
    
    xTaskCreate(
    this->toggleRearLED,    // Function that should be called
    "Toggle LED",   // Name of the task (for debugging)
    1000,            // Stack size (bytes)
    NULL,            // Parameter to pass
    1,               // Task priority
    &task1Handle             // Task handle
  );

  } else {
    if(task1Handle != NULL) {
        vTaskDelete(task1Handle);
    }
    digitalWrite(LED_REAR_LEFT_RED, LOW); 
    digitalWrite(LED_REAR_RIGHT_RED, LOW);  
  }
  
}

void RCBSI::toggleRearLED(void * parameter){
  bool onof = false;
  for(;;){ // infinite loop
    // Turn the LED on
    digitalWrite(LED_REAR_LEFT_RED, onof); 
    digitalWrite(LED_REAR_RIGHT_RED, !onof);
    onof = !onof;
    // Pause the task for 500ms
    vTaskDelay(250 / portTICK_PERIOD_MS);
  }
}
