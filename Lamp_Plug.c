#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Lamp_Plug.h"
#include "InputButton.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_uart.h"
#include "inc/hw_sysctl.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"

void LampOn() {
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0);  // Set PC4 LOW (relay ON)
}

void LampOff() {
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_PIN_4);  // Set PC4 HIGH (relay OFF)
}

void PlugOn() {
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0);  // Set PC5 LOW (relay ON)
}

void PlugOff() {
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, GPIO_PIN_5);  // Set PC5 HIGH (relay OFF)
}


// for testing purposes only here and down
void EnableGreenLED(void) {
    SYSCTL_RCGCGPIO_R |= 0x20;       // Enable clock for Port F
    while ((SYSCTL_PRGPIO_R & 0x20) == 0); // Wait for Port F to be ready

    GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;     // Unlock GPIO Port F
    GPIO_PORTF_CR_R |= (1 << 3);           // Allow changes to PF3
    GPIO_PORTF_DIR_R |= (1 << 3);          // Set PF3 as output
    GPIO_PORTF_DEN_R |= (1 << 3);          // Enable digital function for PF3
}

void TurnGreenLEDOn(void) {
    GPIO_PORTF_DATA_R |= (1 << 3); // Set PF3 high (turn on green LED)
}
void TurnGreenLEDOff(void) {
    GPIO_PORTF_DATA_R &= ~(1 << 3); // Set PF3 low (turn off green LED)
}
