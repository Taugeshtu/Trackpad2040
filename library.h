#ifndef LIBRARY_H
#define LIBRARY_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
	MT40_TX_STATUS_SUCCESS_25 = 0,	// setup was successful, with PWM set at 25% duty cycle
	MT40_TX_STATUS_SUCCESS_50 = 1,	// setup was successful, but PWM had to be set at 50% duty cycle due to low CPU frequency
	MT40_TX_STATUS_CLOCK_ERROR = 10
} MT40_TX_STATUS;

typedef enum {
	MT40_RX_STATUS_SUCCESS = 0,
	MT40_RX_STATUS_ADC_PIN_ERROR = 20
} MT40_RX_STATUS;

typedef struct {
	const bool isMultiplexed;
	const uint8_t* pins;		// !!WARNING!! In case of non-multiplexed RX, these are adc CHANNELS, not pins!
	const uint8_t numPins;       // Number of pins
	const uint8_t activePin;     // Active pin in case of a multiplexed setup
} mt40Pins;

typedef struct {
	mt40Pins pins;
	MT40_TX_STATUS status; // Status of the setup
} mt40TXresult;

typedef struct {
	mt40Pins pins;
	MT40_RX_STATUS status; // Status of the setup
} mt40RXresult;

mt40TXresult mt40SetupTX( const uint8_t TXpins[], const uint8_t numTXpins );
mt40TXresult mt40SetupTXMUXed( const uint8_t MUXpins[], const uint8_t numMUXpins, const uint8_t TXpin );

mt40RXresult mt40SetupRX( const uint8_t RXpins[], const uint8_t numRXpins );
mt40RXresult mt40SetupRXMUXed( const uint8_t MUXpins[], const uint8_t numMUXpins, const uint8_t RXpin );

typedef enum {
	MT40_SCAN_STATUS_OK = 0,
	MT40_SCAN_STATUS_PIN_COUNT_ERROR = 10
} MT40_SCAN_STATUS;

MT40_SCAN_STATUS mt40ScanMatrix( const mt40Pins TXpins, const mt40Pins RXpins, uint8_t TXcount, uint8_t RXcount, uint16_t (*result)[RXcount] );

#endif // LIBRARY_H