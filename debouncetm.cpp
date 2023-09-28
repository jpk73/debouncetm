
// inspired by https://hackaday.com/2015/12/10/embed-with-elliot-debounce-your-noisy-buttons-part-ii/

#include "Arduino.h"
#include "debouncetm.h"

Button::Button(int _button_pin, byte _pin_mode, bool _polarity, float _samplerate, float _longpress_duration, float _doubleclick_window) {
  polarity = _polarity;
  button_pin = _button_pin;
  samplerate = _samplerate * 1000;
  longpress_duration = _longpress_duration * 1000;
  doubleclick_window = _doubleclick_window * 1000;
  history_length = 16;
  history = 0b1111111111111111;
  pressed_flag = 0;
  state_flag = 1;
  doubleclick_flag = 0;
  counter = 0;
  stopwatch = 0;
  stopwatch_pressed = 0;
  stopwatch_doubleclicked = 0;
  pinMode(button_pin, _pin_mode);
}

Button::Button(int _button_pin, byte _pin_mode, bool _polarity, float _samplerate, float _longpress_duration, float _doubleclick_window, int _history_length) {
  polarity = _polarity;
  button_pin = _button_pin;
  samplerate = _samplerate * 1000;
  longpress_duration = _longpress_duration * 1000;
  doubleclick_window = _doubleclick_window * 1000;
  if (_history_length == 8) {history_length = 8; history_8 = 0b11111111;}
  else if (_history_length == 32) {history_length = 32; history_32 = 0b11111111111111111111111111111111;}
  else {history_length = 16; history = 0b1111111111111111;}
  pressed_flag = 0;
  state_flag = 1;
  doubleclick_flag = 0;
  counter = 0;
  stopwatch = 0;
  stopwatch_pressed = 0;
  stopwatch_doubleclicked = 0;
  pinMode(button_pin, _pin_mode);
}

void Button::update() {
  if (stopwatch >= samplerate) {
    stopwatch = 0;
    if (history_length == 8) {history_8 = (history_8 & 0b10000000) | ((history_8 & 0b00111111) << 1) | read();}
    else if (history_length == 16) {history = (history & 0b1000000000000000) | ((history & 0b0011111111111111) << 1) | read();}
    else if (history_length == 32) {history_32 = (history_32 & 0b10000000000000000000000000000000) | ((history_32 & 0b00111111111111111111111111111111) << 1) | read();}
    // for (int j = 31; j >= 0; j--) {Serial.print(bitRead(history, j));} Serial.println();
  }
}

int inline Button::read() {
  int state = 0;
#if defined (__arm__) && defined (CORE_TEENSY) // digitalReadFast on teensy
  state = digitalReadFast(button_pin);
#else
  state = digitalRead(button_pin);
#endif
  return state ^ polarity;
}

bool Button::pressed() {
  bool press = 0;
  if (history_length == 8 && ((history_8 & 0b11000111) == 0b11000000 || (state_flag && isLow()))) {
    press = 1;
    pressed_flag = 1;
    state_flag = 0;
    history_8 = 0b00000000;
  }
  else if (history_length == 16 && ((history & 0b1111100000111111) == 0b1111100000000000 || (state_flag && isLow()))) {
    press = 1;
    pressed_flag = 1;
    state_flag = 0;
    history = 0b000000000000000000;
  }
  else if (history_length == 32 && ((history_32 & 0b11111111110000000000011111111111) == 0b11111111110000000000000000000000 || (state_flag && isLow()))) {
    press = 1;
    pressed_flag = 1;
    state_flag = 0;
    history_32 = 0b00000000000000000000000000000000;
  }
  return press;
}

bool Button::released() {
  bool release = 0;
  if (history_length == 8 && ((history_8 & 0b11000111) == 0b00000111 || (!state_flag && isHigh()))) {
    release = 1;
    if (pressed_flag == 1) clicked_flag = 1;
    pressed_flag = 0;
    state_flag = 1;
    history_8 = 0b11111111;
  }
  else if (history_length == 16 && ((history & 0b1111100000111111) == 0b0000000000111111 || (!state_flag && isHigh()))) {
    release = 1;
    if (pressed_flag == 1) clicked_flag = 1;
    pressed_flag = 0;
    state_flag = 1;
    history = 0b1111111111111111;
  }
  else if (history_length == 32 && ((history_32 & 0b11111111110000000000011111111111) == 0b00000000000000000000011111111111 || (!state_flag && isHigh()))) {
    release = 1;
    if (pressed_flag == 1) clicked_flag = 1;
    pressed_flag = 0;
    state_flag = 1;
    history_32 = 0b11111111111111111111111111111111;
  }
  return release;
}

bool Button::longpressed() {
  bool pressed = 0;
  if (pressed_flag == 0) stopwatch_pressed = 0;
  else if (stopwatch_pressed / 1000 >= longpress_duration) {
    pressed = 1;
    pressed_flag = 0;
    stopwatch_pressed = 0;
  }
  return pressed;
}

bool Button::clicked() {
  bool clicked = 0;
  if (stopwatch_pressed / 1000 < longpress_duration) {
    clicked = clicked_flag;
    clicked_flag = 0;
    if (doubleclick_flag && clicked) {
      counter++;
      if (counter == 2) clicked = 0;
    }
  }
  return clicked;
}

bool Button::doubleclicked() {
  if (!doubleclick_flag) doubleclick_flag = 1;
  bool doubleclicked = 0;
  if (counter == 0) {stopwatch_doubleclicked = 0;}
  else if (stopwatch_doubleclicked > doubleclick_window) {counter = 0; stopwatch_doubleclicked = 0;}
  else if (counter == 2) {doubleclicked = 1; counter = 0; stopwatch_doubleclicked = 0;}
  return doubleclicked;
}

bool Button::isHigh() {
  bool high = 0;
  if (history_length == 8 && ((history_8 & 0b11000111) == 0b11000111)) high = 1;
  else if (history_length == 16 && ((history & 0b1111100000111111) == 0b1111100000111111)) high = 1;
  else if (history_length == 32 && ((history_32 & 0b11111111110000000000011111111111) == 0b11111111110000000000011111111111)) high = 1;
  return high;
}

bool Button::isLow() {
  bool low = 0;
  if (history_length == 8 && ((history_8 & 0b11000111) == 0b00000000)) low = 1;
  else if (history_length == 16 && ((history & 0b1111100000111111) == 0b0000000000000000)) low = 1;
  else if (history_length == 32 && ((history_32 & 0b11111111110000000000011111111111) == 0b00000000000000000000000000000000)) low = 1;
  return low;
}
