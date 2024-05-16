### Data
adc_init(): 77
adc_gpio_init(): 140
adc_gpio_init_fast(): 126
pin_drop(): 91

gpio_init(): 49
gpio_set_dir(): 21
gpio_put(): 28
gpio_clr_mask(): 21

### Snippets
adc_gpio_init_fast():
```
gpio_set_function(pin, GPIO_FUNC_NULL);
gpio_disable_pulls(pin);
gpio_set_input_enabled(pin, false);
```

pin_drop():
```
gpio_init( pin );
gpio_set_dir( pin, GPIO_OUT );
gpio_put( pin, 0 );
```
