#ifndef INC_DESERIALIZE_H_
#define INC_DESERIALIZE_H_

#include <states.h>  // #include <sensors.h>
#include <decision.h> // #include <enums.h>
#include "frame_size.h" 

#include <stddef.h>
#include <stdint.h>

/* Prototipi */
int deserializeLocalStateB1(const uint8_t *buf, size_t len, BUS_LocalStateB1 *state);
int deserializeLocalStateB2(const uint8_t *buf, size_t len, BUS_LocalStateB2 *state);
int deserializeGlobalState(const uint8_t *buf, size_t len, BUS_GlobalState *state);
int deserializeDecision(const uint8_t *buf, size_t len, BUS_Decision *state);

#endif /* INC_DESERIALIZE_H_ */
