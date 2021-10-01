
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID        "bc2f4cc6-aaef-4351-9034-d66268e328f0"
#define CHARACTERISTIC_UUID "06d1e5e7-79ad-4a71-8faa-373789f7d93c"
#define LED_BUILTIN 2
#define SERVOPIN 23
#define GPIO_PWM0A_OUT 26   
#define GPIO_PWM0B_OUT 27
#define LED_REAR_LEFT_RED 22
#define LED_REAR_RIGHT_RED 21

#include "PMRC.h"

volatile boolean deviceConnected = false;

PMRC pmrc("ESP32");
BLECharacteristic* pCharacteristic = NULL;


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      pmrc.onConnect();   
    };
    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      BLEAdvertising *pAdvertising = pServer->getAdvertising();
      pAdvertising->start();
      pmrc.onDisconnect();
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      if (value.length() == 3 ) {
        switch (value[0]){
          case 0x40:
            //Serial.print ("Steering: ");
            if (deviceConnected) { 
              pmrc.setSteering(value[1]);
            }
            break;
          case  0x24:
            //Serial.print ("light: ");
            if (deviceConnected) { 
              pCharacteristic->notify(); 
              pmrc.lightOn(value[1] == 0x2);
            }
            break;
          case  0x23:
            //Serial.print ("motor: ");
            if (deviceConnected) {  
              pmrc.setMotor(value[1]);
            }
            break;
          case  0x25:
            //Serial.print ("speed: ");
            if (deviceConnected) { 
              pCharacteristic->notify();  
              pmrc.setSpeed(value[1]);
            }
            break;
          default:
            Serial.print(value[0],HEX);
            Serial.print("-");
            Serial.print(value[1],HEX);
            break;
          
        }       
      }
    }
};


void setup() {
  
  pinMode(LED_BUILTIN, OUTPUT);
  
  
  //pinMode(17, OUTPUT);
  Serial.begin(115200);
  Serial.println(pmrc.getName().c_str());
  
  BLEDevice::init(pmrc.getName());
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);
  
  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_NOTIFY |
    BLECharacteristic::PROPERTY_WRITE |
    BLECharacteristic::PROPERTY_WRITE_NR
  );

  BLEDescriptor *pDescriptor = new BLEDescriptor((uint16_t)0x2902); // Characteristic Presentation Format
 
  pCharacteristic->addDescriptor(pDescriptor);
                                       
  pCharacteristic->setCallbacks(new MyCallbacks());

  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();

  pAdvertising->addServiceUUID(SERVICE_UUID);
  
  pAdvertising->start();
  
}



void loop() {
  // put your main code here, to run repeatedly
  
  static bool onof = false;
  if (!deviceConnected) { 
    digitalWrite(LED_BUILTIN, onof);
    onof = !onof;
  } 
  
  delay(500);  
  
}
