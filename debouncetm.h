
// inspired by https://hackaday.com/2015/12/10/embed-with-elliot-debounce-your-noisy-buttons-part-ii/

#ifndef debouncetm_h
#define debouncetm_h
#include "Arduino.h"
#include "elapsedMillis.h"

class Button {

  public:
    Button(uint8_t _button_pin, byte _pin_mode, bool _polarity, float _samplerate, float _longpress_duration, float _doubleclick_window);
    void update();
    bool pressed();
    bool released();
    bool longpressed();
    bool clicked();
    bool doubleclicked();
    bool isHigh();
    bool isLow();

  protected:
    uint32_t inline read();
    uint8_t button_pin;
    byte pin_mode;
    bool polarity;
    uint32_t samplerate;
    uint32_t longpress_duration;
    uint32_t doubleclick_window;
    uint32_t history;
    uint8_t counter;
    elapsedMicros stopwatch;
    elapsedMicros stopwatch_pressed;
    elapsedMillis stopwatch_doubleclicked;
    bool state_flag;
    bool pressed_flag;
    bool clicked_flag;
    bool doubleclick_flag;
};

#endif
