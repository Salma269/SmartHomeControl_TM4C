#ifndef TEMP_H
#define TEMP_H

#include <stdint.h>
#include <stdbool.h>
#include "../Project Tasks/tm4c123gh6pm.h"  // Include device-specific definitions

// Macros
#define TEMP_THRESHOLD 15.0f  // Temperature threshold in Celsius
#define SYS_CLOCK_FREQ 16000000  

// Global variables
extern volatile uint32_t adcValue;     // ADC conversion result
extern volatile bool alarmTriggered;

// Function Prototypes

/**
 * @brief Reads the temperature value from the LM35 using ADC.
 */
void ReadTemperature(void);

/**
 * @brief Converts an ADC value to temperature in Celsius.
 * @param adcValue The ADC conversion result.
 * @return Temperature in Celsius.
 */
float ADCToCelsius(uint32_t adcValue);

/**
 * @brief Activates the alarm by turning on the buzzer.
 */
void TriggerAlarm(void);

/**
 * @brief Deactivates the alarm by turning off the buzzer.
 */
void ClearAlarm(void);

#endif // TEMP_H
