#include <Arduino.h>
#include <AsyncNESJoystick.h>

AsyncNESJoystick::AsyncNESJoystick(int data, int latch, int clock)
{
  _data = data;
  pinMode(_data, INPUT);

  _latch = latch;
  digitalWrite(_latch, LOW);
  pinMode(_latch, OUTPUT);

  _clock = clock;
  digitalWrite(_clock, LOW);
  pinMode(_clock, OUTPUT);
}

byte AsyncNESJoystick::readData()
{
  int tempData = 255;

  digitalWrite(_latch, HIGH);
  delayMicroseconds(ANJ_HEADER_SLEEP);
  digitalWrite(_latch, LOW);

  for (int thisBit = 0; thisBit < 9; thisBit++) {
    if (digitalRead(_data) == LOW && thisBit < 8) {
      bitClear(tempData, thisBit);
    }

    digitalWrite(_clock, HIGH);
    delayMicroseconds(ANJ_SLEEP);
    digitalWrite(_clock, LOW);
    delayMicroseconds(ANJ_SLEEP);
  }
  _register = tempData;

  return tempData;
}

byte AsyncNESJoystick::getRegister()
{
  return _register;
}

boolean AsyncNESJoystick::isButtonPressed(int button)
{
  return bitRead(getRegister(), button) == 0;
}

String AsyncNESJoystick::statusButtonsAsString()
{
  return String("#")
    + " A[" + (isButtonPressed(ANJ_BUTTON_A) ? "X" : " ") + "] "
    + " B[" + (isButtonPressed(ANJ_BUTTON_B) ? "X" : " ") + "] "
    + " SL[" + (isButtonPressed(ANJ_BUTTON_SELECT) ? "X" : " ") + "] "
    + " ST[" + (isButtonPressed(ANJ_BUTTON_START) ? "X" : " ") + "] "
    + " U[" + (isButtonPressed(ANJ_BUTTON_UP) ? "X" : " ") + "] "
    + " D[" + (isButtonPressed(ANJ_BUTTON_DOWN) ? "X" : " ") + "] "
    + " L[" + (isButtonPressed(ANJ_BUTTON_LEFT) ? "X" : " ") + "] "
    + " R[" + (isButtonPressed(ANJ_BUTTON_RIGHT) ? "X" : " ") + "] "
    + "#";
}

void AsyncNESJoystick::asyncRead()
{
  if(_micros > micros()) {
    return;
  }

  _micros = micros();
  switch(_step) {
    case 0:
      _bufferData = 255;
      digitalWrite(_latch, HIGH);
      _button = ANJ_BUTTON_A;
      _micros += ANJ_HEADER_SLEEP;
      _step = 1;
      break;
    case 1:
      if (digitalRead(_data) == LOW) {
        bitClear(_bufferData, _button);
      }
      digitalWrite(_latch, LOW);
      _micros += ANJ_SLEEP;
      _step = 2;
      break;
    case 2:
      _button += 1;
      digitalWrite(_clock, HIGH);
      _micros += ANJ_SLEEP;
      _step = 3;
      break;
    case 3:
      if (digitalRead(_data) == LOW && _button <= ANJ_BUTTON_RIGHT) {
        bitClear(_bufferData, _button);
      }
      digitalWrite(_clock, LOW);
      _micros += ANJ_SLEEP;
      _step = (_button > ANJ_BUTTON_RIGHT) ? 4 : 2;
      break;
    case 4:
      bool isDifferent = (_register != _bufferData);
      _register = _bufferData;
      _micros += ANJ_SLEEP;
      _step = 0;
      if (isDifferent) {
        _callback();
      }
      break;
  }
}

void AsyncNESJoystick::onChange(ANJ_CALLBACK_SIGNATURE) {
  this->_callback = _callback;
}