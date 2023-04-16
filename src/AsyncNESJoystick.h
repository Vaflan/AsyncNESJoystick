/*!
 *  @file AsyncNESJoystick.h
 *
 * Asynchronous method for reading famiclone controllers (NES)
 * Many thanks to the resource: https://tresi.github.io/nes/
 */

#ifndef AsyncNESJoystick_h
#define AsyncNESJoystick_h

#define ANJ_BUTTON_A      0
#define ANJ_BUTTON_B      1
#define ANJ_BUTTON_SELECT 2
#define ANJ_BUTTON_START  3
#define ANJ_BUTTON_UP     4
#define ANJ_BUTTON_DOWN   5
#define ANJ_BUTTON_LEFT   6
#define ANJ_BUTTON_RIGHT  7

#define ANJ_HEADER_SLEEP  12L
#define ANJ_SLEEP         6L

#if defined(ESP8266) || defined(ESP32)
#include <functional>
#define ANJ_CALLBACK_SIGNATURE std::function<void(void)> _callback
#else
#define ANJ_CALLBACK_SIGNATURE void (*_callback)(void)
#endif

class AsyncNESJoystick
{
public:
    AsyncNESJoystick(int data, int latch, int clock);
    byte readData();
    byte getRegister();
    boolean isButtonPressed(int button);
    String statusButtonsAsString();
    void asyncRead();
    void onChange(ANJ_CALLBACK_SIGNATURE);

private:
    ANJ_CALLBACK_SIGNATURE;
    volatile unsigned long _micros = 0L;
    volatile int _step = 0;
    volatile int _data = 0;
    volatile int _latch = 0;
    volatile int _clock = 0;
    volatile int _button = 0;
    byte _bufferData = 255;
    byte _register;
};

#endif