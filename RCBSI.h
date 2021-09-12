#ifndef RCBSI_h
#define RCBSI_h

#ifndef  LED_REAR_LEFT_RED
#define LED_REAR_LEFT_RED 22
#endif

#ifndef LED_REAR_RIGHT_RED
#define LED_REAR_RIGHT_RED 21
#endif

#ifndef LED_BUILDIN
#define LED_BUILTIN 2
#endif



typedef struct {
  bool on;
  byte inten;
  bool blink;
  } LED_STATE_STRUCT;



class RCBSI
{
  public:
    RCBSI(String name);
    //LED_STATE_STRUCT LED1;
    void blinkRearLEDs(bool value);
    void lightOn(bool value);
      
  private:
    String _name;
    static void toggleRearLED(void * parameter);
    TaskHandle_t task1Handle;
    bool _lightOn;
    bool _blinkRearLEDs;

  
    
};

#endif
