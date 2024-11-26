#include <stdint.h>
#include "tm4c.h"
#include "systick.h"
#include "lcd.h"

// Function prototypes
void PortF_Init(void);
void PortA_Init(void);
void TempSensor_Init(void);
void Relay_Init(void);
void DoorSensor_Init(void);
void Alarm_Init(void);
uint32_t Read_Temperature(void);
void Check_Alarm(void);

// Global Variables
volatile uint32_t temperature = 0;
volatile uint8_t doorStatus = 0; // 0 = Closed, 1 = Open
volatile uint8_t alarmEnabled = 1;
volatile uint32_t tempThreshold = 30; // Set temperature threshold

int main(void) {
    SysTick_Init();
    PortF_Init();
    PortA_Init();
    TempSensor_Init();
    Relay_Init();
    DoorSensor_Init();
    Alarm_Init();
    LCD_Init();

    while (1) {
        // Update door status
        doorStatus = GPIO_PORTA_DATA_R & 0x01; // Magnetic switch on PA0
        if (doorStatus) {
            LCD_DisplayString("Door: OPEN");
        } else {
            LCD_DisplayString("Door: CLOSED");
        }

        // Read and display temperature
        temperature = Read_Temperature();
        LCD_SetCursor(1, 0);
        LCD_DisplayInteger(temperature);
        LCD_DisplayString("C");

        // Check for alarm
        Check_Alarm();
        
        SysTick_Wait10ms(100); // Delay for smooth updates
    }
}

void PortF_Init(void) {
    SYSCTL_RCGCGPIO_R |= 0x20; // Enable clock for Port F
    GPIO_PORTF_DIR_R |= 0x0E;  // PF1, PF2, PF3 as output
    GPIO_PORTF_DEN_R |= 0x0E;
}

void PortA_Init(void) {
    SYSCTL_RCGCGPIO_R |= 0x01; // Enable clock for Port A
    GPIO_PORTA_DIR_R &= ~0x01; // PA0 as input
    GPIO_PORTA_DEN_R |= 0x01;  // Enable PA0
}

void TempSensor_Init(void) {
    SYSCTL_RCGCADC_R |= 0x01; // Enable ADC0
    ADC0_ACTSS_R &= ~0x08;    // Disable SS3 during config
    ADC0_EMUX_R |= 0x5000;    // Timer trigger conversion
    ADC0_SSMUX3_R = 0;        // Set AIN0
    ADC0_SSCTL3_R |= 0x06;    // Enable temp sensor
    ADC0_ACTSS_R |= 0x08;     // Enable SS3
}

uint32_t Read_Temperature(void) {
    ADC0_PSSI_R = 0x08; // Start sampling
    while ((ADC0_RIS_R & 0x08) == 0);
    uint32_t result = ADC0_SSFIFO3_R;
    ADC0_ISC_R = 0x08; // Clear flag
    return (1475 - ((2475 * result) / 4096)) / 10; // Convert to Celsius
}

void Check_Alarm(void) {
    if (temperature > tempThreshold && alarmEnabled) {
        GPIO_PORTF_DATA_R |= 0x02; // Turn on red LED (PF1)
    } else {
        GPIO_PORTF_DATA_R &= ~0x02; // Turn off red LED
    }
}
