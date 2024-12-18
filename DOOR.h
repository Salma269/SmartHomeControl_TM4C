// door_status.h
#ifndef DOOR_STATUS_H
#define DOOR_STATUS_H

#include <stdint.h>

#define DOOR_PIN 2
#define DOOR_PORT GPIO_PORTF_BASE



// Door status initialization function
void DoorStatus_Init(void);

// Function to get the status of the door
uint8_t Get_Door_Status(void);

#endif // DOOR_STATUS_H
