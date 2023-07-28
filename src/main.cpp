#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <HardwareSerial.h>

HardwareSerial SerialPort(2); // use UART2


typedef struct struct_message {
  char data;
} struct_message;

struct_message mydata;

//Receiving
void OnDataRecv(const uint8_t* mac, const uint8_t* incomingData, int len) {
  memcpy(&mydata, incomingData, sizeof(mydata));
  SerialPort.write(mydata.data);
}

void setup(){
  Serial.begin(9600);
  SerialPort.begin(9600, SERIAL_8N1, 17, 16);


  WiFi.mode(WIFI_STA);

  if(esp_now_init() != ESP_OK){
    Serial.println("error initialisation");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}

void loop(){

}