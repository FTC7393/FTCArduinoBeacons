#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

const uint8_t LED_PIN = 10;

const uint8_t LEFT_BUTTON = 7; //Btn1
const uint8_t BACK_BUTTON = 8; //Rndm
const uint8_t RIGHT_BUTTON = 9; //Btn2

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

//false is red, true is blue
boolean leftColor = false;
boolean rightColor = true;

unsigned long noPressEnd = 0;

int fade = 0;

int buttonStates[3] = {0}; // the current reading from the input pin

void setup() {
  randomSeed(analogRead(A7));

  pinMode(LEFT_BUTTON, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON, INPUT_PULLUP);
  pinMode(BACK_BUTTON, INPUT_PULLUP);
  //  Serial.begin(9600);

  strip.begin();
  strip.setBrightness(255);

  //fading init sequence
  for (int j = 0; j < 2; j++) {
    for (int i = 0; i < 255; i++) {
      setAllColor(0, i, 0);
      delay(1);
    }
    for (int i = 0; i < 255; i++) {
      setAllColor(0, 255 - i, 0);
      delay(1);
    }
  }

  //custom init sequence
  //  setAllColor(0, 0, 0);
  //  delay(250);
  //  setAllColor(0, 255, 0);
  //  delay(250);
  //  setAllColor(0, 0, 0);
  //  delay(250);
  //  setAllColor(0, 255, 0);
  //  delay(250);
  //  setAllColor(0, 0, 0);

  //standard init sequence
  //  setAllColor(255, 0, 0);
  //  delay(250);
  //  setAllColor(0, 255, 0);
  //  delay(250);
  //  setAllColor(0, 0, 255);
  //  delay(250);
  //  setAllColor(0, 255, 0);
  //  delay(500);
  randomize();
}

void randomize() {
  noPressEnd = 0; // reset the press timer

  rightColor = random(2); // random 0 or 1
  leftColor = !rightColor; // the opposite

  //blink the lights back and forth 20 times
  for (int i = 0; i < 20; i++) {
    leftColor = !leftColor;
    rightColor = !rightColor;
    clear_display();
    delay(25);
    update_display();
    delay(100);
  }
  readButtonsAbsolute();
  while (buttonStates[0] || buttonStates[1]) {
    delay(10);
    readButtons();
  }
}

//could be simpler, but is more readable this way
void update_display() {
  int fade2 = 255 - fade;
  if (leftColor) {
    strip.setPixelColor(0, fade, 0, fade2);
    strip.setPixelColor(1, fade, 0, fade2);
    strip.setPixelColor(2, fade, 0, fade2);
  } else {
    strip.setPixelColor(0, fade2, 0, fade);
    strip.setPixelColor(1, fade2, 0, fade);
    strip.setPixelColor(2, fade2, 0, fade);
  }
  if (rightColor) {
    strip.setPixelColor(3, fade, 0, fade2);
    strip.setPixelColor(4, fade, 0, fade2);
    strip.setPixelColor(5, fade, 0, fade2);
  } else {
    strip.setPixelColor(3, fade2, 0, fade);
    strip.setPixelColor(4, fade2, 0, fade);
    strip.setPixelColor(5, fade2, 0, fade);
  }
  strip.show();
}

void clear_display() {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, 0, 0, 0);
  }
  strip.show();
}

// set all the LEDs to the same color
void setAllColor(int red, int green, int blue) {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, red, green, blue);
  }
  strip.show();
}

void loop() {
  readButtons();
  //  if (buttonStates[0]) Serial.println("LEFT");
  //  if (buttonStates[1]) Serial.println("RIGHT");
  //  if (buttonStates[2]) Serial.println("BACK");

  if (buttonStates[0] // left button
      || buttonStates[1] //right button
     ) {

    unsigned long now = millis();
    if (now > noPressEnd) {

      if (rightColor == leftColor) {
        leftColor = !leftColor;
        rightColor = !rightColor;
        fade = 255;
        noPressEnd = now + 7000;
        update_display();
      } else {
        if (buttonStates[0]) {
          rightColor = leftColor;
        } else {
          leftColor = rightColor;
        }
        noPressEnd = now + 5000;
      }
      update_display();
    }
  } else if (buttonStates[2]) { // back button
    unsigned long pressedTime = 1000;
    while (pressedTime >= 1000) {
      unsigned long pressTime = millis();
      while (buttonStates[2]) {
        delay(10);
        readButtons();
        pressedTime = millis() - pressTime;
        if (pressedTime >= 1000) {
          clear_display();
          waitUntilBackReleased();
          break;
        }
      }
      if (pressedTime >= 1000) {
        waitUntilBackPressed();
      }
    }
    randomize();
    waitUntilBackReleased();
  }
  if (fade > 0) fade -= 5;
  if (fade < 0) fade = 0;
  update_display();
  delay(10);
}

void waitUntilBackPressed() {
  waitUntilButtonPressed(2);
}

void waitUntilBackReleased() {
  waitUntilButtonReleased(2);
}

void waitUntilButtonReleased(int button) {
  readButtons();
  while (buttonStates[button]) {
    delay(10);
    readButtons();
  }
}

void waitUntilButtonPressed(int button) {
  readButtons();
  while (!buttonStates[button]) {
    delay(10);
    readButtons();
  }
}

//modified from arduino debouncing code

// constants won't change. They're used here to
// set pin numbers:
const int buttonPins[3] = {LEFT_BUTTON, RIGHT_BUTTON, BACK_BUTTON};    // the number of the pushbutton pin

// Variables will change:

int lastButtonStates[3] = {LOW};   // the previous reading from the input pin

// the following variables are unsigned long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTimes[3] = {0};  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers


void readButtons() {
  for (int i = 0; i < 3; i++) {

    // read the state of the switch into a local variable:
    int reading = !digitalRead(buttonPins[i]);

    // check to see if you just pressed the button
    // (i.e. the input went from LOW to HIGH),  and you've waited
    // unsigned long enough since the last press to ignore any noise:

    // If the switch changed, due to noise or pressing:
    if (reading != lastButtonStates[i]) {
      // reset the debouncing timer
      lastDebounceTimes[i] = millis();
    }

    if ((millis() - lastDebounceTimes[i]) > debounceDelay) {
      // whatever the reading is at, it's been there for unsigned longer
      // than the debounce delay, so take it as the actual current state:

      // if the button state has changed:
      if (reading != buttonStates[i]) {
        buttonStates[i] = reading;

      }
    }

    // save the reading.  Next time through the loop,
    // it'll be the lastButtonState:
    lastButtonStates[i] = reading;
  }


}

void readButtonsAbsolute() {
  for (int i = 0; i < 3; i++) {
    // read the state of the switch
    buttonStates[i] = !digitalRead(buttonPins[i]);
  }
}
