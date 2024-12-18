#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
#include "Temp.h"

uint32_t adcValue = 0;  // Define adcValue with an initial value
bool alarmTriggered = false;  // Define alarmTriggered with an initial value

// Function to read the temperature value from the LM35 using ADC
void ReadTemperature(void) {
    // Trigger the ADC conversion process
    ADC0_PSSI_R = 0x08;  // Start the conversion on sequencer 3
    while ((ADC0_RIS_R & 0x08) == 0);  // Wait for conversion to complete
    adcValue = ADC0_SSFIFO3_R;  // Get the result from the FIFO
    ADC0_ISC_R = 0x08;  // Clear the interrupt flag
}

// Function to convert the ADC value to Celsius
float ADCToCelsius(uint32_t adcValue) {
    float voltage = (3.3 * adcValue) / 4096.0;  // Convert ADC value to voltage (assuming 3.3V reference)
    float temperature = voltage * 100.0;  // Convert voltage to temperature (LM35 gives 10mV/°C)
    return temperature;
}

// Function to trigger the alarm (turn on buzzer)
void TriggerAlarm(void) {
    // Set PE1 low to activate the buzzer
    GPIO_PORTE_DATA_R &= ~0x02;
    alarmTriggered = true;
}

// Function to clear the alarm (turn off buzzer)
void ClearAlarm(void) {
    GPIO_PORTE_DATA_R |= 0x02;    // Set PE1 high to deactivate the buzzer
    alarmTriggered = false;
}