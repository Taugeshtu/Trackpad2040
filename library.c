#include <stdlib.h>
#include "library.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "hardware/clocks.h"

const uint8_t _ADC_PIN_ERROR_CODE = 255;
const uint32_t _TX_FREQ = 10 *1000000;	// in Hz

uint8_t _toPower( const uint8_t n, const uint8_t power ) {
	uint8_t result = 1;
	for( uint8_t powerIndex = 0; powerIndex < power; powerIndex++ ) {
		result *= n;
	}
	return result;
}

uint8_t _GPIO2ADCChannel( uint8_t gpio ) {
	switch( gpio ) {
		case 26: return 0;
		case 27: return 1;
		case 28: return 2;
		case 29: return 3;
		default: return _ADC_PIN_ERROR_CODE;	// Invalid GPIO for ADC
	}
}
MT40_TX_STATUS _calculateTXClock( uint8_t *PWMcounts, float *divider ) {
	if( PWMcounts == NULL || divider == NULL ) {
		return MT40_TX_STATUS_CLOCK_ERROR;
	}
	
	uint32_t sysClock = clock_get_hz( clk_sys );
	if( sysClock >= _TX_FREQ *4 ) {			// ready to run 25% duty cycle?
		*PWMcounts = 4;
		*divider = sysClock /(float)(*PWMcounts *_TX_FREQ);
		return MT40_TX_STATUS_SUCCESS_25;
	}
	else if( sysClock >= _TX_FREQ *2 ) {	// can at least handle 50% duty cycle?
		*PWMcounts = 2;
		*divider = sysClock /(float)(*PWMcounts *_TX_FREQ);
		return MT40_TX_STATUS_SUCCESS_50;
	}
	return MT40_TX_STATUS_CLOCK_ERROR;
}
uint8_t _getMaxPinsCount( const mt40Pins pins ) {
	return pins.isMultiplexed ? _toPower( 2, pins.numPins ) : pins.numPins;
}

void _setupMUX( const uint8_t MUXpins[], const uint8_t numMUXpins ) {
	for( uint8_t i = 0; i < numMUXpins; i++ ) {
		gpio_init( MUXpins[i] );
		gpio_set_dir( MUXpins[i], GPIO_OUT );
	}
}
void _setupPWM( const uint8_t TXpin, const float divider, const uint8_t PWMcounts ) {
	gpio_init( TXpin );
	gpio_set_function( TXpin, GPIO_FUNC_PWM );
	uint sliceNum = pwm_gpio_to_slice_num( TXpin );
	pwm_set_clkdiv( sliceNum, divider );
	pwm_set_wrap( sliceNum, PWMcounts - 1 );
	pwm_set_chan_level( sliceNum, PWM_CHAN_A, 1 );
	pwm_set_enabled( sliceNum, false );
}



mt40TXresult mt40SetupTX( const uint8_t TXpins[], const uint8_t numTXpins ) {
	mt40Pins pinsConfig = {
		.isMultiplexed = false,
		.pins = TXpins,
		.numPins = numTXpins,
		.activePin = 0xFF
	};
	
	uint8_t PWMcounts;
	float divider;
	
	MT40_TX_STATUS status = _calculateTXClock( &PWMcounts, &divider );
	if( status != MT40_TX_STATUS_CLOCK_ERROR ) {
		// Setting up PWM for TX pins:
		for( uint8_t i = 0; i < numTXpins; i++ ) {
			uint8_t TXpin = TXpins[i];
			_setupPWM( TXpin, divider, PWMcounts );
		}
	}
	
	return (mt40TXresult) { .pins = pinsConfig, .status = status };
}
mt40TXresult mt40SetupTXMUXed( const uint8_t MUXpins[], const uint8_t numMUXpins, const uint8_t TXpin ) {
	mt40Pins pinsConfig = {
		.isMultiplexed = true,
		.pins = MUXpins,
		.numPins = numMUXpins,
		.activePin = TXpin
	};
	
	uint8_t PWMcounts;
	float divider;
	
	MT40_TX_STATUS status = _calculateTXClock( &PWMcounts, &divider );
	if( status != MT40_TX_STATUS_CLOCK_ERROR ) {
		_setupMUX( MUXpins, numMUXpins );
		_setupPWM( TXpin, divider, PWMcounts );
	}
	
	return (mt40TXresult) { .pins = pinsConfig, .status = status };
}



mt40RXresult mt40SetupRX( const uint8_t RXpins[], const uint8_t numRXpins ) {
	uint8_t* adcChannels = malloc( numRXpins *sizeof( uint8_t ) );
	for( uint8_t i = 0; i < numRXpins; i++ ) {
		uint8_t adcChannel = _GPIO2ADCChannel( RXpins[i] );
		if( adcChannel == _ADC_PIN_ERROR_CODE )
			return (mt40RXresult) { .status = MT40_RX_STATUS_ADC_PIN_ERROR };
		
		adcChannels[i] = adcChannel;
	}
	
	mt40Pins pinsConfig = {
		.isMultiplexed = false,
		.pins = adcChannels,
		.numPins = numRXpins,
		.activePin = 0xFF
	};
	mt40RXresult result = {
		.status = MT40_RX_STATUS_SUCCESS,
		.pins = pinsConfig
	};
	
	adc_init();
	for( uint8_t i = 0; i < numRXpins; i++ ) {
		adc_gpio_init( RXpins[i] );
	}
	
	return result;
}
mt40RXresult mt40SetupRXMUXed( const uint8_t MUXpins[], const uint8_t numMUXpins, const uint8_t RXpin ) {
	uint8_t adcChannel = _GPIO2ADCChannel( RXpin );
	if( adcChannel == _ADC_PIN_ERROR_CODE )
		return (mt40RXresult) { .status = MT40_RX_STATUS_ADC_PIN_ERROR };
	
	mt40Pins pinsConfig = {
		.isMultiplexed = true,
		.pins = MUXpins,
		.numPins = numMUXpins,
		.activePin = adcChannel
	};
	mt40RXresult result = {
		.status = MT40_RX_STATUS_SUCCESS,
		.pins = pinsConfig
	};
	
	_setupMUX( MUXpins, numMUXpins );
	
	adc_init();
	adc_gpio_init( RXpin );
	
	return result;
}

void _dropToGND( uint8_t pin ) {
	gpio_set_function( pin, GPIO_FUNC_SIO );
	gpio_set_dir( pin, GPIO_OUT );	// TODO: research whether it's better than setting the pin to internal pull-down?..
	gpio_put( pin, 0 );
}

