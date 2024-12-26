#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_uart.h"
#include "inc/hw_sysctl.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"


void Peripheral_Init(void) {
    PortE_Init();
    ADC0_Init();
    PortD_Init();
    SysTick_Init();
    PortC_Init();
}

void PortE_Init(void) {
    SYSCTL_RCGCGPIO_R |= 0x10;        // Enable clock for Port E (bit 4)
    GPIO_PORTE_DIR_R |= 0x02;         // Set PE1 as output for Buzzer
    GPIO_PORTE_DEN_R |= 0x02;         // Enable digital function for PE1
}

void ADC0_Init(void) {
    SYSCTL_RCGCADC_R |= 0x01;         // Enable clock for ADC0
    while ((SYSCTL_PRADC_R & 0x01) == 0);  // Wait for ADC0 to be ready

    ADC0_ACTSS_R &= ~0x08;            // Disable sequencer 3
    ADC0_SSMUX3_R = 0x01;             // Select channel 1 (AIN1 for LM35)
    ADC0_SSCTL3_R = 0x06;             // Configure sequencer 3 for one sample, interrupt on completion (END0)
    ADC0_ACTSS_R |= 0x08;             // Enable sequencer 3
}

void PortD_Init(void) {
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

void SysTick_Init(void) {
    SysTick_InitInterrupt(15999999, SysTick_Callback);  // 1-second delay with a 16 MHz clock
    NVIC_ST_CTRL_R |= 0x07;   // Enable SysTick with system clock and interrupt
}

void PortC_Init(void) {
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R2;  // Enable clock for Port C
    while ((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R2) == 0);  // Wait for Port C to be ready

    GPIO_PORTC_LOCK_R = GPIO_LOCK_KEY;              // Unlock Port C
    GPIO_PORTC_CR_R |= (1U << 4) | (1U << 5);       // Allow changes to PC4 and PC5
    GPIO_PORTC_DIR_R |= (1U << 4) | (1U << 5);      // Set PC4 and PC5 as outputs
    GPIO_PORTC_DEN_R |= (1U << 4) | (1U << 5);      // Enable digital I/O on PC4 and PC5
    GPIO_PORTC_DATA_R |= (1U << 4) | (1U << 5);     // Initially set PC4 and PC5 HIGH (Relays OFF)
}
