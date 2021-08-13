/*
  PM-RC.cpp - Library for Emulating A playmobil RC car BLE.
  Created by Arnoud Thorig.
  Released into the public domain.
*/

#include "Arduino.h"
#include "PMRC.h"
#include <ESP32Servo.h>

PMRC::PMRC(String name)
{
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  _myservo.setPeriodHertz(50);    // standard 50 hz servo
  _myservo.attach(23, 600, 2500); // attaches the servo on pin 18 to the servo object
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
    Serial.print("new Speed: ");
    Serial.println(value, HEX);
  }
}

void PMRC::setSteering(byte value)
{
  if ( _steering != value ) {
    _steering = value;
    int pos = map(value, 0, 0xFF, 45, 135);     // scale it to use it with the servo (value between 0 and 180)
    _myservo.write(pos); // set the servo position according to the scaled valu
    Serial.print("new Angle: ");
    Serial.println(pos);
  }
}

void PMRC::setMotor(byte value)
{
  if ( _motor != value ) {
    _motor = value;
    int power = _motor * _speed;
    Serial.print("new Power: ");
    Serial.println(power);
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
