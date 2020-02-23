
// inspired by https://hackaday.com/2015/12/10/embed-with-elliot-debounce-your-noisy-buttons-part-ii/

#include "Arduino.h"
#include "debouncetm.h"

Button::Button(uint8_t _button_pin, byte _pin_mode, bool _polarity, float _samplerate, float _longpress_duration, float _doubleclick_window) {
  polarity = _polarity;
  button_pin = _button_pin;
  samplerate = _samplerate * 1000;
  longpress_duration = _longpress_duration * 1000000;
  doubleclick_window = _doubleclick_window * 1000;
  history = 0b11111111111111111111111111111111;
  pressed_flag = 0;
  counter = 0;
  stopwatch = 0;
  stopwatch_pressed = 0;
  stopwatch_doubleclicked = 0;
  pinMode(button_pin, _pin_mode);
}

void Button::update() {
  if (stopwatch >= samplerate) {
    stopwatch = 0;
    history = (history & 0b10000000000000000000000000000000) | ((history & 0b00111111111111111111111111111111) << 1) | read();
    // for (int j = 31; j >= 0; j--) {Serial.print(bitRead(history, j));} Serial.println();
  }
}

uint32_t inline Button::read() {
  uint32_t state = 0;
#if defined (__arm__) && defined (CORE_TEENSY) // digitalReadFast on teensy
  state = digitalReadFast(button_pin);
#else
  state = digitalRead(button_pin);
#endif
  return state ^ polarity;
}

bool Button::pressed() {
  bool press = 0;
  if ((history & 0b11111111110000000000011111111111) == 0b11111111110000000000000000000000) {
    press = 1;
    pressed_flag = 1;
    history = 0b00000000000000000000000000000000;
  }
  return press;
}

bool Button::released() {
  bool release = 0;
  if ((history & 0b11111111110000000000011111111111) == 0b00000000000000000000011111111111) {
    release = 1;
    if (pressed_flag == 1) clicked_flag = 1;
    pressed_flag = 0;
    history = 0b11111111111111111111111111111111;
  }
  return release;
}

bool Button::longpressed() {
  bool pressed = 0;
  if (pressed_flag == 0) stopwatch_pressed = 0;
  else if (stopwatch_pressed >= longpress_duration) {
    pressed = 1;
    pressed_flag = 0;
    stopwatch_pressed = 0;
  }
  return pressed;
}

bool Button::clicked() {
  bool clicked = 0;
  if (stopwatch_pressed < longpress_duration) {
    clicked = clicked_flag;
    clicked_flag = 0;
    if (clicked == 1) counter++;
  }
  return clicked;
}

bool Button::doubleclicked() {
  bool doubleclicked = 0;
  if (counter == 0) {stopwatch_doubleclicked = 0;}
  else if (stopwatch_doubleclicked > doubleclick_window) {counter = 0; stopwatch_doubleclicked = 0;}
  else if (counter == 2) {doubleclicked = 1; counter = 0; stopwatch_doubleclicked = 0;}
  return doubleclicked;
}

bool Button::isHigh() {
  bool high = 0;
  if ((history & 0b11111111110000000000011111111111) == 0b11111111110000000000011111111111) high = 1;
  return high;
}

bool Button::isLow() {
  bool low = 0;
  if ((history & 0b11111111110000000000011111111111) == 0b00000000000000000000000000000000) low = 1;
  return low;
}
