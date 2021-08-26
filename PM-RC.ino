
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>





#define SERVICE_UUID        "bc2f4cc6-aaef-4351-9034-d66268e328f0"
#define CHARACTERISTIC_UUID "06d1e5e7-79ad-4a71-8faa-373789f7d93c"
#define LED_BUILTIN 2
#define SERVOPIN 23
// Motor A
#define motor1Pin1 27
#define motor1Pin2 26
#define enable1Pin 14 

#include "PMRC.h"

volatile boolean deviceConnected = false;
byte currentval = 0x7F;

PMRC pmrc("ESP32");
BLECharacteristic* pCharacteristic = NULL;


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      pmrc.setSteering(0x7F);
      pmrc.setLight(false);
      //uint8_t dataValue[] = {0x25,0x1,0xFF}; 
      //pCharacteristic->setValue(dataValue, sizeof(dataValue));
      //pCharacteristic->notify();
  
      
    };
    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      pmrc.setSteering(0x7F);
      pmrc.setLight(false);
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
              if (pmrc.getSpeed() != 1) {
                pCharacteristic->notify (); 
              }
              pmrc.setLight(value[1] == 0x2);
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
  pinMode(17, OUTPUT);
  pinMode(14, OUTPUT);
  //set forward
  digitalWrite(14, HIGH);
  /*
  digitalWrite(17, HIGH);
  delayMicroseconds(800);
  digitalWrite(17, LOW);
  delayMicroseconds(800); 
  digitalWrite(17, LOW);
  */
  Serial.begin(115200);
  Serial.println(pmrc.getName());
  
  BLEDevice::init("PM-RC ESP32");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);
  /*
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_NOTIFY |
                                         BLECharacteristic::PROPERTY_WRITE |
                                         BLECharacteristic::PROPERTY_WRITE_NR
                                       );

*/
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
    pmrc.setLight(onof);
    onof = !onof;
  }
  
  delay(500);  
  
}
