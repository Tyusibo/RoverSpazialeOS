/**
 * @file motor_constants.h
 * @brief Definizione delle costanti, macro e modalità operative del sistema di controllo.
 *
 * Questo file contiene enumerazioni, costanti e macro utilizzate per la gestione
 * del regolatore, della velocità, del duty cycle e dei limiti di saturazione.
 */
#ifndef INC_CONSTANTS_MOTOR_CONSTANTS_H_
#define INC_CONSTANTS_MOTOR_CONSTANTS_H_

/* -------------------------------------------------------------------------- */
/*                               TUNING OPTIONS                               */
/* -------------------------------------------------------------------------- */


/** Frequenza di campionamento (Hz) */
static const float SAMPLING_RATE = 50.0f;


/* -------------------------------------------------------------------------- */
/*                               NON-TUNING AREA                              */
/* -------------------------------------------------------------------------- */

/** Numero totale di tick encoder per giro motore, considerando riduttore e segnali */
static const float COUNTS = (51.0f * 12.0f * 2.0f * 2.0f);  // 2448

/** Periodo di campionamento (s) */
static const float TS = 1.0f / SAMPLING_RATE;


/** Velocità massima ammessa (rpm) */
static const float MAX_VELOCITY = 200.0f;

/** Tick massimi attesi entro un periodo di campionamento */
static const float MAX_TICKS = (1.0f / SAMPLING_RATE) *
                               ((MAX_VELOCITY / 60.0f) * COUNTS);


/* -------------------------------------------------------------------------- */
/*                             DUTY CYCLE LIMITS                              */
/* -------------------------------------------------------------------------- */

/** Valore minimo di input normalizzato */
static const float IN_MIN = -100.0f;
/** Valore massimo di input normalizzato */
static const float IN_MAX = 100.0f;

/** Duty cycle minimo convertito in tensione equivalente */
static const float OUT_MIN = 56.8f;
/** Duty cycle massimo convertito in tensione equivalente */
static const float OUT_MAX = 94.6f;


/* -------------------------------------------------------------------------- */
/*                                 SATURATION                                 */
/* -------------------------------------------------------------------------- */

/** Tensione massima applicabile */
static const float MAX_VOLT = 12.0f;

/** Tensione minima applicabile */
static const float MIN_VOLT = -12.0f;

/* -------------------------------------------------------------------------- */
/*                            OPEN LOOP DC GAINS                              */
/* -------------------------------------------------------------------------- */

/** Guadagno statico (RPM/V) - Motore 1 (Front Left) */
static const float DC_GAIN_MOT1 = 14.480f;

/** Guadagno statico (RPM/V) - Motore 2 (Front Right) */
static const float DC_GAIN_MOT2 = 14.200f;

/** Guadagno statico (RPM/V) - Motore 3 (Rear Right) */
static const float DC_GAIN_MOT3 = 14.510f;

/** Guadagno statico (RPM/V) - Motore 4 (Rear Left) */
static const float DC_GAIN_MOT4 = 14.210f;


#endif /* INC_CONSTANTS_MOTOR_CONSTANTS_H_ */
