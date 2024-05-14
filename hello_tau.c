#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "library.h"

void _printDiff( uint8_t TXcount, uint8_t RXcount, uint16_t (*result)[RXcount], uint16_t (*initial)[RXcount], float conversionFactor ) {
	printf( "-----\n" );
	for( uint8_t i = 0; i < TXcount; i++ ) {
		for( uint8_t j = 0; j < RXcount; j++ ) {
			float convertedValue = result[i][j] *conversionFactor;
			float convertedInitial = initial[i][j] *conversionFactor;
			float x = convertedValue - convertedInitial;
			printf( "[%0.2f] ", x );
		}
		printf( "\n" );
	}
}

void _printRaw( uint8_t TXcount, uint8_t RXcount, uint16_t (*result)[RXcount] ) {
	printf( "-----\n" );
	for( uint8_t i = 0; i < TXcount; i++ ) {
		for( uint8_t j = 0; j < RXcount; j++ ) {
			printf( "[%i] ", result[i][j] );
		}
		printf( "\n" );
	}
}

int main() {
	// Set CPU frequency to 100MHz
	set_sys_clock_khz( 100000, true );
	
	uint8_t TXpins[] = { 0, 1, 2, 3, 4, 5, 6 };
	// uint8_t TXpins[] = { 0, 1 };
	mt40TXresult TXsetup = mt40SetupTX( TXpins, sizeof( TXpins ) /sizeof( TXpins[0] ) );
	
	uint8_t MUXpins[] = { 21, 20, 19, 18 };
	uint8_t RXpin = 28;	// MUST be one of the ADC pins!
	mt40RXresult RXsetup = mt40SetupRXMUXed( MUXpins, sizeof( MUXpins ) /sizeof( MUXpins[0] ), RXpin );
	
	stdio_init_all();
	
	if( TXsetup.status != MT40_TX_STATUS_SUCCESS_25 ) {
		if( TXsetup.status == MT40_TX_STATUS_SUCCESS_50 ) {
			printf( "Setup half-successful - had to drop to 50 percent duty cycle" );
		}
		else {
			printf( "No dice, clock too slow!" );
			return 1;
		}
	}
	
	if( RXsetup.status != MT40_RX_STATUS_SUCCESS ) {
		if( RXsetup.status == MT40_RX_STATUS_ADC_PIN_ERROR ) {
			printf( "One of requested RX pins is not actually an ADC pin!" );
			return 1;
		}
	}
	
	uint8_t TXlinesCount = 7;
	uint8_t RXlinesCount = 9;
	uint16_t initial[TXlinesCount][RXlinesCount];
	const uint8_t initialScans = 1;
	for( uint8_t i = 0; i < initialScans; i++ ) {
		MT40_SCAN_STATUS initialScanStatus = mt40ScanMatrix( TXsetup.pins, RXsetup.pins, TXlinesCount, RXlinesCount, initial );
		if( initialScanStatus != MT40_SCAN_STATUS_OK ) {
			printf( "Scan failed :/" );
		}
		
		sleep_ms( 5 );
	}
	uint16_t result[TXlinesCount][RXlinesCount];
	while( 1 ) {
		MT40_SCAN_STATUS scanStatus = mt40ScanMatrix( TXsetup.pins, RXsetup.pins, TXlinesCount, RXlinesCount, result );
		if( scanStatus != MT40_SCAN_STATUS_OK ) {
			printf( "Scan failed :/" );
		}
		
		_printRaw( TXlinesCount, RXlinesCount, result );
		
		const float conversionFactor = 3.3f /(1 << 12);
		// _printDiff( TXlinesCount, RXlinesCount, result, initial, conversionFactor );
		
		sleep_ms( 1000 );
	}
	
	return 0;
}
