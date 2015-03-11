# Introduction

This project is about how to control an Arduino Motor Shield using AVR C. The project will hopefully give us insight into the following:

1. Atmel AVR Chips
	- Brief understanding of Datasheets
2. AVR C Language
	- Brief understanding (only touching the surface)
3. Arduino Modules
	- Port Manipulation (on development board)
	- Timer Manipulation (on development board)
	- Pin Mappings (from development board to motor shield)
4. IC Modules
	- Programming an 8-bit Shift Register
	- Programming an H-Bridge

Top View | Side View | Bottom View
------------ | ------------- | -------------
<img src="/docs/imgs/kit-top.png" alt="Top View"> | <img src="/docs/imgs/kit-side.png" alt="Side View"> | <img src="/docs/imgs/kit-bottom.png" alt="Bottom View">

Since I believe this little project covers a lot of topics, the information outlined below does not go into expert detail on each topic and therefore a lot of assumptions are made about your understanding. However, I do try to provide some links to other sites which may have better explanations.

Some later extensions to this project might be to allow the end user to control the Motor Shield via some wireless communication. But for now, the scope is to just get the Development Board and the Motor Shield work together.

<p align="center">
<a title="Demo Video" href="http://www.youtube.com/watch?feature=player_embedded&v=Fk4Gt5CNe6o">
<img src="http://img.youtube.com/vi/Fk4Gt5CNe6o/0.jpg" alt="Demo Video">
</a>
</p>

# Components &amp; Tools

### IDE's

