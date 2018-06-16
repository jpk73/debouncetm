
// https://hackaday.com/2015/12/10/embed-with-elliot-debounce-your-noisy-buttons-part-ii/

#include <debouncetm.h>

#define button_pin1 64           // pin of first button
#define button_pin2 68           // pin of second button 
#define samplerate 4             // update() button poll interval in ms
#define longpress_duration 1000  // duration until longpress returns true in ms

Button Button1(button_pin1, samplerate, longpress_duration);
Button Button2(button_pin2, samplerate, longpress_duration);

void setup() {
  Serial.begin(9600);
}

void loop() {
  Button1.update();
  if (Button1.pressed()) Serial.println("Button1 pressed");
  if (Button1.longOnce()) Serial.println("Button1 longpressed"); // Only one longpress will be detected. Needs reset by released()!
  if (Button1.released()) Serial.println("Button1 released");

  Button2.update();
  if (Button2.pressed()) Serial.println("Button2 pressed");
  if (Button2.longRepeat()) Serial.println("Button2 longpressed"); // Repeated return of longpress. Needs reset by released()!
  if (Button2.released()) Serial.println("Button2 released");
}
