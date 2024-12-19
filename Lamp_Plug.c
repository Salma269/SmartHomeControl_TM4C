#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Lamp_Plug.h"
#include "InputButton.h"

// Function to initialize Port F
void PortF_Init(void) {
    SYSCTL_RCGCGPIO_R |= 0x20;          // Enable clock for Port F
    while ((SYSCTL_PRGPIO_R & 0x20) == 0);  // Wait for Port F to be ready
    
    GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;     // Unlock GPIO Port F
    GPIO_PORTF_CR_R |= (1U << 1) | (1U << 2);  // Allow changes to PF1 and PF2
    GPIO_PORTF_DIR_R |= (1U << 1) | (1U << 2); // Set PF1 and PF2 as outputs
    GPIO_PORTF_DEN_R |= (1U << 1) | (1U << 2); // Enable digital I/O on PF1 and PF2
    GPIO_PORTF_DATA_R |= (1U << 1) | (1U << 2); // Initially set PF1 and PF2 HIGH (Lamp and Plug OFF)
}


// Function to set PF1 HIGH, which turns Off the Lamp
void LampOff(void) {
    GPIO_PORTF_DATA_R |= 0x02;  // Set PF1 high
}

// Function to set PF1 LOW, which turns on the Lamp
void LampOn(void) {
    GPIO_PORTF_DATA_R &= ~0x02;  // Set PF1 low
}

// Function to turn PF2 ON, which turns off the plug
void PlugOff(void) {
    GPIO_PORTF_DATA_R |= 0x04;  // Set PF2 high
}

// Function to turn PF2 OFF, which turns on the Plug
void PlugOn(void) {
    GPIO_PORTF_DATA_R &= ~0x04;  // Set PF2 low
}

// for testing purposes only here and down
void EnableGreenLED(void) {
    SYSCTL_RCGCGPIO_R |= 0x20;       // Enable clock for Port F
    while ((SYSCTL_PRGPIO_R & 0x20) == 0); // Wait for Port F to be ready

    GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;     // Unlock GPIO Port F
    GPIO_PORTF_CR_R |= (1 << 3);           // Allow changes to PF3
    GPIO_PORTF_DIR_R |= (1 << 3);          // Set PF3 as output
    GPIO_PORTF_DEN_R |= (1 << 3);          // Enable digital function for PF3
}

void TurnGreenLEDOn(void) {
    GPIO_PORTF_DATA_R |= (1 << 3); // Set PF3 high (turn on green LED)
}
void TurnGreenLEDOff(void) {
    GPIO_PORTF_DATA_R &= ~(1 << 3); // Set PF3 low (turn off green LED)
}

