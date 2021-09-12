#include "Arduino.h"
#include "RCBSI.h"


RCBSI::RCBSI(String name)
{
 _name = name;
 pinMode(LED_REAR_LEFT_RED, OUTPUT);
 pinMode(LED_REAR_RIGHT_RED, OUTPUT);
 _blinkRearLEDs = false;
 _lightOn = false;
 this->blinkRearLEDs(true);
 
}

void RCBSI::lightOn(bool value)
{
  if (_lightOn != value){
      if (value) { 
        Serial.println("light_on"); 
        digitalWrite(LED_BUILTIN, HIGH); 
        digitalWrite(LED_REAR_LEFT_RED, HIGH); 
        digitalWrite(LED_REAR_RIGHT_RED, HIGH);
        
      } else {
        Serial.println("light_off");
        digitalWrite(LED_BUILTIN, LOW);
        digitalWrite(LED_REAR_LEFT_RED, LOW);
        digitalWrite(LED_REAR_RIGHT_RED, LOW);
        
      }
      _lightOn = value;
    
  }
}


void RCBSI::blinkRearLEDs(bool value)
{
  if (_blinkRearLEDs != value){
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
    _blinkRearLEDs = value;
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
