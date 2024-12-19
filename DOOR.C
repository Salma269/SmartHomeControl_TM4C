// door_status.c
#include "DOOR.h"

// Function to get the status of the door
uint8_t Get_Door_Status(void) {
    // Read and return the door status
  return (GPIO_PORTD_DATA_R & (1u << 2)) ? 1 : 0;
