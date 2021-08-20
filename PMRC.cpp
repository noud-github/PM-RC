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
  // Setting PWM properties
  const int freq = 30000;
  const int pwmChannel = 2;
  const int resolution = 8;
  int dutyCycle = 200;
  // sets the pins as outputs:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  
  // configure LED PWM functionalitites
  ledcSetup(pwmChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(enable1Pin, pwmChannel);


  
  ESP32PWM::allocateTimer(0);
  //ESP32PWM::allocateTimer(1);
  //ESP32PWM::allocateTimer(2);
  //ESP32PWM::allocateTimer(3);
  _myservo.setPeriodHertz(50);    // standard 50 hz servo
  _myservo.attach(SERVOPIN, 600, 2500); // attaches the servo on pin 18 to the servo object
  _name = name;
  _lightOn = false;
  _speed = 0x3;
  _power = 0;
  _forward = true;
}

void PMRC::setLight(bool value)
{
  if (_lightOn != value){
    if (value) { 
      Serial.println("light_on"); 
      digitalWrite(LED_BUILTIN, HIGH); 
    } else {
      Serial.println("light_off");
      digitalWrite(LED_BUILTIN, LOW);
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
    setMotor(_motor);
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
  static byte localspeed = _speed;
  if ( _motor != value || localspeed != _speed) {
    _motor = value;
    localspeed = _speed;
    _forward = (value >= 127)? true: false;
    bool Stop = (value == 127)? true: false;
    value = (_forward)?  value - 127:  abs(value - 127)+1;
    byte maxPower = 0;
    Serial.print("val: ");
    Serial.println(value);
    switch (_speed){
      case 1:
        maxPower = 128; 
        // 50%
        break;
      case 2:
        maxPower = 160; 
        // 62%
        break;
      case 3:
        maxPower = 192; 
        // 75%
        break;
      case 4:
        maxPower = 224; 
        // 87%
        break;
      case 5:
        maxPower = 255;
        //100%
        break;
    }
    _power = map(value, 0, 128, 0, maxPower);
    
    if (Stop) {
      Serial.println("STOP");
      ledcWrite(2, 0);
      
    } else {
      Serial.print("forward: ");
      Serial.print(_forward);
      
      Serial.print(" new Power: ");
      ledcWrite(2, _power);
      Serial.println(_power);
    }
  }
}


void PMRC::dash()
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(250);
}

String PMRC::getName() {
  return "PM-RC " + _name;
} // getValue

bool PMRC::getLight() {
  return _lightOn;
}
