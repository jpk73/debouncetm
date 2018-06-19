
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
  if (Button1.pressed()) Serial.println("Button1 pressed");               // needs reset by released()
  if (Button1.longpressed(repeat)) Serial.println("Button1 longpressed"); // repeated return of longpress
  if (Button1.released()) Serial.println("Button1 released");

  Button2.update();
  if (Button2.pressed()) Serial.println("Button2 pressed");               // needs reset by released()
  if (Button2.longpressed(once)) Serial.println("Button2 longpressed");   // only one longpress will be detected
  if (Button2.released()) Serial.println("Button2 released");

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 1000) {                           // check every second:
    previousMillis = currentMillis;
    if (Button1.isHigh()) Serial.println("Button1 HIGH");
    if (Button1.isLow()) Serial.println("Button1 LOW");
    if (Button2.isHigh()) Serial.println("Button2 HIGH");
    if (Button2.isLow()) Serial.println("Button2 LOW");
  }
}
