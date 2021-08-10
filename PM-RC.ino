/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleWrite.cpp
    Ported to Arduino ESP32 by Evandro Copercini
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include "PMRC.h"


#define SERVICE_UUID        "bc2f4cc6-aaef-4351-9034-d66268e328f0"
#define CHARACTERISTIC_UUID "06d1e5e7-79ad-4a71-8faa-373789f7d93c"



#define LED_BUILTIN 2

volatile boolean deviceConnected = false;
volatile boolean setLightOn = false;
boolean lightOn = false;
volatile byte setSpeed = 0x3;
byte speed = 0x3;

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
              if ( value[1] == 0x2 ){
                setLightOn = true;
                pmrc.setLight(true);
              } else {
                setLightOn = false;
                pmrc.setLight(false);
              } 
            }
            break;
          case  0x23:
            Serial.print ("motor: ");
            break;
          case  0x25:
            Serial.print ("speed: ");
            if (deviceConnected) { 
              pCharacteristic->notify();  
              setSpeed =  value[1];
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
  //pAdvertising->setScanResponse(true);
  //pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  //pAdvertising->setMinPreferred(0x12);
 ;
  pAdvertising->start();

}



void loop() {
  // put your main code here, to run repeatedly:
  /*
  if (lightOn != setLightOn){
    if (setLightOn) {
      digitalWrite(LED_BUILTIN, HIGH);  
      Serial.println("light_on");
      
    } else {
      digitalWrite(LED_BUILTIN, LOW);
      Serial.println("light_off");
    }
    lightOn = setLightOn;
  }
  */
  if ( setSpeed != speed ) {
    Serial.print("speed: ");
    Serial.println(setSpeed,HEX);
    speed = setSpeed;
  }

  delay(500);
}
