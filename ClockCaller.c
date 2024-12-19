#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"

// Define the RTC_Time structure
typedef struct {
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
} RTC_Time;

// Timer base values (configured in your Timer0 setup)
#define TIMER_PRESCALER 31 // Prescaler value (32 -> 0.5 MHz clock)
#define TIMER_CLOCK 500000 // Timer frequency after prescaling (0.5 MHz)
#define TIMER_INTERVAL 1   // Timer interval in seconds

RTC_Time GetRTC(void) {
    RTC_Time currentTime;

    // Read the current value of Timer0
    uint32_t currentTimerValue = TIMER0_TAV_R; // Current timer value (32-bit)

    // Total elapsed seconds based on the timer's countdown
    uint32_t secondsElapsed = ((TIMER_CLOCK - currentTimerValue) / TIMER_CLOCK) + elapsed_time_seconds;

    // Convert elapsed seconds into hours, minutes, and seconds
    currentTime.hours = (secondsElapsed / 3600) % 24;
    currentTime.minutes = (secondsElapsed / 60) % 60;
    currentTime.seconds = secondsElapsed % 60;

    return currentTime;
}


void DisplayRTC(char *timeString) {
    RTC_Time time = GetRTC();
    sprintf(timeString, "%02u:%02u:%02u", time.hours, time.minutes, time.seconds);
}

