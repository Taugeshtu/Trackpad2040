### TX pins
Given that Pi Pico's PWM channels layout is:

| GPIO    | PWM chan | Board  | PWM chan | GPIO        |
|---------|----------|--------|----------|-------------|
| GP0     | 0A       | USB    |          | VBUS        |
| GP1     | 0B       |        |          | VSYS        |
| **GND** |          |        |          | **GND**     |
| GP2     | 1A       |        |          | 3V3 En      |
| GP3     | 1B       |        |          | 3V3 Out     |
| GP4     | 2A       |        |          | ADC Vref    |
| GP5     | 2B       |        | 6A       | A2 / GP28   |
| **GND** |          | RP2040 |          | **ADC GND** |
| GP6     | 3A       |        | 5B       | A1 / GP27   |
| GP7     | 3B       |        | 5A       | A0 / GP26   |
| GP8     | 4A       |        |          | RUN         |
| GP9     | 4B       |        | 3A       | GP22        |
| **GND** |          |        |          | **GND**     |
| GP10    | 5A       |        | 2B       | GP21        |
| GP11    | 5B       |        | 2A       | GP20        |
| GP12    | 6A       |        | 1B       | GP19        |
| GP13    | 6B       | H1     | 1A       | GP18        |
| **GND** |          |        |          | **GND**     |
| GP14    | 7A       | H2     | 0B       | GP17        |
| GP15    | 7B       | H3     | 0A       | GP16        |

it is **highly** advisable that you work around pins that have the same slice on them - since the library will be configuring its own clocks for them, so your other pins don't get stuck with clocks necessary for the library.

For example, if you need 4 TX lines, the best you can do would be to use pins 0, 1, 16 & 17 - since they are all on slice 0. That way you get the rest of the slices to set up however you want/need.

A bad setup would be something like using TX pins 0, 3, 4, 28 - not only are you clocking 4 slices out of 8 to library-required frequency, you're also robbing yourself of A2 pin you could've used for other purpose.

### MUX pins


### RX pin
Whichever analog pin happens to be convenient for you. Be advised that the library makes HEAVY use of ADC, so your opportunities for utilizing ADC for other purposes will be limited by its traffic - there's only so many samples a 500ksps ADC can take in a second.
Napkin math:
for reading from a 16x16 matrix (which is quite generous, with recommended pad size being 6x6mm and 0.5mm spacing that covers the sensing area of about 147x147mm) you need 256 reads. If you want to run a touchpad at 1000Hz, that's half your capacity gone.