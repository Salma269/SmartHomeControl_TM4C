#ifndef LAMP_PLUG_H
#define LAMP_PLUG_H

#include <stdint.h>
#include "tm4c123gh6pm.h"

void PlugOn(void);   // Connect to PF2 on relay, remember it is Active Low, but accounted for in the Function
void PlugOff(void);
void LampOn(void);   // Connect to PF1 on relay, same
void LampOff(void);

#endif // LAMP_PLUG_H
