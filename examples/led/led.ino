
// inspired by https://hackaday.com/2015/12/10/embed-with-elliot-debounce-your-noisy-buttons-part-ii/

#include <debouncetm.h>

#define outpin LED_BUILTIN  // led pin
#define buttonpin        2  // button pin
#define samplerate    0.75  // poll interval for update() in msec (a safe starting point would be samplerate = 0.1 * bounce-duration in msec)
#define longpress     0.90  // longpress duration in sec
#define doubleclick   0.50  // doubleclick window in sec

Button button1(buttonpin, INPUT_PULLUP, LOW, samplerate, longpress, doubleclick);  // specify pin, pinMode, polarity, samplerate, longpress duration, doubleclick window

void setup() {
  pinMode(outpin, OUTPUT);
}

void loop() {
  button1.update();
  if (button1.pressed()) {digitalWrite(outpin, HIGH);}
  if (button1.released()) {digitalWrite(outpin, LOW);}
}
