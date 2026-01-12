#ifndef INC_STUB_CRCFUNCTIONS_H_
#define INC_STUB_CRCFUNCTIONS_H_

#include <stdint.h>
//#include <stddef.h>

#define CRC_SIZE (4)

void computeCRC(uint8_t* buffer, uint32_t bufferLength);

uint8_t compareCRC(uint8_t* buffer, uint32_t bufferLength);

#endif /* INC_STUB_CRCFUNCTIONS_H_ */