There are two main development [IDE's](http://en.wikipedia.org/wiki/Integrated_development_environment) (IMHO) namely the [Arduino IDE](http://arduino.cc/en/Main/Software) and the [Atmel Studio](http://www.atmel.com/microsite/atmel_studio6/). For the purpose of this project I'll be using the Arduino IDE.

### Components

Name | Description | Picture
------------ | ------------- | -------------
Arduino Development Board | Target board will be the [Arduino Leonardo](http://arduino.cc/en/Main/arduinoBoardLeonardo), but I'll extend support for [Arduino UNO](http://arduino.cc/en/Main/arduinoBoardUno) as well. | <img src="/docs/imgs/board-arduino-leonardo.png" alt="Arduino Leonardo">
Motor Shield | In my toolbox is the DK Electronics Motor Shield (version 1) and I need to get this working. | <img src="/docs/imgs/shield-dk-motorshield.png" alt="DK Electronics Motor Shield">
4 Wheel Drive Chassis | In my toolbox is the ["4WD Robot Smart Car Kits Chassis"](http://www.ebay.com/itm/4WD-Robot-Smart-Car-Chassis-Kits-car-with-Speed-Encoder-DC-3v-5V-6V-for-Arduino-/171122633517) | <img src="/docs/imgs/chassis-4WD.png" alt="4 Wheel Drive Chassis">
4 AA Battery Holder | A Battery Holder | <img src="/docs/imgs/power-battery-holder.png" alt="Battery Holder">

### Datasheets / Schematics

TODO!

# Notes & Theory

### API Design

Let start with the API which will drive the motors. Outlined below is just a high level set of methods to control the 4WD.

* Q4WD.forward()
> This will turn all 4 motors forward at the specified speed.

* Q4WD.backward()
> This will turn all 4 motors backward at the specified speed.

* Q4WD.left()
> This will make a left turn. Some motors will rotate at different speeds to others.

* Q4WD.right()
> This will make a right turn. Some motors will rotate at different speeds to others.

* Q4WD.stop()
> This will stop all motors. This should not be an immediate stop, not more like a slow down to halt affect.

### Datasheets

In my opinion datasheets are not for humans. Somewhere there is a disconnect between the people who design these specifications and the person trying to understand them for the first time. Please note, my statements below might not always be accurate and its best that you consult your own datasheets and the Internet/Books for help.

### Motor Shield

Before we can program the development board we need to know how the Motor Shield controls the motors and how the Pin Mappings are laid out between the development board the the motor shield.

The main components that we're interested in, on the Motor Shield are the two H-Bridges and the Shift Register. An H-Bridge is the IC which will be controlling our DC motors bidirectionally and typically an H-Bridge can control up to 2 DC motors. An H-Bridge requires 2 inputs to control just 1 motor, so this means we'll need 4 inputs * 2 H-Bridges = 8 Pins on the development board. Of course I'm simplifying things here and not talking about the Voltage, Ground and other Pins.

That's a lot of Pins just to control 4 DC motors and doesn't leave us much room to control other shields or components. So I thought the usage of an 8-bit shift register was an ingenious solution. An shift register IC lets you feed in bits serially, but it then presents them as parallel outputs. (we'll see more of this later). The outputs of our shift register will control the motors direction but not its speed, we still need a PWM signal from the AVR Timers to control the speed of the motors via the H-Bridge (we'll see more of this later).

##### Shift Register (8-bit)

Open your Motor Shield Schematic and locate IC3 and you should see the below image:

IC | Motor Shield Schematic | IC Functional Diagram Schematic
------------ | ------------- | -------------
<img src="/docs/imgs/component-ic-74HC595.png" alt="Real IC"> | <img src="/docs/imgs/datasheet-motorshield-IC3.png" alt="Motor Shield Schematic IC3"> | <img src="/docs/imgs/datasheet-74HC595-shift-register.png" alt="IC Functional Diagram Schematic">

The pins labelled QA through QH are the parallel output pins and therefore inputs into the H-Bridges. Motor 1 is controlled by Pins 2 (QC) and 3 (QD), Motor 2 by Pins 1 (QB) and 4 (QE), etc.

If we compare the Motor Shield Schematic with the Datasheet of the IC, they have different names for each Pin (weird?).

IC PIN | IC Datasheet | MotorShield Schematic | Description
------------ | ------------- | ------------- | -------------
Pin 15 | Q0 | QA | Parallel data output 0
Pin 1 | Q1 | QB | Parallel data output 1
Pin 2 | Q2 | QC | Parallel data output 2
Pin 3 | Q3 | QD | Parallel data output 3
Pin 4 | Q4 | QE | Parallel data output 4
Pin 5 | Q5 | QF | Parallel data output 5
Pin 6 | Q6 | QG | Parallel data output 6
Pin 7 | Q7 | QH | Parallel data output 7
Pin 8 | G | G | Ground (0 V)
Pin 9 | Q7S | QH* | Serial data output
Pin 10 | MR | MR | Master reset (active LOW)
Pin 11 | SHCP | DIR_CLK | Shift register clock input
Pin 12 | STCP | DIR_LATCH | Storage register clock input
Pin 13 | OE | DIR_EN | Output enable input (active LOW)
Pin 14 | DS | DIR_SER | Serial data input
Pin 16 | Vcc | Vcc | Supply voltage (Vcc)

The pseudo-code for sending 1100110 out from the shift register is as follows:

```C++
byte state = 0b1100110;
void shift() {
	SH_CP_Low(); // set the shift register clock pin low
	ST_CP_Low(); // set the storage register clock pin low (close the latch gate)

	for (int i=0; i<8; i++) {
		if (isBitSet(state, i)) {
			DS_High(); // set the serial data input pin high
		} else {
			DS_Low(); // set the serial data input pin low
		}

		SH_CP_High(); // set the shift register clock pin high
		SH_CP_Low(); // set the shift register clock pin low
	}

	ST_CP_High(); // set the storage register clock pin low (open the latch gate)
}
```

To perhaps visualize how data moves through the shift register, lets analyse this example table:

Clock Pulse No | Q0 | Q1 | Q2 | Q3
------------ | ------------- | ------------- | ------------- | -------------
0 | 0 | 0 | 0 | 0
1 | 1 | 0 | 0 | 0
2 | 0 | 1 | 0 | 0
3 | 0 | 0 | 1 | 0
4 | 0 | 0 | 0 | 1
5 | 0 | 0 | 0 | 0

Okay now we know how to work with the Shift Registers and we know which motors are being controlled by which Shift Register Pins, we need to know which Pins on the development board will map to the motor shield and hence control the Shift Register.

Somehow this requires a bit of guess work:

Motor Shield to Board Pins |
------------ |
<img src="/docs/imgs/datasheet-motorshield-J1-J3.png" alt="Motor Shield to Board Pins"> |

* Arduino Uno

Schematic |
------------ |
<img src="/docs/imgs/datasheet-board-uno-pins.png" alt="Schematic Uno"> |

As you can see there is no J1 & J3 mentioned on the Uno Schematic, so we'll have to for now presume that its J1=IOL and J3=IOH. Why we can make this assumption is because we had to look at the Arduino Duemilanove Schematic below:

* Arduino Duemilanove

Schematic |
------------ |
<img src="/docs/imgs/datasheet-board-duemilanove-pins.png" alt="Schematic Duemilanove"> |

The reason we're using the Arduino Duemilanove as a reference is because its Atmega8 chip has the same pin layout as the Atmega328 on the Arduino Uno.

* Arduino Leonardo

Schematic |
------------ |
<img src="/docs/imgs/datasheet-board-leonardo-pins.png" alt="Schematic Leonardo"> |

Follow again the thought process of the Arduino Duemilanove, then J1=J2 and J3=JP1.

Now we know the pin mappings from each development board to the motor shield, so lets see it clearly:

Development Board | Port & Bit | Pin # (on board) | Pin # (on Shift Register)
------------ | ------------ | ------------ | ------------
Uno | D4 | 4 | 11
 | D7 | 7 | 13
 | B0 | 8 | 14
 | B4 | 12 | 12
Duemilanove | D4 | 4 | 11
 | D7 | 7 | 13
 | B0 | 8 | 14
 | B4 | 12 | 12
Leonardo | D4 | 4 | 11
 | E6 | 7 | 13
 | B4 | 8 | 14
 | D6 | 12 | 12

##### H-Bridges

What is the development board pin mapping for the [PWM](http://en.wikipedia.org/wiki/Pulse-width_modulation) signal into the H-Bridges? (3,5,6,11?)

I guess that pretty much covers it for the Motor Shield, without going into [Electric Current](http://en.wikipedia.org/wiki/Electric_current) theory on the DC Motors and [Ohm's law](http://en.wikipedia.org/wiki/Ohm%27s_law). Take a look at the shift register output to see which motors are controlled depending on the bits being set.

Next step is to use PWM Timers on the development board to control the motor speeds.

### Development Board

Wow, when all you want to do is write code there is all this upfront learning that needs to take place! The actual code will be using the pinMode() and digitalWrite() methods but its important to understand how they work.

##### Port Manipulation

A very good starting point is this URL: http://www.arduino.cc/en/Reference/PortManipulation

I think the best place to start is to see an example in AVR C. There are comments in the code which show you the mapping to the Arduino libraries. If you upload this code onto your Leonardo development board it will blink the onboard LED.

```C++
#include <avr/io.h>
#include <util/delay.h>

// Built-In LED on Pin 13.
// Leonardo (ATmega32U4): PC7
uint8_t LED = (1 << 7); // another way to write logical shift is to use macro: _BV(7)

void setup() {
	DDRC |= LED; // pinMode(13, OUTPUT)
	PORTC &= ~LED; // digitalWrite(13, LOW)
}

void loop() {
	PORTC ^= LED; // togglePin(13)
	delay_ms(1000); // delay for 1 ms
}

void delay_ms(uint16_t ms) {
	// eat clock cycles
	while (ms) {
		_delay_ms(1); // call "util/delay.h"
		ms--;
	}
}
```

##### AVR Timers

AVR Timers at first are the most difficult concept to learn partly because its getting the configuration right. The reason I want to use timers is because I'd rather opt-in for hardware controlled timers rather than software controlled timers.

The best place to learn about AVR Timers (IMHO) would be this series of articles listed below.
I cannot describe Timers as well as Mayank Prasad, so do yourself a favour and read them!

- http://maxembedded.com/2011/06/introduction-to-avr-timers/
- http://maxembedded.com/2011/06/avr-timers-timer0/
- http://maxembedded.com/2011/06/avr-timers-timer1/
- http://maxembedded.com/2011/06/avr-timers-timer2/
- http://maxembedded.com/2011/07/avr-timers-ctc-mode/
- http://maxembedded.com/2011/08/avr-timers-pwm-mode-part-i/
- http://maxembedded.com/2012/01/avr-timers-pwm-mode-part-ii/

Okay lets get back to business. When our Motor Shield is connected to our Development board it's talking up Digital Pins 4,7,8,12. The Arduino can generate square waves for PWM on pins 3, 5, 6, 9, 10, 11 and great there is no overlap. Four of the PWM pins are wired directly to the four "enable inputs" on the two motor drivers.

The following shows PWM Pin and its associated motor:

Motor # | Pin #
------------ | ------------
1 | 11
2 | 3
3 | 6
4 | 5

Since we are using 8-bit PWM that means we can assign values from 0 to 255. So 127 is halfway there. That means a 50% duty. cycle on the motors.

More on this to come...with reference to datasheets.

# Known Issues

- Setting the motor speed to anything less than half (i.e. 127) results in the motors not turning.

# License

The project is licensed under MIT.

# References

- http://arduino.cc/en/Hacking/PinMapping32u4
- http://arduino.cc/en/Tutorial/SecretsOfArduinoPWM
- http://blog.oscarliang.net/arduino-timer-and-interrupt-tutorial/
- http://www.embedded.arch.ethz.ch/Examples/TimerDcMotorBackground
- http://extremeelectronics.co.in/avr-tutorials/pwm-signal-generation-by-using-avr-timers-part-ii/
- http://karplus4arduino.wordpress.com/2011/08/06/hexmotor-h-expanded-to-adafruit-motor-shield/
