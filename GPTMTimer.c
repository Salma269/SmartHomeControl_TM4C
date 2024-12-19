#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
#include "sysctl.h"
#include "timer.h"

// Global variable to store elapsed time in seconds
volatile uint32_t elapsed_time_seconds = 0;

// Timer interrupt handler
void Timer0A_Handler(void) {
    // Clear the timer interrupt
    TIMER0_ICR_R = TIMER_ICR_TATOCINT;

    // Increment the elapsed time counter
    elapsed_time_seconds++;
}

void ConfigureTimerForClock(void) {
    // Enable the clock for Timer0
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R0;

    // Wait for Timer0 to be ready
    while (!(SYSCTL_PRTIMER_R & SYSCTL_PRTIMER_R0));

    // Disable Timer0 during configuration
    TIMER0_CTL_R &= ~TIMER_CTL_TAEN;

    // Configure Timer0 as a 32-bit periodic timer
    TIMER0_CFG_R = TIMER_CFG_32_BIT_TIMER;
    TIMER0_TAMR_R = TIMER_TAMR_TAMR_PERIOD;

    // Set the prescaler for Timer A
    TIMER0_TAPR_R = 31;

    // Set the reload value for a 1-second interval
    uint32_t timerReload = 500000 - 1; // Calculated for a 0.5 MHz timer clock
    TIMER0_TAILR_R = timerReload;

    // Clear any pending interrupts
    TIMER0_ICR_R = TIMER_ICR_TATOCINT;

    // Enable timeout interrupt for Timer0A
    TIMER0_IMR_R |= TIMER_IMR_TATOIM;

    // Enable Timer0A in NVIC (interrupt 19 corresponds to Timer0A)
    NVIC_EN0_R |= (1 << 19);

    // Enable Timer0A
    TIMER0_CTL_R |= TIMER_CTL_TAEN;
}
