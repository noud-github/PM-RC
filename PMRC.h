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

#define GPIO_PWM0A_OUT 12   //Declara GPIO 12 como PWM0A
#define GPIO_PWM0B_OUT 14   //Declara GPIO 14 como PWM0B


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
    byte _power;
    Servo _myservo;
    byte _steering;
    byte _motor;
    bool _forward;
    void brushed_motor_forward(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num , float duty_cycle);
    void brushed_motor_backward(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num , float duty_cycle);
    void brushed_motor_stop(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num);

    
};

#endif
