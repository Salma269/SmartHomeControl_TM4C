#ifndef TEMPERATURE_ALARM_H
#define TEMPERATURE_ALARM_H

#include <stdint.h>
#include <stdbool.h>

// Define macros for temperature threshold and GPIO pins
#define TEMP_THRESHOLD 30.0  // Temperature threshold in Celsius
#define Pin1 1

// Global variables
extern uint32_t adcValue;     // Holds the ADC conversion result
extern bool alarmTriggered;  // Keeps track of the alarm state

// Function declarations

/**
 * @brief SysTick callback function for periodic interrupt handling.
 * Toggles the relay and manages temperature-based alarm activation.
 */
void SysTick_Callback(void);

/**
 * @brief Reads the temperature value from the LM35 sensor using ADC.
 * Starts an ADC conversion, waits for completion, and stores the result.
 */
void ReadTemperature(void);

/**
 * @brief Converts the ADC value to temperature in Celsius.
 * @param adcValue The ADC conversion result.
 * @return The temperature in Celsius.
 */
float ADCToCelsius(uint32_t adcValue);

/**
 * @brief Activates the alarm (turns on the buzzer).
 */
void TriggerAlarm(void);

/**
 * @brief Deactivates the alarm (turns off the buzzer).
 */
void ClearAlarm(void);

/**
 * @brief Initializes the relay on the specified port and pin.
 * @param port The port identifier (e.g., 'F').
 * @param pin The pin number to be initialized for the relay.
 */
void Relay_Init(char port, uint8_t pin);

/**
 * @brief Configures the system clock to 16 MHz.
 */
void SysClockConfig(void);

/**
 * @brief Configures GPIO Port F for the buzzer (PF1).
 */
void GPIOFConfig(void);

/**
 * @brief Configures GPIO Port E for ADC input (PE3).
 */
void GPIOEConfig(void);

/**
 * @brief Configures the ADC module for temperature sensor input.
 */
void ADCConfig(void);

/**
 * @brief Configures the SysTick timer for periodic interrupts.
 * @param reloadValue The reload value for the SysTick timer.
 * @param callback The function to be called on each SysTick interrupt.
 */
void SysTick_InitInterrupt(uint32_t reloadValue, void (*callback)(void));

#endif // TEMPERATURE_ALARM_H
