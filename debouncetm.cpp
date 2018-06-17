
// https://hackaday.com/2015/12/10/embed-with-elliot-debounce-your-noisy-buttons-part-ii/

#include "Arduino.h"
#include "debouncetm.h"

Button::Button(uint8_t _button_pin, uint8_t _samplerate, uint16_t _longpress_duration) {
  button_pin = _button_pin;
  samplerate = _samplerate;
  longpress_duration = _longpress_duration;
  history = 0b11111111;
  now = millis();
  timestamp = now;
  timestamp_lpress = now;
  lpress_flag = 0;
  pinMode(button_pin, INPUT_PULLUP);
}

void Button::update() {
  now = millis();
  if (now - timestamp >= samplerate) {
    timestamp = now;
    history = (history & 0b10000000) | ((history & 0b00111111) << 1) | read();
  }
}

uint8_t inline Button::read() {
  uint8_t state = 0;
#if defined (__arm__) && defined (CORE_TEENSY) // digitalReadFast on teensy
  state = digitalReadFast(button_pin);
#else
  state = digitalRead(button_pin);
#endif
  return state;
}

bool Button::pressed() {
  bool press = 0;
  if ((history & 0b11000111) == 0b11000000) {
    press = 1;
    lpress_flag = 1;
    history = 0b00000000;
  }
  return press;
}

bool Button::released() {
  bool release = 0;
  if ((history & 0b11000111) == 0b00000111) {
    release = 1;
    lpress_flag = 0;
    history = 0b11111111;
  }
  return release;
}

bool Button::longpressed(bool repeat) {
  bool lpress = 0;
  if (lpress_flag == 0) timestamp_lpress = now;
  else if (now - timestamp_lpress >= longpress_duration) {
    lpress = 1;
    lpress_flag = repeat;
    timestamp_lpress = now;
  }
  return lpress;
}
