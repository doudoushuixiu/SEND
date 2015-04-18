
#include "stm8l10x_it.h"

#ifdef _COSMIC_

INTERRUPT_HANDLER(NonHandledInterrupt,0)
{

#endif


INTERRUPT_HANDLER(AWU_IRQHandler,4)
{
 // AWU_GetFlagStatus();
  (FlagStatus)(((uint8_t)(AWU->CSR & AWU_CSR_AWUF) == (uint8_t)0x00) ? RESET : SET);
}

