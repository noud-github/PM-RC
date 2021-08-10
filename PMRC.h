/*
  PMRC.h - Library for Emulating a Playmobil RC cars BLE.
  Created by Arnoud Thoerig
  Released into the public domain.
*/

#ifndef PMRC_h
#define PMRC_h



class PMRC
{
  public:
    PMRC(String name);
    void setLight(bool value);
    bool getLight();
    void setSpeed(byte value);
    void dash();
    String getName();
    
  private:
    String _name;
    bool _lightOn;
    byte _speed;
};

#endif
