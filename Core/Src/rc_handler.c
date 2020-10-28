#include "rc_handler.h"
#include "tim.h"
#include "ibus.h"


static void initPwm()
{
  TIM_OC_InitTypeDef pwmConf;

  pwmConf.OCMode = TIM_OCMODE_PWM1;
  pwmConf.Pulse = 0;
  pwmConf.OCPolarity = TIM_OCPOLARITY_HIGH;
  pwmConf.OCFastMode = TIM_OCFAST_DISABLE;

  HAL_TIM_PWM_ConfigChannel(&htim2, &pwmConf, TIM_CHANNEL_1);
  HAL_TIM_PWM_ConfigChannel(&htim2, &pwmConf, TIM_CHANNEL_2);
  HAL_TIM_PWM_ConfigChannel(&htim3, &pwmConf, TIM_CHANNEL_1);
  HAL_TIM_PWM_ConfigChannel(&htim3, &pwmConf, TIM_CHANNEL_2);
}

static void setDutyRatio(TIM_HandleTypeDef *htim, uint32_t channel, uint16_t value)
{
  uint32_t period = htim->Init.Period+1;
  uint32_t pluse=(value * period)/100;

  switch (channel) {
    case TIM_CHANNEL_1:
      htim->Instance->CCR1 = pluse;
      break;
    case TIM_CHANNEL_2:
      htim->Instance->CCR2 = pluse;
      break;
    case TIM_CHANNEL_3:
      htim->Instance->CCR3 = pluse;
      break;
    case TIM_CHANNEL_4:
      htim->Instance->CCR4 = pluse;
      break;
    default:
      break;
  }
}

void initRcActionHandler()
{
  initPwm();

  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
}



// debug
#include "usart.h"
void RcActionHandler()
{

  uint16_t speed = ibusGetChannelHundred(SPEED_CH);

  uint8_t buf[2];
  buf[0] = speed>>8;
  buf[1] = speed&0xFF;

  HAL_UART_Transmit(&huart1, buf, 2, 60);

  setDutyRatio(&htim2, TIM_CHANNEL_1, speed);
  setDutyRatio(&htim2, TIM_CHANNEL_2, 30);
  setDutyRatio(&htim3, TIM_CHANNEL_1, 40);
  setDutyRatio(&htim3, TIM_CHANNEL_2, 50);

}
