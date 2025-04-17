#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Lamp_Plug.h"
#include "DOOR.h"

// GPIO Port D Interrupt Handler
void GPIOPortD_Handler(void) {
    // Handle interrupt for PD0 (lamp Button)
    if (GPIO_PORTD_RIS_R & (1u << 0)) {      // Check if interrupt is from PD0
        GPIO_PORTD_ICR_R = (1u << 0);        // Clear the interrupt flag
        if (GPIO_PORTD_DATA_R & (1u << 0)) {
            // Button pressed (signal HIGH)
            LampOn();
        } else {
            // Button released (signal LOW)
            LampOff();
        }
    }

    // Handle interrupt for PD1 (plug Button)
    if (GPIO_PORTD_RIS_R & (1u << 1)) {      // Check if interrupt is from PD1
        GPIO_PORTD_ICR_R = (1u << 1);        // Clear the interrupt flag
        if (GPIO_PORTD_DATA_R & (1u << 1)) {
            // Button pressed (signal HIGH)
            PlugOn();
        } else {
            // Button released (signal LOW)
            PlugOff();
        }
    }

    // Handle interrupt for PD2 (Door functionality)
    if (GPIO_PORTD_RIS_R & (1u << 2)) {      // Check if interrupt is from PD2
        GPIO_PORTD_ICR_R = (1u << 2);        // Clear the interrupt flag
        doorStatus = Get_Door_Status();
        if (doorStatus) {
            // Button pressed (signal HIGH)
            SendDataToGUI("DOOR:CLOSED\n");
        } else {
            // Button released (signal LOW)
            SendDataToGUI("DOOR:OPEN\n");
        }
    }
    
}
