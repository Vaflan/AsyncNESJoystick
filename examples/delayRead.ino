#include <AsyncNESJoystick.h>

#define NES_DATA 3
#define NES_LATCH 4
#define NES_CLOCK 2

AsyncNESJoystick joystick(NES_DATA, NES_LATCH, NES_CLOCK);

byte nesPrevRegister = 255;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  byte nesRegister = joystick.readData();
  if (nesRegister != nesPrevRegister) {
    Serial.println(joystick.statusButtonsAsString());
    nesPrevRegister = nesRegister;
  }
}