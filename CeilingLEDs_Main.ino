/*
 * Title: CeilingLEDs_Main
 * 
 * Description: 
 *      - This sketch was created for an ESP8266 to control WS2812B LED light strip animations around the ceiling of a room. 
 *        SinricPro is used to enable Wi-Fi control of the animations with IFTTT and Google Home.
 *      - NOTE: A few parts of this sketch have values specifically calibrated for my room. So, to make use of this sketch, 
 *        follow comments and make changes as needed to fit your setup. Alternatively, you can copy-and-paste the parts that 
 *        are useful in order to build your own version of this sketch for your project.
 *
 * Author: Electriangle
 *      - Channel: https://www.youtube.com/@Electriangle
 *      - Ceiling LEDs Video: https://www.youtube.com/watch?v=_9XsQskPDV8
 *      
 *      - NOTE: Animation_Fireworks and Animation_Twinkle tabs contain code that was not written by Electriangle. Credit goes 
 *              to the respective authors mentioned within each tab header comment block.
 *
 * Date Created: 1/15/2021
 * Last Updated: 11/25/2022
*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "SinricPro.h"              // SinricPro library for Wi-Fi control
#include "SinricProLight.h"
#include <FastLED.h>                // FastLED library used for LED animations

#define WIFI_SSID         "YOUR_WIFI_SSID"    
#define WIFI_PASS         "YOUR_WIFI_PASS"
#define APP_KEY           "YOUR_SINRICPRO_APP_KEY"
#define APP_SECRET        "YOUR_SINRICPRO_APP_SECRET"
#define LIGHT_ID          "YOUR_SINRICPRO_LIGHT_ID"
#define BAUD_RATE         115200

const int LED_PIN_1 = 16;           // Output pin 1 for LED lights
const int LED_PIN_2 = 2;            // Output pin 2 for LED lights
const int NUM_LEDS_1 = 149 + 148 + 148 + 22;        // Total LEDs on first strand LED_PIN_1 = 467
const int NUM_LEDS_2 = 149 + 75;                    // Total LEDs on second strand LED_PIN_2 = 222
const int NUM_LEDS = NUM_LEDS_1 + NUM_LEDS_2;       // Total amount of LEDs being used
int val = 0;                        // Variable for reading the pin status

CRGB led1[NUM_LEDS_1];
CRGB led2[NUM_LEDS_2];

bool twinkle_animation = false;
bool firework_animation = false;
bool rainbow_animation = false;
bool fade_animation = false;
bool hallway_lights_on = false;
bool hallway_on = false;
bool movie_mode_on = false;
bool movie_on = false;

void disable_animations() {
  twinkle_animation = false;
  firework_animation = false;
  rainbow_animation = false;
  fade_animation = false;
  hallway_lights_on = false;
  movie_mode_on = false;
}

// define array of supported color temperatures
int colorTemperatureArray[] = {2200, 2700, 4000, 5500, 7000};  
int max_color_temperatures = sizeof(colorTemperatureArray) / sizeof(colorTemperatureArray[0]); // calculates how many elements are stored in colorTemperature array

// a map used to convert a given color temperature into color temperature index (used for colorTemperatureArray)
std::map<int, int> colorTemperatureIndex;

// initializes the map above with color temperatures and index values
// so that the map can be used to do a reverse search like
// int index = colorTemperateIndex[4000]; <- will result in index == 2
void setupColorTemperatureIndex() {
  Serial.printf("Setup color temperature lookup table\r\n");
  for (int i=0;i < max_color_temperatures; i++) {
    colorTemperatureIndex[colorTemperatureArray[i]] = i;
    Serial.printf("colorTemperatureIndex[%i] = %i\r\n", colorTemperatureArray[i], colorTemperatureIndex[colorTemperatureArray[i]]);
  }
}

// we use a struct to store all states and values for our light
struct {
  bool powerState = false;
  int brightness = 0;
  struct {
    byte r = 0;
    byte g = 0;
    byte b = 0;
  } color;
  int colorTemperature = colorTemperatureArray[0]; // set colorTemperature to first element in colorTemperatureArray array
} device_state; 

bool onPowerState(const String &deviceId, bool &state) {
  Serial.printf("Device %s power turned %s \r\n", deviceId.c_str(), state?"on":"off");
  device_state.powerState = state;

  if (state == 1) {
    Serial.println("on");
    corner_star(2, 20, 5);
  }
  if (state == 0) {
    Serial.println("off");
    FastLED.clear();
    FastLED.show();
  }
  disable_animations();
  return true; // request handled properly
}

bool onColorTemperature(const String &deviceId, int &colorTemperature) {
  device_state.colorTemperature = colorTemperature;
  disable_animations();
  FastLED.clear();
  FastLED.show();
  
  switch(colorTemperature) {
    case 0:
      twinkle_animation = true;
      Serial.println("Twinkle Mode ON");
      break;
    case 1:
      firework_animation = true;
      Serial.println("Firework Mode ON");
      break;
    case 2:
      rainbow_animation = true;
      Serial.println("Rainbow Mode ON");
      break;
    case 3:
      fade_animation = true;
      Serial.println("Fade Mode ON");
      break;
    case 4:
      hallway_lights_on = true;
      hallway_on = false;
      Serial.println("Hallway Lights ON");
      break;
    case 5:
      movie_mode_on = true;
      movie_on = false;
      Serial.println("Movie Mode ON");
      break;
    default:
      break;
  }
  return true;
}

void setupWiFi() {
  Serial.printf("\r\n[Wifi]: Connecting");
  WiFi.persistent( false );
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(".");
    delay(250);
  }
  IPAddress localIP = WiFi.localIP();
  Serial.printf("connected!\r\n[WiFi]: IP-Address is %d.%d.%d.%d\r\n", localIP[0], localIP[1], localIP[2], localIP[3]);
}

void setupSinricPro() {
  Serial.println("Setting up SinricPro...");
  // get a new Light device from SinricPro
  SinricProLight &myLight = SinricPro[LIGHT_ID];

  // set callback function to device
  myLight.onPowerState(onPowerState);
  myLight.onColorTemperature(onColorTemperature);

  // setup SinricPro
  SinricPro.onConnected([](){ Serial.printf("Connected to SinricPro\r\n"); }); 
  SinricPro.onDisconnected([](){ Serial.printf("Disconnected from SinricPro\r\n"); });
  SinricPro.begin(APP_KEY, APP_SECRET);
}

// main setup function
void setup() {
  Serial.begin(BAUD_RATE); Serial.printf("\r\n\r\n");
  setupWiFi();
  setupSinricPro();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  FastLED.addLeds<WS2812B, LED_PIN_1, GRB>(led1, NUM_LEDS_1);
  FastLED.addLeds<WS2812B, LED_PIN_2, GRB>(led2, NUM_LEDS_2);
  
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 2000); // 5V, 2000 mA
  FastLED.clear();
  FastLED.show();
  delay(100);

  startup_fadeinout();
  delay(500);

  FastLED.clear();
  FastLED.show();
}

void loop() {
  SinricPro.handle();
  if (twinkle_animation) {twinkle_action();}
  if (firework_animation) {firework_action();}
  if (rainbow_animation) {rainbow_action();}
  if (fade_animation) {fade_action();}
  if (hallway_lights_on) {hallway_lights();}
  if (movie_mode_on) {movie_mode();}
}
