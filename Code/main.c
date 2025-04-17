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
#include "Peripheral.h"  
#include "tm4c123gh6pm.h"


#define UART0_BASE      0x4000C000
#define UART_BASE UART0_BASE

volatile int doorStatus = 0; // Initially, the door is open 

// Define UART0 Base and GPIO PortA
#define GPIO_PORTA_BASE 0x40004000

#define GPIO_PA0_U0RX 0x00000001  // GPIO pin for UART0 RX
#define GPIO_PA1_U0TX 0x00000401  // GPIO pin for UART0 TX

extern volatile float currentTemperature = 15.0f; 
volatile uint32_t temperature = 15;


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
    } else if (strcmp(command, "BUZZER_ON") == 0) {
        TriggerAlarm();
    } else if (strcmp(command, "BUZZER_OFF") == 0) {
        ClearAlarm();
    }
}

void SendDataToGUI(const char *status) {
  
    if (strlen(status) >= 32) {
        UART_SendString("ERROR: Status message too long\n");
        return;
    }
    // Send door status
    UART_SendString(status);
    
    temperature += 1;
    if (temperature > 40) {
        temperature = 25;
    }
    // Send updated temperature to the GUI
    char temperatureBuffer[32];
    snprintf(temperatureBuffer, sizeof(temperatureBuffer), "TEMP:%d\n", temperature);
    UART_SendString(temperatureBuffer);  // Send the temperature message via UART
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

void SysTick_Callback_Func(void) {
    // Read the current temperature (ADC value)
    uint32_t adcValueNow = ReadTemperature1();  
    float temperature = ADCToCelsius(adcValueNow);  // Convert ADC value to Celsius
    currentTemperature = temperature;  // Store the current temperature

    // Send updated temperature to the GUI
    char temperatureBuffer[32];
    snprintf(temperatureBuffer, sizeof(temperatureBuffer), "TEMP:%.2f\n", temperature);
    UART_SendString(temperatureBuffer);  // Send the temperature message via UART

    // Check for alarm conditions
    if (temperature > TEMP_THRESHOLD && !alarmTriggered) {
        TriggerAlarm();  // Turn on the alarm if temperature exceeds threshold
    } else if (temperature <= TEMP_THRESHOLD && alarmTriggered) {
        ClearAlarm();  // Turn off the alarm if temperature is below threshold
    }
}


int main(void) {
    char commandBuffer[32];
    uint32_t commandIndex = 0;

    // Set system clock to 50 MHz
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    // Initialize UART
    UART_Init();
    
    Peripheral_Init();

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
        // Send the current door status to the GUI
        // Update door status periodically
            // Door status has changed, notify the GUI
            if (doorStatus) {
                SendDataToGUI("DOOR:OPEN\n");
            } else {
                SendDataToGUI("DOOR:CLOSED\n");
            }
            doorStatus = Get_Door_Status(); // Update global door status

        // Periodically send data to GUI
        // SendDataToGUI();
        SysCtlDelay(SysCtlClockGet() / 3); // 1-second delay
    }
}
