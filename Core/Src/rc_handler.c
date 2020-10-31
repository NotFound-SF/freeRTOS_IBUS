#include "rc_handler.h"
#include "tim.h"
#include "ibus.h"


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
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
}

static uint8_t isSwitchOn(uint8_t sw)
{
  return IBUS_DATA_END == ibusGetChannel(sw);
}


static void leftControl(uint16_t speed, enum DIRECTION dirc)
{
  if (dirc == DIRC_FORWARD) {
    setDutyRatio(&htim2, TIM_CHANNEL_1, speed);
    setDutyRatio(&htim2, TIM_CHANNEL_2, 0);
  } else {
    setDutyRatio(&htim2, TIM_CHANNEL_1, 0);
    setDutyRatio(&htim2, TIM_CHANNEL_2, speed);
  }
}

static void rightControl(uint16_t speed, enum DIRECTION dirc)
{
  if (dirc == DIRC_FORWARD) {
    setDutyRatio(&htim2, TIM_CHANNEL_3, 0);
    setDutyRatio(&htim2, TIM_CHANNEL_4, speed);
  } else {
    setDutyRatio(&htim2, TIM_CHANNEL_3, speed);
    setDutyRatio(&htim2, TIM_CHANNEL_4, 0);
  }
}

static void forward(uint16_t speed)
{
  leftControl(speed, DIRC_FORWARD);
  rightControl(speed, DIRC_FORWARD);
}


static void back(uint16_t speed)
{
  leftControl(speed, DIRC_BACK);
  rightControl(speed, DIRC_BACK);
}

static void turnRoundForward(uint16_t direction, uint16_t speed)
{
  uint16_t deviation;
  uint16_t internalSpeed;
  if (IBUS_DATA_MIDDLE > direction) {
    deviation = IBUS_DATA_MIDDLE - direction;
    internalSpeed = (((IBUS_DATA_MIDDLE-IBUS_DATA_BEGIN)-deviation)*speed/(IBUS_DATA_MIDDLE-IBUS_DATA_BEGIN));
    rightControl(speed, DIRC_FORWARD);
    leftControl(internalSpeed, DIRC_FORWARD);
  } else {
    deviation = direction - IBUS_DATA_MIDDLE;
    internalSpeed = (((IBUS_DATA_MIDDLE-IBUS_DATA_BEGIN)-deviation)*speed/(IBUS_DATA_MIDDLE-IBUS_DATA_BEGIN));
    leftControl(speed, DIRC_FORWARD);
    rightControl(internalSpeed, DIRC_FORWARD);
  }
}

static void turnRoundBack(uint16_t direction, uint16_t speed)
{
  uint16_t deviation;
  uint16_t internalSpeed;
  if (IBUS_DATA_MIDDLE > direction) {
    deviation = IBUS_DATA_MIDDLE - direction;
    internalSpeed = (((IBUS_DATA_MIDDLE-IBUS_DATA_BEGIN)-deviation)*speed/(IBUS_DATA_MIDDLE-IBUS_DATA_BEGIN));
    rightControl(speed, DIRC_BACK);
    leftControl(internalSpeed, DIRC_BACK);
  } else {
    deviation = direction - IBUS_DATA_MIDDLE;
    internalSpeed = (((IBUS_DATA_MIDDLE-IBUS_DATA_BEGIN)-deviation)*speed/(IBUS_DATA_MIDDLE-IBUS_DATA_BEGIN));
    leftControl(speed, DIRC_BACK);
    rightControl(internalSpeed, DIRC_BACK);
  }
}

// #include "usart.h"
void RcActionHandler()
{
  uint16_t speed = ibusGetChannelHundred(SPEED_CH);
  uint16_t direction = ibusGetChannel(DIRECTION_CH);
/*
  uint8_t buf[2];
  buf[0] = direction>>8;
  buf[1] = direction&0xFF;
*/
  if (isSwitchOn(FL_SW_CH)) {
    HAL_GPIO_WritePin(Light1_GPIO_Port, Light1_Pin, GPIO_PIN_SET);
  } else {
    HAL_GPIO_WritePin(Light1_GPIO_Port, Light1_Pin, GPIO_PIN_RESET);
  }

  if (direction == 0) {
    if (isSwitchOn(FB_SW_CH)) {
      HAL_GPIO_WritePin(Light2_GPIO_Port, Light2_Pin, GPIO_PIN_SET);
      back(speed);
    } else {
      HAL_GPIO_WritePin(Light2_GPIO_Port, Light2_Pin, GPIO_PIN_RESET);
      forward(speed);
    }
  } else {
    if (isSwitchOn(FB_SW_CH)) {
      HAL_GPIO_WritePin(Light2_GPIO_Port, Light2_Pin, GPIO_PIN_SET);
      turnRoundBack(direction, speed);
    } else {
      HAL_GPIO_WritePin(Light2_GPIO_Port, Light2_Pin, GPIO_PIN_RESET);
      turnRoundForward(direction, speed);
    }
  }

//  HAL_UART_Transmit(&huart1, buf, 2, 60);
}
