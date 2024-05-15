Simulation of the choice RX line design:
https://everycircuit.com/circuit/5469424441884672

<a href="https://everycircuit.com/circuit/5469424441884672">MT_RXline_OpAmpBuffer - EveryCircuit</a><br>
<iframe width="560" height="360" src="https://everycircuit.com/embed/5469424441884672" frameborder="0"></iframe>

The idea behind it is: ADC may be having a hard time reading off a signal that's a reflection of 10MHz PWM square. Let's make its job simpler by adding a low-pass filter! (basically a small-ish capacitor that will buffer/integrate the signal). But oh no, when we do that we have 2 problems:
1. Adding a capacitor to a capacitance-operating RX line is a bad idea since it messes up the signal
2. You need certain combination of filter resistor and capacitor to make them integrate with certain frequency cut-off; when you add a drain resistor to "dry up" the capacitor these 3 requirements stand in contention, wanting opposite things:
	- capacitor wants to be big to store up charge, but that drives the signal voltage down, making ADC's job hard
	- filter resistor wants to be small to allow fast capacitor charge, but big to not let it drain "backwards"
	- drain resistor wants to be big to not create yet another voltage drop via divider, but small to discharge the buffer quickly enough

To solve `1.` we introduce Op-Amp buffer/voltage follower - why mess up the signal when you can not do that?
To solve `2.` we connect the output of our ADC "front-end" to not only ADC input pin, but also to another GPIO pin, so that we can forcibly drive that pin DOWN to the ground and quickly drain the buffer when we jump between matrix cells

40pF capacitance up top simulates human finger (somewhere between 10-50pF, depending on touch severity)
40pF capacitance down low is RX line's own capacitance
20k resistor is to dry up the lines when they are not bound via [[MUX]]

Idea: maybe I can just switch function of the ADC pin to GPIO?.. And leave ADC settings in tact?