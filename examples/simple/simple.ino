
// inspired by https://hackaday.com/2015/12/10/embed-with-elliot-debounce-your-noisy-buttons-part-ii/

#include <debouncetm.h>

#define buttonpin        2  // button pin
#define samplerate    0.75  // poll interval for update() in msec (a safe starting point would be samplerate = 0.1 * bounce-duration in msec)
#define longpress     0.90  // longpress duration in sec
#define doubleclick   0.50  // doubleclick window in sec
#define history         16  // internal history length: 8, 16 or 32 bit

Button button1(buttonpin, INPUT_PULLUP, LOW, samplerate, longpress, doubleclick, history);  // specify pin, pinMode, polarity, samplerate, longpress duration, doubleclick window, history length

unsigned long previousMillis = millis();

void setup() {Serial.begin(115200);}

void loop() {
  button1.update();
  if (button1.pressed()) Serial.println("pressed");               // returns true only if called after update()
  if (button1.longpressed()) Serial.println("longpressed");       // returns true only if called after pressed()
  if (button1.released()) Serial.println("released");             // returns true only if called after pressed()
  if (button1.clicked()) Serial.println("clicked");               // returns true only if called after released()
  if (button1.doubleclicked()) Serial.println("doubleclicked");   // returns true only if called after clicked()

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 1000) {                   // check every second:
    previousMillis = currentMillis;
    if (button1.isLow()) Serial.println("isLow");                 // returns true only if called after pressed()
    if (button1.isHigh()) Serial.println("isHigh");               // returns true only if called after released()
  }
}
