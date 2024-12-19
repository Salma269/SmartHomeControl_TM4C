#ifndef INPUTBUTTON_H
#define INPUTBUTTON_H

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Lamp_Plug.h"
#include "DOOR.h"

// Function prototypes for configuring and handling button interrupts
void configure_PD0_PD1_PD2_as_input_with_interrupt(void);
void GPIOPortD_Handler(void);  // Interrupt Service Routine for Port D

#endif // INPUTBUTTON_H

