#include "print.h"

/* -------------------------------------------------- */
/* UART corrente                                      */
/* -------------------------------------------------- */
static UART_HandleTypeDef *current_printer;

void setPrinter(UART_HandleTypeDef *huart)
{
    if (huart != NULL) {
        current_printer = huart;
    }
}

UART_HandleTypeDef* getPrinter(void)
{
    return current_printer;
}

/* Utility interna: trasmette una stringa già pronta */
static void prv_tx_str(const char *s)
{
    if (s == NULL) return;
    if (current_printer == NULL) return;

    HAL_UART_Transmit(current_printer, (uint8_t*)s, (uint16_t)strlen(s), HAL_MAX_DELAY);
}

/* Utility interna: trasmette buffer raw */
static void prv_tx_buf(const uint8_t *buf, uint32_t len)
{
    if (buf == NULL || len == 0) return;
    if (current_printer == NULL) return;

    HAL_UART_Transmit(current_printer, (uint8_t*)buf, (uint16_t)len, HAL_MAX_DELAY);
}

/* -------------------------------------------------- */
/* Base / low-level                                   */
/* -------------------------------------------------- */
void uartPrint(const char *msg)
{
    prv_tx_str(msg);
}

void printMsg_impl(const char *pData)
{
    prv_tx_str(pData);
}

void clearScreen(void)
{
    printMsg(CLRSCR);
}

/* -------------------------------------------------- */
/* Primitivi (stampe testabili)                        */
/* -------------------------------------------------- */
void printInt(int32_t v)
{
    char msg[24];
    snprintf(msg, sizeof(msg), "%ld", (long)v);
    prv_tx_str(msg);
}

void printFloat(float v, uint8_t decimals)
{
    /* decimals clamp (evita formati assurdi) */
    if (decimals > 6) decimals = 6;

    char fmt[8];
    char msg[32];

    /* fmt tipo: "%.2f" */
    snprintf(fmt, sizeof(fmt), "%%.%uf", (unsigned)decimals);
    snprintf(msg, sizeof(msg), fmt, (double)v);

    prv_tx_str(msg);
}

/* -------------------------------------------------- */
/* Debug frame sizes                                  */
/* -------------------------------------------------- */
void UART_PrintFrameSizes(void)
{
    char buf[64];
    int len;

#define PRINT_SIZE(name, value)                                  \
    do {                                                         \
        len = snprintf(buf, sizeof(buf),                         \
                       "%-30s : %lu\r\n",                        \
                       name, (unsigned long)(value));            \
        if (len > 0) {                                           \
            prv_tx_buf((const uint8_t*)buf, (uint32_t)len);       \
        }                                                        \
    } while (0)

    /* --- Sensori --- */
    PRINT_SIZE("SPEED_FRAME_SIZE", SPEED_FRAME_SIZE);
    PRINT_SIZE("TEMPERATURE_FRAME_SIZE", TEMPERATURE_FRAME_SIZE);
    PRINT_SIZE("BATTERY_LEVEL_FRAME_SIZE", BATTERY_LEVEL_FRAME_SIZE);
    PRINT_SIZE("SONAR_FRAME_SIZE", SONAR_FRAME_SIZE);
    PRINT_SIZE("GYROSCOPE_FRAME_SIZE", GYROSCOPE_FRAME_SIZE);
    PRINT_SIZE("REMOTE_CONTROLLER_FRAME_SIZE", REMOTE_CONTROLLER_FRAME_SIZE);

    /* --- Stati --- */
    PRINT_SIZE("LOCAL_STATE_B1_FRAME_SIZE", LOCAL_STATE_B1_FRAME_SIZE);
    PRINT_SIZE("LOCAL_STATE_B2_FRAME_SIZE", LOCAL_STATE_B2_FRAME_SIZE);
    PRINT_SIZE("GLOBAL_STATE_FRAME_SIZE", GLOBAL_STATE_FRAME_SIZE);

    /* --- Decisione / Controllo --- */
    PRINT_SIZE("SET_POINT_FRAME_SIZE", SET_POINT_FRAME_SIZE);
    PRINT_SIZE("DECISION_FRAME_SIZE", DECISION_FRAME_SIZE);

#undef PRINT_SIZE
}

