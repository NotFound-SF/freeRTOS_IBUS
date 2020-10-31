/*
 * ibus.h
 *
 *  Created on: Oct 24, 2020
 *      Author: st
 */

#ifndef INC_IBUS_H_
#define INC_IBUS_H_

#include "stm32f1xx_hal.h"

#define IBUS_FRAME_GAP     7
#define IBUS_CHECKSUM_SIZE (2)
#define IBUS_SERIAL_RX_PACKET_LENGTH (32)
#define IBUS_CHANNELS 14
#define IBUS_DATA_BEGIN     1000
#define IBUS_DATA_END       2000
#define IBUS_DATA_INTERVAL  (IBUS_DATA_END-IBUS_DATA_BEGIN)
#define IBUS_DATA_MIDDLE    (IBUS_DATA_BEGIN+IBUS_DATA_INTERVAL/2)

uint16_t ibusGetChannel(uint8_t idx);
uint16_t ibusGetChannelHundred(uint8_t idx);
void ibusUpdateChannel(const uint8_t *ibusPacket);

#endif /* INC_IBUS_H_ */
