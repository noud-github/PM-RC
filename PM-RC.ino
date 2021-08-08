/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleWrite.cpp
    Ported to Arduino ESP32 by Evandro Copercini
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "bc2f4cc6-aaef-4351-9034-d66268e328f0"
#define CHARACTERISTIC_UUID "06d1e5e7-79ad-4a71-8faa-373789f7d93c"

#define DPRINTB16(Num) for (unsigned int t = 0x8000; t; t >>= 1) Serial.write(Num  & t ? '1' : '0'); // Prints a 16 bit binary number with leading zeros

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {
        Serial.println("*********");
        Serial.print("New value: ");
        /*for (int i = 0; i < value.length(); i++){
          Serial.print(value[i],HEX);
          Serial.print("-");
          //byte val = value[i];
          //DPRINTB16(val);
        }
        */
        switch (value[0]){
          case 0x40:
            Serial.print ("Stering: ");
            break;
          case  0x24:
            Serial.print ("light: ");
            break;
          case  0x23:
            Serial.print ("motor: ");
            break;
          case  0x25:
            Serial.print ("speed: ");
            break;
          default:
            Serial.print(value[0],HEX);
            Serial.print("-");
        
          
        }
        Serial.print(value[1],HEX);
        Serial.println();
        Serial.println("*********");
      }
    }
};

void setup() {
  Serial.begin(115200);
  Serial.println("5- See the magic =)");
  
  BLEDevice::init("PM-RC ESP32");
  BLEServer *pServer = BLEDevice::createServer();

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
  pAdvertising->start();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
}
