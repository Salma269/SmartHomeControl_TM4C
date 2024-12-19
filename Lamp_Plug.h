#ifndef LAMP_PLUG_H
#define LAMP_PLUG_H

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Lamp_Plug.h"
#include "SysTick.h"
#include "InputButton.h"

void PortF_Init(void); //portF initialization
void InitializePortF_Lamp_Plug_Button(void); //the start
void PlugOn(void);   // Connect to PF2 on relay, remember it is Active Low, but accounted for in the Function
void PlugOff(void);
void LampOn(void);   // Connect to PF1 on relay, same
void LampOff(void);


//for testing purposes only
void EnableGreenLED(void);
void TurnGreenLEDOn(void);
void TurnGreenLEDOff(void);

#endif // LAMP_PLUG_H
