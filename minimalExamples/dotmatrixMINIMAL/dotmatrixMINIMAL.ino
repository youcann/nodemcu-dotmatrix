// Program to demonstrate the MD_Parola library
//
// Simplest program that does something useful - Hello World!
//
// MD_MAX72XX library can be found at https://github.com/MajicDesigns/MD_MAX72XX
//

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 8

#define CLK_PIN   D7
#define DATA_PIN  D5
#define CS_PIN    D6

// Hardware SPI connection
//MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
// Arbitrary output pins
 MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);


//mymurks
uint8_t degC[] = { 6, 3, 3, 56, 68, 68, 68 }; // Deg C

void setup(void)
{
  P.begin();
  P.setIntensity(1);
  P.addChar('$', degC);
  P.displayText("12:34  22.45$", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
}

void loop(void)
{
  P.displayAnimate();
}
