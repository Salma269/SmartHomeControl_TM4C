#ifndef INPUTBUTTON_H
#define INPUTBUTTON_H

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Lamp_Plug.h"

void configure_PD0_as_input_portD(void);
uint8_t read_PD0(void);
void TriggerButton(void);

#endif