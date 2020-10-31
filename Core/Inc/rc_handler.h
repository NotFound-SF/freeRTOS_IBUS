/*
 * rc_handler.h
 *
 *  Created on: Oct 24, 2020
 *      Author: st
 */

#ifndef INC_RC_HANDLER_H_
#define INC_RC_HANDLER_H_

#include "stm32f1xx_hal.h"


#define SPEED_CH     2
#define DIRECTION_CH 3
#define FB_SW_CH     4   // Forward and backward switch
#define FL_SW_CH     5   // Headlamp switch


void RcActionHandler();
void initRcActionHandler();

#endif /* INC_RC_HANDLER_H_ */
