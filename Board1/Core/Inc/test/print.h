#ifndef INC_PRINT_H_
#define INC_PRINT_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "frame_size.h"  // normalmente incluso in serialize/deserialize
#include "decision.h"    // #include "enums.h"
#include "states.h"      // #include "sensors.h"

/* Escape ANSI */
#define CLRSCR "\033[2J\033[H"

/* -------------------------------------------------- */
/* Configurazione printer                             */
/* -------------------------------------------------- */
#include "usart.h"
void setPrinterHandler(UART_HandleTypeDef *huart);
UART_HandleTypeDef* getPrinterHandler(void);

/* -------------------------------------------------- */
/* Base / low-level                                   */
/* -------------------------------------------------- */
void clearScreen(void);
void uartPrint(const char *msg);                 /* alias comodo: stampa stringa */

/* -------------------------------------------------- */
/* Primitivi (stampe testabili)                        */
/* -------------------------------------------------- */
void printMsg_impl(const char *pData);
void printInt(int32_t v);
void printFloat(float v, uint8_t decimals);

/* Macro richiesta: printMsg(...) */
#define printMsg(msg)   printMsg_impl((msg))

/* -------------------------------------------------- */
/* Debug                                               */
/* -------------------------------------------------- */
void UART_PrintFrameSizes(void);
void uartPrintBuffHex(const char *tag, const uint8_t *buf, uint32_t len);

/* -------------------------------------------------- */
/* Elementari su campi/pezzi di strutture (pubbliche) */
/* -------------------------------------------------- */
void printHeader(const char *title);
void printLabel(const char *label);
void printNewLine(void);

/* Elementari LocalStateB1 */
void printMotorSpeeds(const BUS_Speed *sp);
void printTemperature(float t);
void printBattery(float b);

/* Elementari LocalStateB2 */
void printSonar(const BUS_Sonar *sn);
void printGyroscope(float g);
void printRemoteController(const BUS_RemoteController *rc);

/* Elementari Decision */
void printEnumLine(const char *label, int v);
void printSetPoint(const BUS_SetPoint *sp);
void printLeds(const BUS_Leds *leds);

/* -------------------------------------------------- */
/* Strutture complesse                                 */
/* -------------------------------------------------- */
void printLocalStateB1(const BUS_LocalStateB1 *s);
void printLocalStateB2(const BUS_LocalStateB2 *s);
void printGlobalState(const BUS_GlobalState *g);
void printDecision(const BUS_Decision *d);

#endif /* INC_PRINT_H_ */
