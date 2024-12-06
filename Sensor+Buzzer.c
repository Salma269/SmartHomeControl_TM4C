#include <stdint.h>
#include <stdbool.h>

// Base addresses of the peripherals
#define SYSCTL_RCGC2_GPIOF   (*((volatile uint32_t *)0x400FE108))  // GPIOF Clock Gate Control
#define SYSCTL_RCGC2_GPIOE   (*((volatile uint32_t *)0x400FE108))  // GPIOE Clock Gate Control
#define SYSCTL_RCGC0_ADC0    (*((volatile uint32_t *)0x400FE100))  // ADC0 Clock Gate Control

// GPIO registers
#define GPIO_PORTF_DATA      (*((volatile uint32_t *)0x400253FC))  // GPIO Port F Data
#define GPIO_PORTF_DIR       (*((volatile uint32_t *)0x40025400))  // GPIO Port F Direction
#define GPIO_PORTF_DEN       (*((volatile uint32_t *)0x4002551C))  // GPIO Port F Digital Enable

#define GPIO_PORTE_DIR       (*((volatile uint32_t *)0x40024400))  // GPIO Port E Direction
#define GPIO_PORTE_DEN       (*((volatile uint32_t *)0x4002451C))  // GPIO Port E Digital Enable

// ADC registers
#define ADC0_ACTSS            (*((volatile uint32_t *)0x40038000))  // ADC0 Active Sample Sequencer
#define ADC0_EMUX             (*((volatile uint32_t *)0x40038014))  // ADC0 Event Multiplexer Select
#define ADC0_SSMUX3           (*((volatile uint32_t *)0x400380A0))  // ADC0 Sample Sequencer 3 Input Multiplexer
#define ADC0_SSCTL3           (*((volatile uint32_t *)0x400380A4))  // ADC0 Sample Sequencer 3 Control
#define ADC0_PSSI             (*((volatile uint32_t *)0x40038028))  // ADC0 Processor Sample Sequence Initiate
#define ADC0_RIS              (*((volatile uint32_t *)0x40038004))  // ADC0 Raw Interrupt Status
#define ADC0_ISC              (*((volatile uint32_t *)0x4003800C))  // ADC0 Interrupt Status and Clear
#define ADC0_SSFIFO3          (*((volatile uint32_t *)0x400380A8))  // ADC0 Sample Sequencer 3 FIFO

// SysTick registers
#define STCTRL                 (*((volatile uint32_t *)0xE000E010))  // SysTick Control and Status
#define STRELOAD               (*((volatile uint32_t *)0xE000E014))  // SysTick Reload Value
#define STCURRENT              (*((volatile uint32_t *)0xE000E018))  // SysTick Current Value

// Define constants
#define TEMP_THRESHOLD 30  // Threshold temperature in Celsius
#define SYS_CLOCK_FREQ 16000000  // System clock frequency (16 MHz)

// Global variables
volatile uint32_t adcValue = 0;  // Variable to store ADC value
volatile bool alarmTriggered = false;

// Function to configure the system clock to 16 MHz (using PLL)
void SysClockConfig(void) {
    // Set system clock to 16 MHz using PLL
    *((volatile uint32_t *)0x400FE000) = 0x1;  // Enable PLL
    while ((*((volatile uint32_t *)0x400FE060) & 0x00000040) == 0);  // Wait for PLL to stabilize
}

// Function to configure SysTick timer for periodic interrupt
void SysTickConfig(void) {
    // Set SysTick reload value for 3 seconds (assuming system clock of 16 MHz)
    STRELOAD = (3 * SYS_CLOCK_FREQ) - 1;  // (3 seconds) Reload value
    STCTRL = 0x07;  // Enable SysTick with interrupt and system clock
}

// Function to configure GPIO for buzzer control (PF1)
void GPIOFConfig(void) {
    SYSCTL_RCGC2_GPIOF |= 0x00000020;  // Enable clock for GPIO Port F
    while ((SYSCTL_RCGC2_GPIOF & 0x00000020) == 0);  // Wait for GPIOF to be ready

    GPIO_PORTF_DIR |= 0x02;  // Set PF1 as output (for buzzer control)
    GPIO_PORTF_DEN |= 0x02;  // Enable PF1 as digital I/O
}

// Function to configure GPIO for ADC input (PE3)
void GPIOEConfig(void) {
    SYSCTL_RCGC2_GPIOE |= 0x00000010;  // Enable clock for GPIO Port E
    while ((SYSCTL_RCGC2_GPIOE & 0x00000010) == 0);  // Wait for GPIOE to be ready

    GPIO_PORTE_DIR &= ~0x08;  // Set PE3 as input
    GPIO_PORTE_DEN |= 0x08;   // Enable PE3 as digital input
}

// Function to configure ADC0 to read from PE3
void ADCConfig(void) {
    SYSCTL_RCGC0_ADC0 |= 0x00010000;  // Enable ADC0
    while ((SYSCTL_RCGC0_ADC0 & 0x00010000) == 0);  // Wait for ADC0 to be ready

    ADC0_EMUX = 0x0000;  // Processor-triggered conversions (default)
    ADC0_SSMUX3 = 0x0000;  // Set channel 0 (PE3) as input for sequencer 3
    ADC0_SSCTL3 = 0x0060;  // Enable interrupt and end of sequence
    ADC0_ACTSS |= 0x08;  // Enable sample sequencer 3
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
    GPIO_PORTF_DATA |= 0x02;  // Set PF1 high to activate the buzzer
    alarmTriggered = true;
}

// Function to clear the alarm (turn off buzzer)
void ClearAlarm(void) {
    GPIO_PORTF_DATA &= ~0x02;  // Set PF1 low to deactivate the buzzer
    alarmTriggered = false;
}

// SysTick interrupt handler
void SysTickHandler(void) {
    ReadTemperature();  // Read temperature from LM35
    float temperature = ADCToCelsius(adcValue);

    if (temperature > TEMP_THRESHOLD && !alarmTriggered) {
        TriggerAlarm();  // Trigger buzzer if temperature exceeds threshold
    } else if (temperature <= TEMP_THRESHOLD && alarmTriggered) {
        ClearAlarm();  // Clear buzzer if temperature is below threshold
    }
}

int main(void) {
    SysClockConfig();  // Configure system clock to 16 MHz
    GPIOFConfig();     // Configure PF1 for buzzer
    GPIOEConfig();     // Configure PE3 for ADC input
    ADCConfig();       // Configure ADC
    SysTickConfig();   // Configure SysTick timer for periodic interrupt

    while (1) {
        // The main loop is empty because SysTick interrupt handles the logic
    }

    return 0;
}
