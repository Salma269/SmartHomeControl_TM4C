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

// Function to read the temperature value from the LM35 using ADC
void ReadTemperature(void) {
  //This register is used to trigger the ADC conversion process
    //When a conversion is triggered, the ADC starts converting an 
    //analog input signal into a digital value, which can then be processed by the microcontroller.
    ADC0_PSSI = 0x08;  // Start the conversion on sequencer 3
    while ((ADC0_RIS & 0x08) == 0);  // Wait for conversion to complete
    adcValue = ADC0_SSFIFO3;  // Get the result from the FIFO
    ADC0_ISC = 0x08;  // Clear the interrupt flag
}

// Function to convert the ADC value to Celsius
float ADCToCelsius(uint32_t adcValue) {
    float voltage = (3.3 * adcValue) / 4096.0;  // Convert ADC value to voltage (assuming 3.3V reference)
    float temperature = voltage * 100.0;  // Convert voltage to temperature (LM35 gives 10mV/°C)
    return temperature;
}

// Function to trigger the alarm (turn on buzzer)
void TriggerAlarm(void) {
    GPIO_PORTE_DATA |= 0x02;  // Set PE1 high to activate the buzzer
    alarmTriggered = true;
}

// Function to clear the alarm (turn off buzzer)
void ClearAlarm(void) {
    GPIO_PORTE_DATA &= ~0x02;  // Set PE1 low to deactivate the buzzer
    alarmTriggered = false;
}
