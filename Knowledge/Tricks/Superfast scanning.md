### Principle
[[Latency data]] shows that doing any kind of IO function switching (even toggling between high and low for MUXes! ) is sufficiently slow to be a concern over naive implementation:
- just pin setting is 28 cycles PER PIN without the calling infrastructure (loops and such); for 4-bit MUX that's ~120 cycles AT LEAST, or >1us
- for un-muxed TX, we're looking at switching functions of 2 pins, which is fairly decent, at ~42 cycles each

### Implementations
- [ ] Naive - just use gpio API
- [ ] choice code - dig deeper into API, get the hw_ functions, run the registers
- [ ] DMA-driven: claim and configure DMA channels, essentially preparing the necessary controls, then push them all at once because DMA channels are parallel, woo~
- [ ] DMA+PIO: the most hardcore one; DMA puts data for PIO to drive the switching of the pins. NO IDEA if that allows toggling PWM (could be generated by PIO directly tho! but a whole new can of worms), but definitely an option for toggling MUX selectors. Maybe it's even possible without DMA?..