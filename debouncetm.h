
// inspired by https://hackaday.com/2015/12/10/embed-with-elliot-debounce-your-noisy-buttons-part-ii/

#ifndef debouncetm_h
#define debouncetm_h
#include "Arduino.h"
#include "elapsedMillis.h"

class Button {

  public:
    Button(int _button_pin, byte _pin_mode, bool _polarity, float _samplerate, float _longpress_duration, float _doubleclick_window);                      // 16bit history
    Button(int _button_pin, byte _pin_mode, bool _polarity, float _samplerate, float _longpress_duration, float _doubleclick_window, int _history_length); // 0 = 8bit, 1 = 32bit history
    void update();
    bool pressed();
    bool released();
    bool longpressed();
    bool clicked();
    bool doubleclicked();
    bool isHigh();
    bool isLow();

  protected:
    int inline read();
    int button_pin;
    byte pin_mode;
    bool polarity;
    int samplerate;
    int longpress_duration;
    int doubleclick_window;
    uint8_t history_8 = 0;
    uint16_t history = 0;
    uint32_t history_32 = 0;
    int counter;
    elapsedMicros stopwatch;
    elapsedMicros stopwatch_pressed;
    elapsedMillis stopwatch_doubleclicked;
    bool state_flag;
    bool pressed_flag;
    bool clicked_flag;
    bool doubleclick_flag;
    int history_length;
};

#endif
