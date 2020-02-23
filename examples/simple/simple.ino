
// inspired by https://hackaday.com/2015/12/10/embed-with-elliot-debounce-your-noisy-buttons-part-ii/

#include <debouncetm.h>

#define buttonpin        2  // button pin
#define samplerate    0.75  // poll interval for update() in msec (a safe starting point would be samplerate = 0.1 * bounce-duration in msec)
#define longpress     0.80  // longpress duration in sec

Button button1(buttonpin, INPUT_PULLUP, LOW, samplerate, longpress);  // specify pin, pinMode, polarity, samplerate, longpress duration

unsigned long previousMillis = millis();

void setup() {Serial.begin(115200);}

void loop() {
  button1.update();
  if (button1.pressed()) Serial.println("pressed");               // pressed() returns true only if called after released()
  if (button1.released()) Serial.println("released");             // released() returns true only if called after pressed()
  if (button1.longpressed()) Serial.println("longpressed");       // longpressed() returns true only if called after pressed()
  if (button1.clicked()) Serial.println("clicked");               // clicked() returns true only if called after longpressed() and released()

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 1000) {                   // check every second:
    previousMillis = currentMillis;
    if (button1.isHigh()) Serial.println("button1 HIGH");         // isHigh() returns true only if called after released()
    if (button1.isLow()) Serial.println("button1 LOW");           // isLow() returns true only if called after pressed()
  }
}