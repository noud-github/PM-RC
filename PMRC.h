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



#ifndef PMRC_h
#define PMRC_h

#ifndef GPIO_PWM0A_OUT
#define GPIO_PWM0A_OUT 26   
#endif

#ifndef GPIO_PWM0B_OUT
#define GPIO_PWM0B_OUT 27   
#endif




#include <ESP32Servo.h>
#include "RCBSI.h"

class PMRC
{
  public:
    PMRC(String name);
    void setLight(bool value);
    void setSpeed(byte value);
    String getName();
    void setSteering(byte value);
    void setMotor(byte value);
    void onDisconnect();
    void onConnect();
    
  private:
    String _name;
    byte _speed;
    byte _power;
    Servo _myservo;
    RCBSI *_mybsi;
    byte _steering;
    byte _motor;
    bool _forward;
    void brushed_motor_forward(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num , float duty_cycle);
    void brushed_motor_backward(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num , float duty_cycle);
    void brushed_motor_stop(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num);

    
};

#endif
