
// https://hackaday.com/2015/12/10/embed-with-elliot-debounce-your-noisy-buttons-part-ii/

#ifndef debouncetm_h
#define debouncetm_h
#include "Arduino.h"

class Button {

  public:
    Button(uint8_t _button_pin, uint8_t _samplerate, uint16_t _longpress_duration);
    void update();
    bool pressed();
    bool released();
    bool longpressed(bool repeat);

  protected:
    uint8_t inline read();
    uint8_t button_pin;
    uint8_t samplerate;
    uint16_t longpress_duration;
    uint8_t history;
    uint64_t now;
    uint64_t timestamp;
    uint64_t timestamp_lpress;
    bool lpress_flag;

};

#endif
