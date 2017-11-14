#include "main.h"

EXTI_InitTypeDef   EXTI_InitStructure;

static __IO uint32_t uwLsiFreq = 0;

__IO uint32_t uwTimingDelay = 0;
__IO uint32_t uwCaptureNumber = 0;
__IO uint32_t uwPeriodValue = 0;


void Delay(__IO uint32_t nTime);
static uint32_t GetLSIFrequency(void);

int main(void)
{
        
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_EXTI);

  
  if (SysTick_Config(SystemCoreClock / 1000))
  { 
    
    while (1);
  }

  
  if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
  {
    
    STM_EVAL_LEDOn(LED3);

    
    RCC_ClearFlag();
  }
  else
  {
    
    STM_EVAL_LEDOff(LED3);
  }
 
  
  uwLsiFreq = GetLSIFrequency();
   
 
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);


  IWDG_SetPrescaler(IWDG_Prescaler_32);

    
  IWDG_SetReload(uwLsiFreq/64);

  
  IWDG_ReloadCounter();

  
  IWDG_Enable();

  while (1)
  {
    
    STM_EVAL_LEDToggle(LED4);
    
    
    Delay(1000);

   
    IWDG_ReloadCounter();  
  }
}


static uint32_t GetLSIFrequency(void)
{
  NVIC_InitTypeDef   NVIC_InitStructure;
  TIM_ICInitTypeDef  TIM_ICInitStructure;
  RCC_ClocksTypeDef  RCC_ClockFreq;

  
  RCC_LSICmd(ENABLE);
  
 
  while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {
  }

 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
  
  
  TIM_RemapConfig(TIM5, TIM5_LSI);


  TIM_PrescalerConfig(TIM5, 0, TIM_PSCReloadMode_Immediate);
  
  
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV8;
  TIM_ICInitStructure.TIM_ICFilter = 0;
  TIM_ICInit(TIM5, &TIM_ICInitStructure);
  
  
  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

 
  TIM_Cmd(TIM5, ENABLE);

  
  TIM5->SR = 0;
    
  
  TIM_ITConfig(TIM5, TIM_IT_CC4, ENABLE);


  while(uwCaptureNumber != 2)
  {
  }
  
  TIM_DeInit(TIM5);


 
  RCC_GetClocksFreq(&RCC_ClockFreq);

  
  if ((RCC->CFGR & RCC_CFGR_PPRE1) == 0)
  { 
    
    return ((RCC_ClockFreq.PCLK1_Frequency / uwPeriodValue) * 8);
  }
  else
  { 
    return (((2 * RCC_ClockFreq.PCLK1_Frequency) / uwPeriodValue) * 8) ;
  }
}


void Delay(__IO uint32_t nTime)
{ 
  uwTimingDelay = nTime;

  while(uwTimingDelay != 0);
}

#ifdef  USE_FULL_ASSERT


void assert_failed(uint8_t* file, uint32_t line)
{

  while (1)
  {
  }
}
#endif

