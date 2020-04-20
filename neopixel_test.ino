// Simple NeoPixel test.  Lights just a few pixels at a time so a
// 1m strip can safely be powered from Arduino 5V pin.  Arduino
// may nonetheless hiccup when LEDs are first connected and not
// accept code.  So upload code first, unplug USB, connect pixels
// to GND FIRST, then +5V and digital pin 6, then re-plug USB.
// A working strip will show a few pixels moving down the line,
// cycling between red, green and blue.  If you get no response,
// might be connected to wrong end of strip (the end wires, if
// any, are no indication -- look instead for the data direction
// arrows printed on the strip).

#include <Adafruit_NeoPixel.h>
#include <IRremote.h>

#define PIN      6
#define N_LEDS 106

int soundPin = A0;
int sensorValue = 0;
int lastTenValues[10]; 

int RECV_PIN = 11;

int b = 0;

int cRed = 0;
int cGreen = 0;
int cBlue = 0;
int randPixel = 0;
int randUsed = 0;

int strip_brightness = 70;

int holding_loop = 0;

bool chase_leds = false;

bool invalid_option = true;
bool waiting_for_second = false;

IRrecv irrecv(RECV_PIN);
IRsend irsend;

decode_results results;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.setBrightness(strip_brightness);
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  if(irrecv.decode(&results))
  {
    showReceivedData();
    irrecv.resume(); //Receive the next value
  }
  delay(25);
}

void showReceivedData(){
  irrecv.resume();
  Serial.println(results.value, HEX);
  if (results.value == 0xFF30CF){ // One: Chase
    set_colour(strip.Color(250, 250, 250));
    irrecv.resume();
    while(!irrecv.decode(&results)){
    }
    set_colour(strip.Color(0, 0, 0));
    if (results.value == 0xFF30CF){ // One
      chase(strip.Color(255, 0, 0));// Red
    }
    else if (results.value == 0xFF18E7){ // Two
      chase(strip.Color(255, 145, 0)); // Orange
    }
    else if (results.value == 0xFF7A85){ // Three
      chase(strip.Color(255, 255, 0)); //Yellow
    }
    else if (results.value == 0xFF10EF){ // Four
      chase(strip.Color(0, 255, 0)); //Green
    }
    else if (results.value == 0xFF38C7){ // Five
      chase(strip.Color(22, 224, 123)); //Turquoise
    }
    else if (results.value == 0xFF5AA5){ // Six
      chase(strip.Color(22, 191, 224)); //Cyan
    }
    else if (results.value == 0xFF42BD){ // Seven
      chase(strip.Color(0, 0, 255)); //Blue
    }
    else if (results.value == 0xFF4AB5){ // Eight
      chase(strip.Color(69, 0, 138)); //Purple
    }
    else if (results.value == 0xFF52AD){ // Nine
      chase(strip.Color(255, 102, 204)); //Pink
    }
    delay(10);
    results.value = 0xFFFFFF;
  }
  if (results.value == 0xFF18E7){ // Two: Fill
    set_colour(strip.Color(250, 250, 250));
    irrecv.resume();
    while(!irrecv.decode(&results)){
    }
    set_colour(strip.Color(0, 0, 0));
    if (results.value == 0xFF30CF){ // One
      set_colour(strip.Color(255, 0, 0));// Red
    }
    else if (results.value == 0xFF18E7){ // Two
      set_colour(strip.Color(255, 145, 0)); // Orange
    }
    else if (results.value == 0xFF7A85){ // Three
      set_colour(strip.Color(255, 255, 0)); //Yellow
    }
    else if (results.value == 0xFF10EF){ // Four
      set_colour(strip.Color(0, 255, 0)); //Green
    }
    else if (results.value == 0xFF38C7){ // Five
      set_colour(strip.Color(22, 224, 123)); //Turquoise
    }
    else if (results.value == 0xFF5AA5){ // Six
      set_colour(strip.Color(22, 191, 224)); //Cyan
    }
    else if (results.value == 0xFF42BD){ // Seven
      set_colour(strip.Color(0, 0, 255)); //Blue
    }
    else if (results.value == 0xFF4AB5){ // Eight
      set_colour(strip.Color(69, 0, 138)); //Purple
    }
    else if (results.value == 0xFF52AD){ // Nine
      set_colour(strip.Color(255, 102, 204)); //Pink
    }
    delay(10);
    results.value = 0xFFFFFF;
  }
  if (results.value == 0xFF7A85){
    set_colour(strip.Color(250, 250, 250));
    irrecv.resume();
    while(!irrecv.decode(&results)){
    }
    set_colour(strip.Color(0, 0, 0));
    if (results.value == 0xFF30CF){ // One
      rainbow(5);// Red
    }
    else if (results.value == 0xFF18E7){ // Two
      rainbow(20); // Green
    }
    else if (results.value == 0xFF7A85){ // Three
      rainbow(35); //Blue
    }
    else if (results.value == 0xFF10EF){
      random_fill();
    }
    delay(10);
    results.value = 0xFFFFFF;
  }
}
/*
static void lightMusic(){
  cRed = random(0, 255);
  cGreen = random(0, 255);
  cBlue = random(0, 255);
  sensorValue = analogRead(soundPin);
  //Serial.println(sensorValue, DEC);
  delay(100);
  lastTenValues[0] = soundPin;
  for(int i = 0; i < 10; i++){
    Serial.print(lastTenValues[i]);
  }
}*/

static void random_fill(){
  results.value = 0xFFFFFF;
  while(results.value == 0xFFFFFF){
    irrecv.resume();
    while(randUsed < strip.numPixels()*10){
      if (!irrecv.decode(&results)){
      }
      randPixel = random(0, strip.numPixels());
      cRed = random(0, 255);
      cGreen = random(0, 255);
      cBlue = random(0, 255);
      strip.setPixelColor(randPixel, strip.Color(cRed, cGreen, cBlue));
      randUsed += 1;
      strip.show();
      delay(30);
    }
    randUsed = 0;
  }
}

static void set_colour(uint32_t c){
  for (uint32_t i = 0; i < strip.numPixels(); i++){
    strip.setPixelColor(i, c);
    strip.show();
  }
}

static void chase(uint32_t c) {
  delay(10);
  results.value = 0xFFFFFF;
  while(results.value == 0xFFFFFF){
    irrecv.resume();
    for (uint32_t i = 0; i < strip.numPixels()+30; i++) {
      if (!irrecv.decode(&results)){
      }
      //delay(5);
      strip.setPixelColor(i, 0);// Set them to blank
      strip.setPixelColor(i, c); // Draw new pixel
      strip.setPixelColor(i-4, 0); // Erase pixel a few steps back
      strip.show();
      delay(25);
    } 
  }
}

void rainbow(int wait) {
  delay(10);
  results.value = 0xFFFFFF;
  while(results.value == 0xFFFFFF){
    for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
      for(int i=0; i<strip.numPixels(); i++) {
        if (!irrecv.decode(&results)){
        }
        int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
        strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
      }
      strip.show();
      delay(wait);
    }
  }
}
