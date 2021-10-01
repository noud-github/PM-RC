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

class RCBSICallbacks;

typedef struct {
  bool on;
  byte inten;
  bool blink;
  } LED_STATE_STRUCT;


class RCBSI
{
  public:
    void setCallbacks(RCBSICallbacks* pCallbacks); 
    RCBSI(std::string name);
    //LED_STATE_STRUCT LED1;
    void blinkRearLEDs(bool value);
    void lightOn(bool value);
    std::string getName();
      
  private:
    std::string _name;
    static void toggleRearLED(void * parameter);
    TaskHandle_t task1Handle;
    bool _lightOn;
    bool _blinkRearLEDs;
    RCBSICallbacks* m_pRCBSICallbacks = nullptr;
      
};

class RCBSICallbacks {
public:
  virtual ~RCBSICallbacks() {};
  
  virtual void onConnect(RCBSI* pServer);
  
  virtual void onDisconnect(RCBSI* pServer);

}; // RCBSICallbacks




#endif
