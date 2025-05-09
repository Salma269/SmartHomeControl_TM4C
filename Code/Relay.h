#ifndef RELAY_H
#define RELAY_H

#include <stdint.h>
#include "DIO.h"

// Relay States
#define RELAY_ON  1
#define RELAY_OFF 0

// Function Prototypes
void Relay_Init(char port, uint8_t pins);
void Relay_Control(char port, uint8_t pin, uint8_t state);
void Relay_Toggle(char port, uint8_t pin);

#endif // RELAY_H
