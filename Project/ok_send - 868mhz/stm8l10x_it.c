
#include "stm8l10x_it.h"


extern unsigned char   count1;


#ifdef _COSMIC_

INTERRUPT_HANDLER(NonHandledInterrupt,0)
{

#endif


INTERRUPT_HANDLER(AWU_IRQHandler,4)
{
  // AWU_GetFlagStatus();
  (FlagStatus)(((uint8_t)(AWU->CSR & AWU_CSR_AWUF) == (uint8_t)0x00) ? RESET : SET);
}



INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 25)
{
    count1++;

  TIM4_ClearITPendingBit(TIM4_IT_Update);
}
