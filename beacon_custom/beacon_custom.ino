#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

const uint8_t LED_PIN = 10;
const uint8_t NUM_LEDS = 6;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

const uint8_t NUM_ROWS = 9;
const uint8_t NUM_COLS = 2;
const uint8_t NUM_BUTTONS = NUM_ROWS * NUM_COLS;


uint8_t buttons[18] = {0};
const uint8_t rowPins[9] = {A3, 2, 3, 4, 5, 6, 7, 8, 9};
const uint8_t colPins[2] = {A5, A4};

void setup() {
//  Serial.begin(9600);
  for(int i=0; i<NUM_ROWS; i++){
    pinMode(rowPins[i], INPUT_PULLUP);
  }
  for(int i=0; i<NUM_COLS; i++){
    pinMode(colPins[i], OUTPUT);
  }
  strip.begin();
  strip.setBrightness(255);
//  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  readButtons();
  for(int i=0; i<NUM_LEDS; i++){
    if(i<NUM_LEDS/2){
      strip.setPixelColor(i, 255*buttons[i], 255*buttons[i+NUM_LEDS/2], 255*buttons[i+NUM_LEDS]);
    } else {
      strip.setPixelColor(i, 255*buttons[i+NUM_LEDS], 255*buttons[i+NUM_LEDS*3/2], 255*buttons[i+NUM_LEDS*2]);
    }
  }
  strip.show();
  delay(5);
}

void readButtons(){
  digitalWrite(colPins[0], LOW);
  digitalWrite(colPins[1], HIGH);
  for(int i=0; i<NUM_ROWS; i++){
    buttons[i] = !digitalRead(rowPins[i]);
  }
  digitalWrite(colPins[0], HIGH);
  digitalWrite(colPins[1], LOW);
  for(int i=0; i<NUM_ROWS; i++){
    buttons[NUM_ROWS + i] = !digitalRead(rowPins[i]);
  }
}
