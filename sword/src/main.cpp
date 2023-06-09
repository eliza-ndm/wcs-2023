#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include <SPI.h>
#include <NeoPixelBus.h>

// REPLACE WITH THE RECEIVER'S MAC Address
uint8_t broadcastAddress[] = {0x94, 0xB5, 0x55, 0x77, 0xE4, 0xE8};

const uint16_t PixelCount = 48*2; // this example assumes 4 pixels, making it smaller will cause a failure
const uint8_t PixelPin = 14;  // make sure to set this to the correct pin, ignored for Esp8266
// three element pixels, in different order and speeds
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);

#define colorSaturation 128


void ledStrip(int startIndex)
{
    //int ledColourPattern[] = {50,0,50,100,200,100};
    int ledColourPattern[] = {25,0,25,50,100,50};
    int m = 0;
    if (startIndex > 5) {
      m = 0;
    }else{
      m = startIndex;
    }
    Serial.println(PixelCount/2);
    for (int i = 0; i <= PixelCount/2; i++) {
      Serial.println("iiiiiiii");
      Serial.println(i);
      Serial.println(ledColourPattern[m]);
      RgbColor pixel(ledColourPattern[m],ledColourPattern[m],200);
      strip.SetPixelColor(i, pixel);
      strip.SetPixelColor(PixelCount-i, pixel);
      
      Serial.println(m);
      if (m == 5){
        m = 0;
      } else {
        m = m +1;
      }
    }
    strip.Show();
    

}

void ledStripActivate(int startIndex)
{
    //int ledColourPattern[] = {50,0,50,100,200,100};
    int ledColourPattern[] = {25,0,25,50,100,50};
    int m = 0;
    if (startIndex > 5) {
      m = 0;
    }else{
      m = startIndex;
    }
    Serial.println(PixelCount/2);
    for (int i = 0; i <= PixelCount/2; i++) {

      RgbColor pixel(ledColourPattern[m],ledColourPattern[m],200);
      strip.SetPixelColor(i, pixel);
      strip.SetPixelColor(PixelCount-i, pixel);
      strip.Show();
      Serial.println(m);
      delay(50); 
      if (m == 5){
        m = 0;
      } else {
        m = m +1;
      }
      
    }
    
    

}


void ledStripDark()
{

    for (int i = 0; i <= PixelCount; i++) {
      RgbColor pixel(0,0, 0);
      strip.SetPixelColor(i, pixel);
      
    }
    strip.Show();
    

}
// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
    int id; // must be unique for each sender board
    int x;
    int y;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Create peer interface
esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }


  pinMode(2, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  strip.Begin();
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  
}

int ledI = 0;
int ledActive = 0;

unsigned long lastTrig = 0;

void loop() {

  //wifi button
  int buttonState = digitalRead(2);
  //turn on led button
  int ledButton = digitalRead(15);
  Serial.println(buttonState);
  Serial.println(ledButton);
  //delay(500);

  if (ledButton){
    Serial.println("not pressed");
  }else{
    Serial.println("current");
    Serial.println(ledActive);
    if (ledActive){
      ledActive = 0;
      ledStripDark();
      delay(500);

    } else {
      ledActive = 1;
      ledStripActivate(0);
    }
    Serial.println("NEW");
    Serial.println(ledActive); 

  }

  if (ledActive){
    if (ledI >= 5){
      ledI = 0;
    }else {
      ledI = ledI +1;
    }
    ledStrip(ledI);
  }

  if (buttonState){
    Serial.println("A");
      myData.id = 63;
      myData.x = 0;
      myData.y = 0;
  }else{
    Serial.println("B");
      myData.id = 63;
      myData.x = 2;
      myData.y = 1;

    if (millis() - lastTrig >= 2000){
                // Send message via ESP-NOW
      esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
      
      if (result == ESP_OK) {
        Serial.println("Sent with success");
        lastTrig = millis();
      }
      else {
        Serial.println("Error sending the data");
      }
      delay(100);

    }

  }


}

