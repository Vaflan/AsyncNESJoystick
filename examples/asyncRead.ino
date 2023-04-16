#include <AsyncNESJoystick.h>


//===============================================================================
//  Pin Declarations
//===============================================================================
//Inputs:
#define NES_DATA 3 // The data pin for the NES controller

//Outputs:
#define NES_LATCH 4 // The latch pin for the NES controller
#define NES_CLOCK 2 // The clock pin for the NES controller

AsyncNESJoystick joystick(NES_DATA, NES_LATCH, NES_CLOCK);

byte nesPrevRegister = 255;

//===============================================================================
//  Initialization
//===============================================================================
void setup()
{
  // Initialize serial port speed for the serial terminal
  Serial.begin(9600);
}


//===============================================================================
//  Main
//===============================================================================
void loop()
{
  joystick.asyncRead();
  byte nesRegister = joystick.getRegister();
  if (nesRegister != nesPrevRegister) {
    Serial.println(joystick.statusButtonsAsString());
    nesPrevRegister = nesRegister;
  }
}