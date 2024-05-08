#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"

int main() {
	// Set CPU frequency to 100MHz
	set_sys_clock_khz( 100000, true );
	
	// PWM setup on pin 26
	gpio_init( 26 );
	gpio_set_function( 26, GPIO_FUNC_PWM );
	uint slice_num = pwm_gpio_to_slice_num( 26 );
	
		// Set up PWM clock & wrapping
		// To achieve 10MHz frequency we divide main clock by 10; but then multiply by 4 so our 4 counts complete a cycle.
		// "wrap" is inclusive; "chan_level" is at which count we flip the value
	pwm_set_clkdiv( slice_num, 10.0/4.0 );
	pwm_set_wrap( slice_num, 3 );
	pwm_set_chan_level( slice_num, PWM_CHAN_A, 1 );
	pwm_set_enabled( slice_num, true );
	
	// ADC setup
	adc_init();
	adc_gpio_init( 28 );
	adc_select_input( 2 );
	
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
