#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Lamp_Plug.h"

// Configure PD0 and PD1 as input and enable interrupts
void configure_PD0_PD1_as_input_with_interrupt(void) {
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R3;  // Enable clock for Port D
    while ((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R3) == 0);  // Wait for clock stabilization

    GPIO_PORTD_LOCK_R = GPIO_LOCK_KEY;       // Unlock Port D
    GPIO_PORTD_CR_R |= 0x03;                // Allow changes to PD0 and PD1
    GPIO_PORTD_DIR_R &= ~0x03;              // Set PD0 and PD1 as inputs
    GPIO_PORTD_DEN_R |= 0x03;               // Enable digital functionality
    GPIO_PORTD_PUR_R |= 0x03;               // Enable pull-up resistors

    GPIO_PORTD_IS_R &= ~0x03;               // Set to edge-sensitive
    GPIO_PORTD_IBE_R &= ~0x03;              // Interrupt on one edge
    GPIO_PORTD_IEV_R &= ~0x03;              // Falling edge trigger
    GPIO_PORTD_ICR_R = 0x03;                // Clear any prior interrupts
    GPIO_PORTD_IM_R |= 0x03;                // Unmask interrupts for PD0 and PD1

    NVIC_EN0_R |= (1 << (INT_GPIOD - 16));  // Enable interrupt in NVIC for Port D
}

// GPIO Port D Interrupt Handler
void GPIOPortD_Handler(void) {
    if (GPIO_PORTD_RIS_R & 0x01) {          // Check if interrupt is from PD0
        GPIO_PORTD_ICR_R = 0x01;            // Clear the interrupt flag
        TriggerButtonForPlug();             // Handle Plug button
    }
    if (GPIO_PORTD_RIS_R & 0x02) {          // Check if interrupt is from PD1
        GPIO_PORTD_ICR_R = 0x02;            // Clear the interrupt flag
        TriggerButtonForLamp();             // Handle Lamp button
    }
}
