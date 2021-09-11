
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
byte currentval = 0x7F;

TaskHandle_t task1Handle = NULL;


PMRC pmrc("ESP32");
BLECharacteristic* pCharacteristic = NULL;


void toggleLED(void * parameter){
  static bool onof = false;
  for(;;){ // infinite loop

    // Turn the LED on
    digitalWrite(LED_REAR_LEFT_RED, onof); 
    digitalWrite(LED_REAR_RIGHT_RED, !onof);
    onof = !onof;
 
    // Pause the task for 500ms
    vTaskDelay(250 / portTICK_PERIOD_MS);

   
  }
}


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      pmrc.setSteering(0x7F);
      pmrc.setLight(false);
      //uint8_t dataValue[] = {0x25,0x1,0xFF}; 
      //pCharacteristic->setValue(dataValue, sizeof(dataValue));
      //pCharacteristic->notify();
      digitalWrite(LED_REAR_LEFT_RED, LOW); 
      digitalWrite(LED_REAR_RIGHT_RED, LOW);
      if(task1Handle != NULL) {
        vTaskDelete(task1Handle);
      }
      
    };
    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      pmrc.setSteering(0x7F);
      pmrc.setLight(false);
      pmrc.onDiscconect();
      digitalWrite(LED_REAR_LEFT_RED, LOW); 
      digitalWrite(LED_REAR_RIGHT_RED, LOW);
      xTaskCreate(
        toggleLED,    // Function that should be called
        "Toggle LED",   // Name of the task (for debugging)
        1000,            // Stack size (bytes)
        NULL,            // Parameter to pass
        1,               // Task priority
        &task1Handle     // Task handle
      );
      
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
  pinMode(LED_REAR_LEFT_RED, OUTPUT);
  pinMode(LED_REAR_RIGHT_RED, OUTPUT);
  
  pinMode(17, OUTPUT);
  Serial.begin(115200);
  Serial.println(pmrc.getName());
  
  BLEDevice::init("PM-RC ESP32");
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

  xTaskCreate(
    toggleLED,    // Function that should be called
    "Toggle LED",   // Name of the task (for debugging)
    1000,            // Stack size (bytes)
    NULL,            // Parameter to pass
    1,               // Task priority
    &task1Handle             // Task handle
  );

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
