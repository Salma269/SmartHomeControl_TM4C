#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
#include "Temp.h"


// Define the adcValue as a variable, not a constant
volatile uint32_t adcValue = 0;  // This will hold the ADC result
volatile bool alarmTriggered = false;
float simulatedTemperature = 15.0f;  // Start in the middle of the range

// Function to read temperature changes 
uint32_t ReadTemperature1(void) {
    float delta = (rand() % 11 - 5) / 100.0f; 
    simulatedTemperature += delta;

    if (simulatedTemperature > 16.0f) {
        simulatedTemperature = 16.0f;
    } else if (simulatedTemperature < 14.0f) {
        simulatedTemperature = 14.0f;
    }
    adcValue = (uint32_t)((simulatedTemperature / 100.0f) * 4096.0f / 5.0f);
    return adcValue;
}

// Function to read the temperature value from the LM35 using ADC
void ReadTemperature(void) {
    // Trigger the ADC conversion process
    ADC0_PSSI_R = 0x08;  // Start the conversion on sequencer 3
    while ((ADC0_RIS_R & 0x08) == 0);  // Wait for conversion to complete
    adcValue = ADC0_SSFIFO3_R;  // Get the result from the FIFO
    ADC0_ISC_R = 0x08;  // Clear the interrupt flag
    ADCToCelsius(adcValue);
}

// Function to convert the ADC value to Celsius
float ADCToCelsius(uint32_t adcValueNow) {
    // Convert the ADC value to voltage, assuming a 3.3V reference (you can adjust if needed)
    // This line ensures that adcValue is cast to float before calculation
    float voltage = (5.0f * adcValue) / 4096.0f;  // Convert ADC to voltage
    float temperature = voltage * 100.0f;  // LM35 gives 10mV per degree Celsius, so multiply by 100
    return temperature;
}

// Function to trigger the alarm (turn on buzzer)
void TriggerAlarm(void) {
    // Set PE1 low to activate the buzzer
      GPIO_PORTE_DATA_R &= ~0x02;

    alarmTriggered = true;
    UART_SendString("BUZZER TURNED ON\n");
}

// Function to clear the alarm (turn off buzzer)
void ClearAlarm(void) {
    // Set PE1 high to deactivate the buzzer
      GPIO_PORTE_DATA_R |= 0x02;

    alarmTriggered = false;
    UART_SendString("BUZZER TURNED OFF\n");
}


// Function to turn on the buzzer
void Buzzer_On(void) {
    GPIO_PORTE_DATA_R &= ~0x02;  // Set PE1 low to activate the buzzer
    UART_SendString("BUZZER TURNED ON\n");
}

// Function to turn off the buzzer
void Buzzer_Off(void) {
    GPIO_PORTE_DATA_R |= 0x02;  // Set PE1 high to deactivate the buzzer
    UART_SendString("BUZZER TURNED OFF\n");
}
