
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include "PMRC.h"
#include <ESP32Servo.h>

#define SERVICE_UUID        "bc2f4cc6-aaef-4351-9034-d66268e328f0"
#define CHARACTERISTIC_UUID "06d1e5e7-79ad-4a71-8faa-373789f7d93c"
#define LED_BUILTIN 2

volatile boolean deviceConnected = false;
byte currentval = 0x7F;

PMRC pmrc("ESP32");


int pos = 0;    // variable to store the servo position
// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33 
int servoPin = 23;


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      pmrc.setSteering(0x7F);
      pmrc.setLight(false);
      
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
              pCharacteristic->notify(); 
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
  Serial.begin(115200);
  Serial.println(pmrc.getName());
  
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
  /*
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);    // tell servo to go to position in variable 'pos'
    delay(15);             // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);    // tell servo to go to position in variable 'pos'
    delay(15);             // waits 15ms for the servo to reach the position
  */
  delay(2000);
  
  //delay(20);                          // wait for the servo to get there
  
}
