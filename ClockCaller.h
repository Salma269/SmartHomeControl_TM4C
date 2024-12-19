#ifndef CLOCKCALLER_H
#define CLOCKCALLER_H

#include <stdint.h>

// RTC Time structure to represent hours, minutes, and seconds
typedef struct {
    uint8_t hours;   // 0-23
    uint8_t minutes; // 0-59
    uint8_t seconds; // 0-59
} RTC_Time;

// Function to calculate the current RTC time from the Timer0
RTC_Time GetRTC(void);

// Function to get the current RTC time as a formatted string ("HH:MM:SS")
void DisplayRTC(char *timeString);

#endif // CLOCKCALLER_H

