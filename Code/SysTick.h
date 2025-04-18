#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Callback function for the SysTick interrupt to handle periodic tasks.
 */
//void SysTick_Callback(void);

// Function prototypes
void SysTick_Init(uint32_t reloadValue);
void SysTick_Enable(void);
void SysTick_Disable(void);
void SysTick_PeriodSet(uint32_t ui32Period);
uint32_t SysTick_PeriodGet(void);
uint32_t SysTick_ValueGet(void);
bool SysTick_Is_Time_Out(void);

// Function prototypes for interrupt mode
void SysTick_InitInterrupt(uint32_t reloadValue, void (*callback)(void));
#endif // SYSTICK_H
