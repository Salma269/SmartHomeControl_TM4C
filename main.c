#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
#include "Temp.h"
#include "Systick.h"
#include "InputButton.h"  
#include "Lamp_Plug.h"    

// Function Prototypes
void Peripheral_Init(void);


// Callback function for SysTick interrupt
void SysTick_Callback(void) {
    // Toggle the relay on Port E, Pin 1
    ReadTemperature();  // Read temperature from LM35
    float temperature = ADCToCelsius(adcValue);
    if (temperature > TEMP_THRESHOLD && !alarmTriggered) {
        TriggerAlarm();  // Trigger buzzer if temperature exceeds threshold
    } else if (temperature <= TEMP_THRESHOLD && alarmTriggered) {
        ClearAlarm();  // Clear buzzer if temperature is below threshold
    }
}

/**
 * Initializes and enables peripherals such as GPIO, ADC, and SysTick.
 */
void Peripheral_Init(void) {
        // Enable the clock for Port E (for buzzer is connected)
    SYSCTL_RCGCGPIO_R |= 0x10;        // Enable clock for Port E (bit 4)
    GPIO_PORTE_DIR_R |= 0x02;         // Set PE1 as output for Buzzer
    GPIO_PORTE_DEN_R |= 0x02;         // Enable digital function for PE1

    // Enable clock for ADC0 (for sensor)
    SYSCTL_RCGCADC_R |= 0x01;         // Enable clock for ADC0
    while ((SYSCTL_PRADC_R & 0x01) == 0);  // Wait for ADC0 to be ready
    ADC0_CTL_R &= ~0x00000001;        // Disable ADC0 sequencer 3
    ADC0_SSMUX3_R = 0x00000001;       // Select channel 1 (AIN1 for LM35)
    ADC0_SSCTL3_R = 0x0006;           // Configure sequencer 3 (single sample, interrupt on completion)
    ADC0_ACTSS_R |= 0x08;             // Enable sequencer 3

    // Initialize SysTick with 1-second delay ( 16 MHz clock)
    SysTick_InitInterrupt(15999999, SysTick_Callback);  // 1-second delay with a 16 MHz clock

    // Enable SysTick interrupt and SysTick timer
    NVIC_ST_CTRL_R |= 0x07;   // Enable SysTick with system clock and interrupt
    //__enable_irq();           // Enable global interrupts
}

//pins used
//PF1 for Lamp
//PF2 for plug//PD0 for Plug Button
//PD1 for Lamp Button



int main(void) {
    
    // Initialize peripherals
    Peripheral_Init();
    PortF_Init();  // Initialize Port F
    configure_PD0_PD1_PD2_as_input_with_interrupt(); // Configure PD0 and PD1 with interrupts;         //Initialize portD

    // Main loop - SysTick interrupt will handle temperature checking and buzzer
    while (1) {
    }
}
