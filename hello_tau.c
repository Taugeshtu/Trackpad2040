#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "library.h"

int main() {
	// Set CPU frequency to 100MHz
	set_sys_clock_khz( 100000, true );
	
	uint8_t TXpins[] = { 0, 1, 2, 3, 4, 5, 6 };
	mt40SetupTX( TXpins, sizeof( TXpins ) /sizeof( TXpins[0] ) );
	
	uint8_t MUXpins[] = { 18, 19, 20, 21 };
	uint8_t RXpin = 28;	// MUST be one of the ADC pins!
	mt40SetupRXMUXed( MUXpins, sizeof( MUXpins ) /sizeof( MUXpins[0] ), RXpin );
	
	stdio_init_all();
	// Keep the program running
	while( 1 ) {
		const float conversion_factor = 3.3f /(1 << 12);
		uint16_t result = adc_read();
		printf( "v: %f V\n", result * conversion_factor );
		sleep_ms( 100 );
	}
	
	return 0;
}