/* -------------------------------------------------- */
/* Debug print full buffer in HEX                     */
/* -------------------------------------------------- */
void uartPrintBuffHex(const char *tag, const uint8_t *buf, uint32_t len)
{
    char msg[80];

    snprintf(msg, sizeof(msg), "%s (%lu): ", tag, (unsigned long)len);
    prv_tx_str(msg);

    for (uint32_t i = 0; i < len; i++) {
        snprintf(msg, sizeof(msg), "%02X ", buf[i]);
        prv_tx_str(msg);
    }
    prv_tx_str("\r\n");
}

/* -------------------------------------------------- */
/* Strutture complesse (costruite con elementari)      */
/* -------------------------------------------------- */
void printHeader(const char *title)
{
    printMsg(title);
    printMsg("\r\n");
}

void printLabel(const char *label)
{
    printMsg("  ");
    printMsg(label);
    printMsg(": ");
}

void printNewLine(void)
{
    printMsg("\r\n");
}

/* ---- LocalStateB1 ---- */
void printMotorSpeeds(const BUS_Speed *sp)
{
    char msg[128];
    snprintf(msg, sizeof(msg),
             "  Speed: %.2f %.2f %.2f %.2f\r\n",
             sp->motor1, sp->motor2, sp->motor3, sp->motor4);
    uartPrint(msg);
}

void printTemperature(float t)
{
    char msg[64];
    snprintf(msg, sizeof(msg), "  Temperature: %.2f C\r\n", (double)t);
    uartPrint(msg);
}

void printBattery(float b)
{
    char msg[64];
    snprintf(msg, sizeof(msg), "  Battery: %.2f %%\r\n", (double)b);
    uartPrint(msg);
}

void printLocalStateB1(const BUS_LocalStateB1 *s)
{
    if (s == NULL) return;

    printHeader("LocalStateB1");
    printMotorSpeeds(&s->speed);
    printTemperature(s->temperature);
    printBattery(s->batteryLevel);
}

/* ---- LocalStateB2 ---- */
void printSonar(const BUS_Sonar *sn)
{
    char msg[96];
    snprintf(msg, sizeof(msg), "  Sonar: L=%d F=%d R=%d\r\n",
             sn->left, sn->front, sn->right);
    uartPrint(msg);
}

void printGyroscope(float g)
{
    char msg[64];
    snprintf(msg, sizeof(msg), "  Gyroscope: %.2f\r\n", (double)g);
    uartPrint(msg);
}

void printRemoteController(const BUS_RemoteController *rc)
{
    char msg[128];
    snprintf(msg, sizeof(msg),
             "  RemoteController: x_lever=%d y_lever=%d buttons=%d\r\n",
             rc->x_lever, rc->y_lever, rc->buttons);
    uartPrint(msg);
}

void printLocalStateB2(const BUS_LocalStateB2 *s)
{
    if (s == NULL) return;

    printHeader("LocalStateB2");
    printSonar(&s->sonar);
    printGyroscope(s->gyroscope);
    printRemoteController(&s->remoteController);
}

/* ---- GlobalState ---- */
void printGlobalState(const BUS_GlobalState *g)
{
    if (g == NULL) return;

    printHeader("GlobalState");
    printLocalStateB1(&g->localStateB1);
    printLocalStateB2(&g->localStateB2);
}

/* ---- Decision ---- */
void printEnumLine(const char *label, int v)
{
    printLabel(label);
    printInt(v);
    printNewLine();
}

void printSetPoint(const BUS_SetPoint *sp)
{
    char msg[96];
    snprintf(msg, sizeof(msg), "  setPoint(Left)=%.3f\r\n", (double)sp->leftAxis);
    uartPrint(msg);
    snprintf(msg, sizeof(msg), "  setPoint(Right)=%.3f\r\n", (double)sp->rightAxis);
    uartPrint(msg);
}

void printDecision(const BUS_Decision *d)
{
    if (d == NULL) return;

    printHeader("Decision");

    printEnumLine("actuator",   (int)d->actuator);
    printEnumLine("userAction", (int)d->userAction);
    printEnumLine("roverAction",(int)d->roverAction);
    printEnumLine("safeAction", (int)d->safeAction);

    printSetPoint(&d->setPoint);
}
