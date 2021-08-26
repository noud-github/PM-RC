/*
  PMRC.h - Library for Emulating a Playmobil RC cars BLE.
  Created by Arnoud Thoerig
  Released into the public domain.
*/

#include "driver/mcpwm.h"

#ifndef SERVOPIN 
#define SERVOPIN 23
#endif

#ifndef LED_BUILDIN
#define LED_BUILTIN 2
#endif

#ifndef motor1Pin1
#define motor1Pin1 27
#endif

#ifndef motor1Pin2 
#define motor1Pin2 26
#endif

#ifndef enable1Pin 
#define enable1Pin 14 
#endif

#ifndef PMRC_h
#define PMRC_h


#include <ESP32Servo.h>

class PMRC
{
  public:
    PMRC(String name);
    void setLight(bool value);
    bool getLight();
    void setSpeed(byte value);
    void dash();
    String getName();
    void setSteering(byte value);
    void setMotor(byte value);
    byte getSpeed();

    
  private:
    String _name;
    bool _lightOn;
    byte _speed;
    byte _power;
    Servo _myservo;
    byte _steering;
    byte _motor;
    bool _forward;
   
    
};

#endif
