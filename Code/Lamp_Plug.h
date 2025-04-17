#ifndef LAMP_PLUG_H
#define LAMP_PLUG_H

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "SysTick.h"
#include "InputButton.h"

void PlugOn(void);   // Connect to PC5 on relay
void PlugOff(void);
void LampOn(void);   // Connect to PC4 on relay
void LampOff(void);


#endif // LAMP_PLUG_H
