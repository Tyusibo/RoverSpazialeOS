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

/**
 * @brief Modalità di stampa dei dati di debug.
 */
typedef enum {
    VERBOSE_NONE = 0,   /**< Nessuna stampa */
    VERBOSE_U = 1,      /**< Stampa dell’uscita del regolatore */
    VERBOSE_DUTY = 2,   /**< Stampa del duty cycle */
    VERBOSE_VELOCITY = 3, /**< Stampa della velocità */
    VERBOSE_ERROR = 4   /**< Stampa dell’errore */
} VerboseMode_t;

/** Modalità di verbosità attiva */
static const VerboseMode_t VERBOSE = VERBOSE_VELOCITY;

/**
 * @brief Modalità di calcolo della velocità.
 *
 * - AVERAGE → velocità calcolata come media dei campioni
 * - INSTANTANEOUS → velocità istantanea tra due campioni
 */
typedef enum {
    AVERAGE = 0,        /**< Media sui campioni */
    INSTANTANEOUS = 1   /**< Calcolo istantaneo */
} ComputeVelocity_t;

/** Modalità di calcolo della velocità attiva */
static const ComputeVelocity_t SPEED = AVERAGE;

/** Frequenza di campionamento (Hz) */
static const float SAMPLING_RATE = 200.0f;

/** Frequenza del timer TIM7 (Hz) */
static const float FREQUENCY_TIM7 = 1000000.0f;

/* -------------------------------------------------------------------------- */
/*                               NON-TUNING AREA                              */
/* -------------------------------------------------------------------------- */

/** Numero totale di tick encoder per giro motore, considerando riduttore e segnali */
static const float COUNTS = (51.0f * 12.0f * 2.0f * 2.0f);  // 2448

/** Periodo di campionamento (s) */
static const float TS = 1.0f / SAMPLING_RATE;

/** Valore massimo del contatore a 32 bit dell’encoder */
static const uint64_t MAX_CNT_VALUE = ((1ULL << 32) - 1);

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


#endif /* INC_CONSTANTS_MOTOR_CONSTANTS_H_ */
