#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include <SPI.h>

#include <FastLED.h>


#define MTR_TRIG_1 5
#define MTR_TRIG_2 22
#define MTR_TRIG_3 23
#define LED_CONTROL 19
#define BULLET_LED 2
#define EXPLODE 15
#define BULLET_HOLE_TRIG 18

const uint16_t PixelCount1 = 300; // this example assumes 4 pixels, making it smaller will cause a failure
const uint8_t PixelPin1 = LED_CONTROL;  // make sure to set this to the correct pin, ignored for Esp8266

const uint16_t PixelCount2 = 300; // this example assumes 4 pixels, making it smaller will cause a failure
const uint8_t PixelPin2 = BULLET_LED;  // make sure to set this to the correct pin, ignored for Esp8266

CRGB leds1[PixelCount1];
CRGB leds2[PixelCount2];

#define colorSaturation 128

// // three element pixels, in different order and speeds
// NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
// NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip2(PixelCount2, PixelPin2);

// RgbColor red(colorSaturation, 0, 0);
// RgbColor green(0, colorSaturation, 0);
// RgbColor blue(0, 0, colorSaturation);
// RgbColor white(colorSaturation);
// RgbColor black(0);

// HslColor hslRed(red);
// HslColor hslGreen(green);
// HslColor hslBlue(blue);
// HslColor hslWhite(white);
// HslColor hslBlack(black);

float prev = 0; 

void ledStrip(int status)
{
  CRGB colour = CRGB::Black;
  
  if (status == 1){
    colour = CRGB::Red;
  }

  for (int i = 0; i < PixelCount1; i++) {
    leds1[i] = colour;  // Set color of pixel in strip 1
    FastLED.show();

  }



}

void bulletHoleLed(int status){
  CRGB colour = CRGB::Black;
  
  if (status == 1){
    colour = CRGB::Red;
  }

  for (int i = 0; i < PixelCount1; i++) {
    leds2[i] = colour;  // Set color of pixel in strip 1
    FastLED.show();

  }
}


// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  int id;
  int x;
  int y;
}struct_message;

// Create a struct_message called myData
struct_message myData;

// Create a structure to hold the readings from each board
struct_message board1;
struct_message board2;
struct_message board3;

// Create an array with all the structures
struct_message boardsStruct[3] = {board1, board2, board3};

void swordSlash(){
  Serial.printf("SWORD TRIGGERRR");
  ledStrip(1);
  delay(500); 
  ledStrip(0);  
}

void bulletHole(){
  Serial.print("BULLET HOLE");
  digitalWrite(BULLET_HOLE_TRIG, HIGH); // sets the digital pin 13 on
  delay(100);            // waits for a second
  digitalWrite(BULLET_HOLE_TRIG, LOW);  // sets the digital pin 13 off
  delay(800);  
  bulletHoleLed(1);
  delay(500); 
  bulletHoleLed(0);  

}

void monsterMovOne(){
  digitalWrite(MTR_TRIG_1, HIGH); 
  delay(500);         
  digitalWrite(MTR_TRIG_1, LOW);  
}

void monsterMovTwo(){
  digitalWrite(MTR_TRIG_2, HIGH); 
  delay(500);         
  digitalWrite(MTR_TRIG_2, LOW);  
}

void monsterMovThree(){
  digitalWrite(MTR_TRIG_3, HIGH); 
  delay(500);         
  digitalWrite(MTR_TRIG_3, LOW);  
}

void explode(){
  digitalWrite(EXPLODE, HIGH); 
  delay(500);         
  digitalWrite(EXPLODE, LOW);  
}

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.printf("Board ID %u: %u bytes\n", myData.id, len);




  //check if packed is from approved device
  if (myData.id == 63){

    switch (myData.x) {
    case 1:
      Serial.println("1 trig");
      bulletHole();
      break;
    case 2:
      Serial.println("2 trig");
      swordSlash();
      break;
    case 3:
      Serial.println("3 trig");
      explode();
      break;
    case 4:
      Serial.println("4 trig");
      monsterMovOne();
      break;
    case 5:
      Serial.println("5 trig");
      monsterMovTwo();
      break;
    case 6:
      Serial.println("6 trig");
      monsterMovThree();
      break;
    default:
      // do nothing
      break;
    }
  }

  if (myData.id == 1){
    //sword

  }
}
 
void setup() {
  //Initialize Serial Monitor
  Serial.begin(115200);
  
  //Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  //Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);

  // strip.Begin();

  // strip2.Begin();
  FastLED.addLeds<NEOPIXEL, PixelPin1>(leds1, PixelCount1);
  FastLED.addLeds<NEOPIXEL, PixelPin2>(leds2, PixelCount2);

  //motor trigs
  pinMode(MTR_TRIG_1, OUTPUT);
  pinMode(MTR_TRIG_2, OUTPUT);
  pinMode(MTR_TRIG_3, OUTPUT);

  pinMode(EXPLODE, OUTPUT);
  pinMode(BULLET_HOLE_TRIG, OUTPUT);

}
 

void loop() {


	// for (pos = 0; pos <= 150; pos += 1) { // goes from 0 degrees to 180 degrees
	// 	// in steps of 1 degree
	// 	myservo.write(pos);    // tell servo to go to position in variable 'pos'
	// 	delay(2);             // waits 15ms for the servo to reach the position
	// }
  // delay(500); 
  // Serial.println("b");
	// for (pos = 150; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
	// 	myservo.write(pos);    // tell servo to go to position in variable 'pos'
	// 	delay(2);             // waits 15ms for the servo to reach the position
	// }
  // delay(10000); 
  // ledStrip(255, 250);
  // delay(500); 
  // ledStrip(0, 0);
}