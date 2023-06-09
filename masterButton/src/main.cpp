#include <esp_now.h>
#include <WiFi.h>



const int button1Pin = 35; // pin number for button 1
const int button2Pin = 32; // pin number for button 2
const int button3Pin = 33; // pin number for button 3
const int button4Pin = 25; // pin number for button 4
const int button5Pin = 13; // pin number for button 5

#define gunshot 1
#define slash 2
#define monsterMove 4
#define monsterExplode 3


// REPLACE WITH THE RECEIVER'S MAC Address
uint8_t broadcastAddress[] = {0x94, 0xB5, 0x55, 0x77, 0xE4, 0xE8};

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
  pinMode(button1Pin, INPUT_PULLUP); // set button 1 pin as input with internal pull-up resistor
  pinMode(button2Pin, INPUT_PULLUP); // set button 2 pin as input with internal pull-up resistor
  pinMode(button3Pin, INPUT_PULLUP); // set button 3 pin as input with internal pull-up resistor
  pinMode(button4Pin, INPUT_PULLUP); // set button 4 pin as input with internal pull-up resistor
  pinMode(button5Pin, INPUT_PULLUP); // set button 5 pin as input with internal pull-up resistor


  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }



  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
int incomingByte = 0; // for incoming serial data

void loop() {
  int buttonPressed = 0;

  // if (digitalRead(button1Pin) == LOW) { // check if button 1 is pressed
  //   Serial.println("Button 1 pressed"); // print message to serial monitor
  //   buttonPressed = 0;
  // }
  if (digitalRead(button2Pin) == LOW) { // check if button 2 is pressed
    Serial.println("Button 2 pressed"); // print message to serial monitor
    buttonPressed = monsterMove;
  }
  if (digitalRead(button3Pin) == LOW) { // check if button 3 is pressed
    Serial.println("Button 3 pressed"); // print message to serial monitor
    buttonPressed = slash;
  }
  if (digitalRead(button4Pin) == LOW) { // check if button 4 is pressed
    Serial.println("Button 4 pressed"); // print message to serial monitor
    buttonPressed = gunshot;
  }
  if (digitalRead(button5Pin) == LOW) { // check if button 5 is pressed
    Serial.println("Button 5 pressed"); // print message to serial monitor
    buttonPressed = monsterExplode;
  }

  if (buttonPressed > 0){
      
      // Set values to send
      myData.id = 63;
      myData.x = buttonPressed;
      myData.y = 0;
      Serial.println("SENDING"); // print message to serial monitor
      Serial.println(myData.x); // print message to serial monitor
      buttonPressed = 0;
    
      // Send message via ESP-NOW
      esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
      
      if (result == ESP_OK) {
        Serial.println("Sent with success");
        delay(1000);
      }
      else {
        Serial.println("Error sending the data");
      }
      delay(100);
      }
  delay(100); // wait a short time before checking buttons again
}


// void loop() {
//   // send data only when you receive data:
//   if (Serial.available() > 0) {
//     // read the incoming byte:
//     incomingByte = Serial.read();

//     // say what you got:
//     Serial.print("I received: ");
//     Serial.println(incomingByte, DEC);
//     int number = incomingByte - 48;
//     Serial.println(number, DEC);
    
//     }
// }
 
