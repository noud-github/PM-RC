/*
  PM-RC.cpp - Library for Emulating A playmobil RC car BLE.
  Created by Arnoud Thorig.
  Released into the public domain.
*/

#include "Arduino.h"
#include "PMRC.h"

PMRC::PMRC(String name)
{
 
  _name = name;
  _lightOn = false;
  _speed = 0x3;
}

void PMRC::setLight(bool value)
{
  if (_lightOn != value){
    if (value) { 
      Serial.println("light_on"); 
      digitalWrite(2, HIGH); 
    } else {
      Serial.println("light_off");
      digitalWrite(2, LOW);
    }
    _lightOn = value;
  }
}

void PMRC::setSpeed(byte value)
{
  if ( _speed != value ) {
    _speed = value;
    Serial.print("newspeed: ");
    Serial.println(value, HEX);
  }
}



void PMRC::dash()
{
  digitalWrite(2, HIGH);
  delay(1000);
  digitalWrite(2, LOW);
  delay(250);
}

String PMRC::getName() {
  return "PM-RC " + _name;
} // getValue

bool PMRC::getLight() {
  return _lightOn;
}
