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


uint8_t isChecksumOkIa6b(const uint8_t *ibusPacket);

#endif /* INC_IBUS_H_ */
