// door_status.c
#include "DOOR.h"
#include "GPIO.h" // Assuming GPIO functions are used for door status detection

// Define door GPIO pin mapping (example)

// Door status initialization function
void DoorStatus_Init(void) {
    // Initialize the GPIO pin for the door sensor
    GPIO_Init(DOOR_PORT, DOOR_PIN, INPUT, DIGITAL);
}

// Function to get the status of the door
uint8_t Get_Door_Status(void) {
    // Read and return the door status
    return GPIO_ReadPin(DOOR_PORT, DOOR_PIN);
}
