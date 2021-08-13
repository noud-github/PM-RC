/*
  PMRC.h - Library for Emulating a Playmobil RC cars BLE.
  Created by Arnoud Thoerig
  Released into the public domain.
*/

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
    
  private:
    String _name;
    bool _lightOn;
    byte _speed;
    Servo _myservo;
    byte _steering;
    byte _motor;
};

#endif
