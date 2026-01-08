#ifndef INC_TEST_COMMUNICATION_TEST_H_
#define INC_TEST_COMMUNICATION_TEST_H_

#include <stdint.h>

typedef enum {
	COMMUNICATION_NO_STOP = 0,
	COMMUNICATION_BUTTON_RESTART = 1,
	COMMUNICATION_ONE_RUN = 2
} CommunicationMode_t;

void CommunicationTest_Init(CommunicationMode_t mode);
void CommunicationTest_Task(void);

#endif /* INC_TEST_COMMUNICATION_TEST_H_ */
