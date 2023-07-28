#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <HardwareSerial.h>

HardwareSerial SerialPort(2); // use UART2
uint8_t broadcastAddress[] = {0xC4,0x5B,0xBE,0x3F,0x4C,0xD8};
esp_now_peer_info_t peerInfo;

typedef struct struct_message {
  char data;
} struct_message;

struct_message mydata;

//Receiving
void OnDataRecv(const uint8_t* mac, const uint8_t* incomingData, int len) {
  memcpy(&mydata, incomingData, sizeof(mydata));
  SerialPort.write(mydata.data);
}

void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t _status)
{
	Serial.println(_status == ESP_NOW_SEND_SUCCESS ? "Send OK" : "Send Fail");
}

void setup(){
  Serial.begin(9600);
  SerialPort.begin(9600, SERIAL_8N1, 17, 16);


  WiFi.mode(WIFI_STA);
	Serial.println(WiFi.macAddress());

  if(esp_now_init() != ESP_OK){
    Serial.println("error initialisation");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
  esp_now_register_send_cb(onDataSent);

	memcpy(peerInfo.peer_addr, broadcastAddress, 6);
	peerInfo.channel = 0;
	peerInfo.encrypt = false;

	if (esp_now_add_peer(&peerInfo) != ESP_OK){
		Serial.println("Failed to add peer");
		return;
	}
}

char input;
void loop(){
  if (Serial.available()){
    input = char(Serial.read());
    Serial.printf("recv %c \n", input);
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&input, sizeof(mydata)); 	
  }
}