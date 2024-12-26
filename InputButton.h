#ifndef INPUTBUTTON_H
#define INPUTBUTTON_H

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Lamp_Plug.h"
#include "DOOR.h"
extern volatile int doorStatus;

// Function prototypes for configuring and handling button interrupts
void GPIOPortD_Handler(void);  // Interrupt Service Routine for Port D

#endif // INPUTBUTTON_H

