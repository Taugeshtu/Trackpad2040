This looks like a great start:
https://hci.cs.uni-saarland.de/projects/multi-touch-kit/
The paper: [[MTK_Multitouch_DIY.pdf]]
The tutorial: [[MTK_Tutorial.pdf]]

Arduino-based, can take trash electrodes, why the fuck not...

Working principle:
- Have a bunch of TX and RX lines
- Pump TX lines one by one by 10MHz, 25% duty PWM (square is fiiiiine)
- While TX line burns, listen to RX lines (can be one by one, can be all at once) - feed them into ADC and watch for voltage

So let's reproduce it on something a bit beefier, hoping we can achieve higher poll rate and thus less latency! Besides, if we have a beefy MC to run this off, we can do all kinds of cool shit with it!