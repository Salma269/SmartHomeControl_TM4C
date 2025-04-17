#ifndef PERIPHERAL_H
#define PERIPHERAL_H

void PortE_Init(void);
void ADC0_Init(void);
void PortD_Init(void);
void PortC_Init(void);
void SysTick_InitInterrupt(uint32_t reloadValue, void (*callback)(void));
void Peripheral_Init(void);

#endif // PERIPHERAL_H
