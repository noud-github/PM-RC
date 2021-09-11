/*
  PM-RC.cpp - Library for Emulating A playmobil RC car BLE.
  Created by Arnoud Thorig.
  Released into the public domain.
*/

#include "Arduino.h"
#include "PMRC.h"
#include <ESP32Servo.h>
#include "driver/mcpwm.h"

PMRC::PMRC(String name)
{

 
  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, GPIO_PWM0A_OUT);
 

  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, GPIO_PWM0B_OUT); 
  
 
  mcpwm_config_t pwm_config;
 
  pwm_config.frequency = 500;                         
  pwm_config.cmpr_a = 0;                               
  pwm_config.cmpr_b = 0;                                
  pwm_config.counter_mode = MCPWM_UP_COUNTER;           
  pwm_config.duty_mode = MCPWM_DUTY_MODE_0;             
  //Inicia(Unidade 0, Timer 0, Config PWM)
  mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config); 

  
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
      //digitalWrite(LED_REAR_LEFT_RED, HIGH); 
      //digitalWrite(LED_REAR_RIGHT_RED, HIGH);
      
    } else {
      Serial.println("light_off");
      digitalWrite(LED_BUILTIN, LOW);
      //digitalWrite(LED_REAR_LEFT_RED, LOW);
      //digitalWrite(LED_REAR_RIGHT_RED, LOW);
      
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
        maxPower = 40; 
        // 50%
        break;
      case 2:
        maxPower = 55; 
        // 62%
        break;
      case 3:
        maxPower = 70; 
        // 75%
        break;
      case 4:
        maxPower = 85; 
        // 87%
        break;
      case 5:
        maxPower = 100;
        //100%
        break;
    }
    // Set speed -> PWM duty 0-100
    _power = map(value, 0, 128, 10, maxPower);
    
    if (Stop) {
      Serial.println("STOP");
      brushed_motor_stop(MCPWM_UNIT_0, MCPWM_TIMER_0);
      //ledcWrite(2, 0);
      
    } else {
      Serial.print("forward: ");
      Serial.print(_forward);
      if (_forward){
        brushed_motor_forward(MCPWM_UNIT_0, MCPWM_TIMER_0, _power);
      } else {
        brushed_motor_backward(MCPWM_UNIT_0, MCPWM_TIMER_0, _power);
      }
      Serial.print(" new Power: ");
     
      
      Serial.println(_power);
    }
  }
}




void PMRC::brushed_motor_forward(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num , float duty_cycle)
{
    mcpwm_set_signal_low(mcpwm_num, timer_num, MCPWM_OPR_B);
    mcpwm_set_duty(mcpwm_num, timer_num, MCPWM_OPR_A, duty_cycle);
    mcpwm_set_duty_type(mcpwm_num, timer_num, MCPWM_OPR_A, MCPWM_DUTY_MODE_0);
}
 
void PMRC::brushed_motor_backward(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num , float duty_cycle)
{
    mcpwm_set_signal_low(mcpwm_num, timer_num, MCPWM_OPR_A);
    mcpwm_set_duty(mcpwm_num, timer_num, MCPWM_OPR_B, duty_cycle);
    mcpwm_set_duty_type(mcpwm_num, timer_num, MCPWM_OPR_B, MCPWM_DUTY_MODE_0);
}
 
void PMRC::brushed_motor_stop(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num)
{
    mcpwm_set_signal_low(mcpwm_num, timer_num, MCPWM_OPR_A); //Desliga o sinal do MCPWM no Operador A
    mcpwm_set_signal_low(mcpwm_num, timer_num, MCPWM_OPR_B); //Desliga o sinal do MCPWM no Operador B
    
}


void PMRC::onDiscconect()
{
  brushed_motor_stop(MCPWM_UNIT_0, MCPWM_TIMER_0);
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
