#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Lamp_Plug.h"


// Function to initialize Port F
void PortF_Init(void) {
    SYSCTL_RCGCGPIO_R |= 0x20;          // Enable clock for Port F
    while ((SYSCTL_PRGPIO_R & 0x20) == 0);  // Wait for Port F to be ready
    
    GPIO_PORTF_LOCK_R = 0x4C4F434B;     // Unlock GPIO Port F
    GPIO_PORTF_CR_R |= (1U << 1) | (1U << 2);  // Allow changes to PF1 and PF2
    GPIO_PORTF_DIR_R |= (1U << 1) | (1U << 2); // Set PF1 and PF2 as outputs
    GPIO_PORTF_DEN_R |= (1U << 1) | (1U << 2); // Enable digital I/O on PF1 and PF2
    GPIO_PORTF_DATA_R |= (1U << 1) | (1U << 2); // Initially set PF1 and PF2 HIGH (Lamp and Plug OFF)
}


// Function to set PF1 to Low , which turns Off the Lamp
void LampOff(void) {
    GPIO_PORTF_DATA_R |= 0x02;  // Set PF1 high
}

// Function to set PF1 to low, which turns on the Lamp
void LampOn(void) {
    GPIO_PORTF_DATA_R &= ~0x02;  // Set PF1 low
}
// Function to turn PF2 ON, which turns off the plug
void PlugOff(void) {
    GPIO_PORTF_DATA_R |= 0x04;  // Set PF2 high (turn Blue LED ON)
}

// Function to turn PF2 OFF, which turns on the Plug
void PlugOn(void) {
    GPIO_PORTF_DATA_R &= ~0x04;  // Set PF2 low (turn Blue LED OFF)
}