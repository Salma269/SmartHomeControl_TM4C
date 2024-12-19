#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Lamp_Plug.h"
#include "DOOR.h"

void configure_PD0_PD1_PD2_as_input_with_interrupt(void) {
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R3;  // Enable clock for Port D
    while ((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R3) == 0);  // Wait for clock stabilization

    GPIO_PORTD_LOCK_R = GPIO_LOCK_KEY;                        // Unlock Port D
    GPIO_PORTD_CR_R |= (1u << 0) | (1u << 1) | (1u << 2);     // Allow changes to PD0, PD1, and PD2
    GPIO_PORTD_DIR_R &= ~((1u << 0) | (1u << 1) | (1u << 2)); // Set PD0, PD1, and PD2 as inputs
    GPIO_PORTD_DEN_R |= (1u << 0) | (1u << 1) | (1u << 2);    // Enable digital functionality
    GPIO_PORTD_PUR_R |= (1u << 0) | (1u << 1) | (1u << 2);    // Enable pull-up resistors

    GPIO_PORTD_IS_R &= ~((1u << 0) | (1u << 1) | (1u << 2));  // Set to edge-sensitive
    GPIO_PORTD_IBE_R |= (1u << 0) | (1u << 1) | (1u << 2);    // Interrupt on both edges
    GPIO_PORTD_ICR_R = (1u << 0) | (1u << 1) | (1u << 2);     // Clear any prior interrupts
    GPIO_PORTD_IM_R |= (1u << 0) | (1u << 1) | (1u << 2);     // Unmask interrupts for PD0, PD1, and PD2

    NVIC_EN0_R |= (1 << (INT_GPIOD - 16));                    // Enable interrupt in NVIC for Port D
}

// GPIO Port D Interrupt Handler
// GPIO Port D Interrupt Handler
void GPIOPortD_Handler(void) {
    // Handle interrupt for PD0 (Plug Button)
    if (GPIO_PORTD_RIS_R & (1u << 0)) {      // Check if interrupt is from PD0
        GPIO_PORTD_ICR_R = (1u << 0);        // Clear the interrupt flag
        if (GPIO_PORTD_DATA_R & (1u << 0)) {
            // Button pressed (signal HIGH)
            LampOn();
        } else {
            // Button released (signal LOW)
            LampOff();
        }
    }

    // Handle interrupt for PD1 (Lamp Button)
    if (GPIO_PORTD_RIS_R & (1u << 1)) {      // Check if interrupt is from PD1
        GPIO_PORTD_ICR_R = (1u << 1);        // Clear the interrupt flag
        if (GPIO_PORTD_DATA_R & (1u << 1)) {
            // Button pressed (signal HIGH)
            PlugOn();
        } else {
            // Button released (signal LOW)
            PlugOff();
        }
    }

    // Handle interrupt for PD2 (Custom functionality)
    if (GPIO_PORTD_RIS_R & (1u << 2)) {      // Check if interrupt is from PD2
        GPIO_PORTD_ICR_R = (1u << 2);        // Clear the interrupt flag
        if (GPIO_PORTD_DATA_R & (1u << 2)) {
            // Button pressed (signal HIGH)
            // Add specific functionality for PD2 press
          
          TurnGreenLEDOn();
        } else {
            // Button released (signal LOW)
            // Add specific functionality for PD2 release
        TurnGreenLEDOff();
        
        }
    }
}



