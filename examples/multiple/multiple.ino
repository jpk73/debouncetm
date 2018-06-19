
// https://hackaday.com/2015/12/10/embed-with-elliot-debounce-your-noisy-buttons-part-ii/

#include <debouncetm.h>

#define button_pin1 64           // pin of first button
#define button_pin2 68           // pin of second button 
#define samplerate 4             // update() button poll interval in ms
#define longpress_duration 1000  // duration until longpress returns true in ms

#define repeat 1
#define once 0

Button Button1(button_pin1, samplerate, longpress_duration);
Button Button2(button_pin2, samplerate, longpress_duration);

void setup() {
  Serial.begin(115200);
}

unsigned long previousMillis = millis();

void loop() {
  Button1.update();
  if (Button1.pressed()) Serial.println("Button1 pressed");               // pressed() returns true only if called after released()
  if (Button1.longpressed(repeat)) Serial.println("Button1 longpressed"); // longpressed(1) repeatedly returns true
  if (Button1.released()) Serial.println("Button1 released");             // released() returns true only if called after pressed()

  Button2.update();
  if (Button2.pressed()) Serial.println("Button2 pressed");
  if (Button2.longpressed(once)) Serial.println("Button2 longpressed");   // longpressed(0) returns true only once
  if (Button2.released()) Serial.println("Button2 released");

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 1000) {                           // check every second:
    previousMillis = currentMillis;
    if (Button1.isHigh()) Serial.println("Button1 HIGH");                 // isHigh() returns true only if called after released()
    if (Button1.isLow()) Serial.println("Button1 LOW");                   // isLow() returns true only if called after pressed()
    if (Button2.isHigh()) Serial.println("Button2 HIGH");
    if (Button2.isLow()) Serial.println("Button2 LOW");
  }
}
