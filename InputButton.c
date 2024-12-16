#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Lamp_Plug.h"

// Function to configure PD0 as input
void configure_PD0_as_input_portD(void) {
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R3;  // Enable clock for Port D
    while ((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R3) == 0);  // Wait for clock stabilization

    GPIO_PORTD_LOCK_R = GPIO_LOCK_KEY;  // Unlock Port D if needed
    GPIO_PORTD_CR_R |= 0x01;           // Commit changes to PD0

    GPIO_PORTD_DIR_R &= ~0x01;         // Set PD0 as input
    GPIO_PORTD_DEN_R |= 0x01;          // Enable digital function on PD0
    GPIO_PORTD_PUR_R |= 0x01;          // Enable pull-up resistor (optional)
}

// Function to read the state of PD0
uint8_t read_PD0(void) {
    return (GPIO_PORTD_DATA_R & 0x01) ? 1 : 0;  // Check bit 0 (PD0)
}


void TriggerButton(void){
  if (read_PD0() == 0) {            // When PD0 is LOW
        PlugOn();                // Turn the Red LED ON
    } else {
        PlugOff();               // Turn the Red LED OFF
    }
}
