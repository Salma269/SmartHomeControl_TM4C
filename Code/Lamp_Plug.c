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
