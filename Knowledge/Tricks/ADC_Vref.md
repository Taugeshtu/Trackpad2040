> Driving high the SMPS mode pin (GPIO23), to force the power supply into PWM mode, can greatly reduce the inherent ripple of the SMPS at light load, and therefore the ripple on the ADC supply

![[ADC_powering_Pico.png]]

> It is safe to supply ADC_AVDD at a higher or lower voltage than IOVDD, e.g. to power the ADC at 3.3V, for optimum performance, while supporting 1.8V signal levels on the digital IO. But the voltage on the ADC analogue inputs must not exceed IOVDD, e.g. if IOVDD is powered at 1.8V, the voltage on the ADC inputs should be limited to 1.8V. Voltages greater than IOVDD will result in leakage currents through the ESD protection diodes
This pertains to custom board designs on RP2040; on Pico IOVDD is basically 3V3 with a decoupling cap - nothing to worry about!

from [forum post about ADC noise](https://forums.raspberrypi.com/viewtopic.php?p=2032382&sid=99dfe1150aff7a1e012df6b73decc487#p2032382)
> You really need to add a filter capacitor, i'm using 0.1uF for low frequency , on the ADC_IN pin to remove any digital noise. This greatly increase the SNR


[Another great forum post with findings:](https://forums.raspberrypi.com/viewtopic.php?t=330208)
- Without ADC_VREF and with regular mode SMPS, sampling in 12 bit mode, there is a noise of amplitude of about **60** units.
- Without ADC_VREF and with the SMPS in PWM mode, sampling in 12 bit mode, there is a noise of amplitude of about **20** units.
- With ADC_VREF provided by a CR2032 battery and with the SMPS in PWM mode, sampling in 12 bit mode, there is a noise of amplitude of about **10** units.
- With ADC_VREF provided by a two C batteries in series, utilized also to power the Pico bypassing the SMPS, sampling in 12 bit mode, there is a noise of amplitude of about **3** units -- with some 10 units periodic, very short pulse at about 160Hz which I have not investigated the nature of yet.