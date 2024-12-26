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
#include "InputButton.h"  
#include "Lamp_Plug.h" 
#include "Temp.h"
#include "Systick.h"

#define UART0_BASE      0x4000C000
#define UART_BASE UART0_BASE

// Define UART0 Base and GPIO PortA
#define GPIO_PORTA_BASE 0x40004000
// Simulated temperature
volatile uint32_t temperature = 25;
#define GPIO_PA0_U0RX 0x00000001  // GPIO pin for UART0 RX
#define GPIO_PA1_U0TX 0x00000401  // GPIO pin for UART0 TX

// UART0 initialization and interrupt setup
void UART_Init(void) {
    // Enable clock for GPIO Port A and UART0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    // Wait for the peripherals to be ready
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0));

    // Configure GPIO pins PA0 and PA1 for UART
    GPIOPinConfigure(GPIO_PA0_U0RX); // Configure PA0 as UART0 RX
    GPIOPinConfigure(GPIO_PA1_U0TX); // Configure PA1 as UART0 TX
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Configure UART0
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    // Enable UART0
    UARTEnable(UART0_BASE);

    // Enable UART0 receive interrupt
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT); // Enable receive and receive timeout interrupts

    // Enable UART0 interrupt in the NVIC
    IntEnable(INT_UART0); // Enable the interrupt in the NVIC (interrupt controller)

    // Enable global interrupts
    IntMasterEnable();  // Enable global interrupts using Tiva C driver function
}

void UART_SendChar(char c) {
    // Wait until the UART is ready to transmit
    while (*((volatile uint32_t *)(UART0_BASE + 0x018)) & (1 << 5));
    *((volatile uint32_t *)(UART0_BASE + 0x000)) = c;
}

void UART_SendString(const char *str) {
    while (*str) {
        UART_SendChar(*str++);
    }
}

// Function to handle commands from the GUI
void ProcessCommand(const char *command) {
    if (strcmp(command, "LAMP_ON") == 0) {
        LampOn();  // Turn on the lamp
        UART_SendString("LAMP TURNED ON\n");  // Respond back
    } else if (strcmp(command, "LAMP_OFF") == 0) {
        LampOff();  // Turn off the lamp
        UART_SendString("LAMP TURNED OFF\n");  // Respond back
    } else if (strcmp(command, "PLUG_ON") == 0) {
        PlugOn();  // Turn on the plug
        UART_SendString("PLUG TURNED ON\n");
    } else if (strcmp(command, "PLUG_OFF") == 0) {
        PlugOff();  // Turn off the plug
        UART_SendString("PLUG TURNED OFF\n");
    }
}

// Function to simulate sending data to the GUI
void SendDataToGUI(char status[32]) {
    char buffer[32];
    
    // Send door status
    UART_SendString(status);

    // Send temperature
    snprintf(buffer, sizeof(buffer), "TEMP:%d\n", temperature);
    UART_SendString(buffer);

    // Simulate temperature change
    temperature += 1;
    if (temperature > 40) {
        temperature = 25;
    }
}

// UART interrupt handler
void UART0_Handler(void) {
    static char commandBuffer[32];
    static uint32_t commandIndex = 0;
    char data = UARTCharGet(UART0_BASE); // Get the received character

    if (data == '\n' || data == '\r') {  // Command end
        commandBuffer[commandIndex] = '\0';  // Null terminate the command string
        ProcessCommand(commandBuffer);  // Process full command
        commandIndex = 0;  // Reset buffer index for next command
    } else if (commandIndex < sizeof(commandBuffer) - 1) {
        commandBuffer[commandIndex++] = data;  // Add character to buffer
    }
}


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

