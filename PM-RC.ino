
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include "PMRC.h"


#define SERVICE_UUID        "bc2f4cc6-aaef-4351-9034-d66268e328f0"
#define CHARACTERISTIC_UUID "06d1e5e7-79ad-4a71-8faa-373789f7d93c"
#define LED_BUILTIN 2

volatile boolean deviceConnected = false;

PMRC pmrc("ESP32");

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };
    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {
        //Serial.println("*********");
        //Serial.print("New value: ");
        switch (value[0]){
          case 0x40:
            //Serial.print ("Stering: ");
            break;
          case  0x24:
            Serial.print ("light: ");
            if (deviceConnected) { 
              pCharacteristic->notify(); 
              pmrc.setLight(value[1] == 0x2);
            }
            break;
          case  0x23:
            Serial.print ("motor: ");
            break;
          case  0x25:
            Serial.print ("speed: ");
            if (deviceConnected) { 
              pCharacteristic->notify();  
              pmrc.setSpeed(value[1]);
            }
            break;
          default:
            Serial.print(value[0],HEX);
            Serial.print("-");
            break;
          
        }
        
        //Serial.print(value[1],HEX);
        //Serial.println();
        //Serial.println("*********");
        
      }
    }
};

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("5- See the magic =)");
  Serial.println(pmrc.getName());
  //digitalWrite(LED_BUILTIN, HIGH);
  BLEDevice::init("PM-RC ESP32");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_NOTIFY |
                                         BLECharacteristic::PROPERTY_WRITE |
                                         BLECharacteristic::PROPERTY_WRITE_NR
                                       );

  BLEDescriptor *pDescriptor1 = new BLEDescriptor((uint16_t)0x2902); // Characteristic Presentation Format
 
  pCharacteristic->addDescriptor(pDescriptor1);
                                       
  pCharacteristic->setCallbacks(new MyCallbacks());

  pCharacteristic->setValue("Hello World");
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->start();

}



void loop() {
  // put your main code here, to run repeatedly

  delay(2000);
}