void Peripheral_Init(void) {
  
    // Enable the clock for Port E (for buzzer is connected)
    SYSCTL_RCGCGPIO_R |= 0x10;        // Enable clock for Port E (bit 4)
    GPIO_PORTE_DIR_R |= 0x02;         // Set PE1 as output for Buzzer
    GPIO_PORTE_DEN_R |= 0x02;         // Enable digital function for PE1
    
    // Enable clock for ADC0 (for sensor)
    SYSCTL_RCGCADC_R |= 0x01;  // Enable clock for ADC0
    while ((SYSCTL_PRADC_R & 0x01) == 0);  // Wait for ADC0 to be ready

    // Disable sequencer 3 (before configuring it)
    ADC0_ACTSS_R &= ~0x08;  // Disable sequencer 3

    // Configure sequencer 3 to read from channel 1 (AIN1, which is LM35)
    ADC0_SSMUX3_R = 0x01;   // Select channel 1 (AIN1 for LM35)
    ADC0_SSCTL3_R = 0x06;   // Configure sequencer 3 for one sample, interrupt on completion (END0)

    // Enable sequencer 3 (start taking samples)
    ADC0_ACTSS_R |= 0x08;  // Enable sequencer 3 
    // Enable clock for Port F
    SYSCTL_RCGCGPIO_R |= 0x20;          // Enable clock for Port F
    while ((SYSCTL_PRGPIO_R & 0x20) == 0);  // Wait for Port F to be ready
    
    // Initialize Port F
    GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;     // Unlock GPIO Port F
    GPIO_PORTF_CR_R |= (1U << 1) | (1U << 2);  // Allow changes to PF1 and PF2
    GPIO_PORTF_DIR_R |= (1U << 1) | (1U << 2); // Set PF1 and PF2 as outputs
    GPIO_PORTF_DEN_R |= (1U << 1) | (1U << 2); // Enable digital I/O on PF1 and PF2
    GPIO_PORTF_DATA_R |= (1U << 1) | (1U << 2); // Initially set PF1 and PF2 HIGH (Lamp and Plug OFF)
    
    // Configure PD0, PD1, PD2 as inputs with interrupt
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
    
    // Initialize SysTick with 1-second delay ( 16 MHz clock)
    SysTick_InitInterrupt(15999999, SysTick_Callback);  // 1-second delay with a 16 MHz clock

    // Enable SysTick interrupt and SysTick timer
    NVIC_ST_CTRL_R |= 0x07;   // Enable SysTick with system clock and interrupt

    // Port C Initialization
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R2;  // Enable clock for Port C
    while ((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R2) == 0);  // Wait for Port C to be ready

    GPIO_PORTC_LOCK_R = GPIO_LOCK_KEY;              // Unlock Port C
    GPIO_PORTC_CR_R |= (1U << 4) | (1U << 5);       // Allow changes to PC4 and PC5
    GPIO_PORTC_DIR_R |= (1U << 4) | (1U << 5);      // Set PC4 and PC5 as outputs
    GPIO_PORTC_DEN_R |= (1U << 4) | (1U << 5);      // Enable digital I/O on PC4 and PC5
    GPIO_PORTC_DATA_R |= (1U << 4) | (1U << 5);     // Initially set PC4 and PC5 HIGH (Relays OFF)
}


int main(void) {
    char commandBuffer[32];
    uint32_t commandIndex = 0;

    // Set system clock to 50 MHz
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    // Initialize UART
    UART_Init();
    
    Peripheral_Init();
    ReadTemperature();

    // Main loop
    while (1) {
        // Check for received characters
        if (UARTCharsAvail(UART_BASE)) {
            char c = UARTCharGet(UART_BASE);

            // End of command
            if (c == '\n' || c == '\r') {
                commandBuffer[commandIndex] = '\0';
                ProcessCommand(commandBuffer);
                commandIndex = 0; // Reset buffer index
            } else if (commandIndex < sizeof(commandBuffer) - 1) {
                commandBuffer[commandIndex++] = c;
            }
        }

        // Periodically send data to GUI
        // SendDataToGUI();
        SysCtlDelay(SysCtlClockGet() / 3); // 1-second delay
    }
}
