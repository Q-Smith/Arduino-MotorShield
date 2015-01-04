/*
Motor Shield Library

MIT License (MIT)
Copyright (c) 2015 Q-Smith
*/

#include <avr/io.h>

#if ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Q4WD.h"

Q4WD::Q4WD() {
	// Init Shift Register
	pinMode(MOTORCLK, OUTPUT);
	pinMode(MOTORENABLE, OUTPUT); // is this required?
	pinMode(MOTORDATA, OUTPUT);
	pinMode(MOTORLATCH, OUTPUT);

	stop();

	// Init Timers
	_timer1.init();
	_timer2.init();
	_timer3.init();
	_timer4.init();
}

void Q4WD::forward() {
	stop();

	// Motor 1
	_timer1.pwm(255);
	_shiftState |= _BV(MOTOR1_A);
	_shiftState &= ~_BV(MOTOR1_B);
	changeDirection();

	// Motor 2
	_timer2.pwm(255);
	_shiftState |= _BV(MOTOR2_A);
	_shiftState &= ~_BV(MOTOR2_B);
	changeDirection();

	// Motor 3
	_timer3.pwm(255);
	_shiftState |= _BV(MOTOR3_A);
	_shiftState &= ~_BV(MOTOR3_B);
	changeDirection();

	// Motor 4
	_timer4.pwm(255);
	_shiftState |= _BV(MOTOR4_A);
	_shiftState &= ~_BV(MOTOR4_B);
	changeDirection();
}

void Q4WD::backward() {
	stop();

	// Motor 1
	_timer1.pwm(255);
	_shiftState |= _BV(MOTOR1_B);
	_shiftState &= ~_BV(MOTOR1_A);
	changeDirection();

	// Motor 2
	_timer2.pwm(255);
	_shiftState |= _BV(MOTOR2_B);
	_shiftState &= ~_BV(MOTOR2_A);
	changeDirection();

	// Motor 3
	_timer3.pwm(255);
	_shiftState |= _BV(MOTOR3_B);
	_shiftState &= ~_BV(MOTOR3_A);
	changeDirection();

	// Motor 4
	_timer4.pwm(255);
	_shiftState |= _BV(MOTOR4_B);
	_shiftState &= ~_BV(MOTOR4_A);
	changeDirection();
}

void Q4WD::left() {
	stop();

	// Motor 1
	_timer1.pwm(255);
	_shiftState |= _BV(MOTOR1_A);
	_shiftState &= ~_BV(MOTOR1_B);
	changeDirection();

	// Motor 2
	_timer2.pwm(128);
	_shiftState |= _BV(MOTOR2_B);
	_shiftState &= ~_BV(MOTOR2_A);
	changeDirection();

	// Motor 3
	_timer3.pwm(255);
	_shiftState |= _BV(MOTOR3_A);
	_shiftState &= ~_BV(MOTOR3_B);
	changeDirection();

	// Motor 4
	_timer4.pwm(128);
	_shiftState |= _BV(MOTOR4_B);
	_shiftState &= ~_BV(MOTOR4_A);
	changeDirection();
}

void Q4WD::right() {
	stop();

	// Motor 1
	_timer1.pwm(128);
	_shiftState |= _BV(MOTOR1_B);
	_shiftState &= ~_BV(MOTOR1_A);
	changeDirection();

	// Motor 2
	_timer2.pwm(255);
	_shiftState |= _BV(MOTOR2_A);
	_shiftState &= ~_BV(MOTOR2_B);
	changeDirection();

	// Motor 3
	_timer3.pwm(128);
	_shiftState |= _BV(MOTOR3_B);
	_shiftState &= ~_BV(MOTOR3_A);
	changeDirection();

	// Motor 4
	_timer4.pwm(255);
	_shiftState |= _BV(MOTOR4_A);
	_shiftState &= ~_BV(MOTOR4_B);
	changeDirection();
}

void Q4WD::stop() {
	_shiftState = 0;

	// Motor 1
	_shiftState &= ~_BV(MOTOR1_A);
	_shiftState &= ~_BV(MOTOR1_B);
	changeDirection();

	// Motor 2
	_shiftState &= ~_BV(MOTOR2_A);
	_shiftState &= ~_BV(MOTOR2_B);
	changeDirection();

	// Motor 3
	_shiftState &= ~_BV(MOTOR3_A);
	_shiftState &= ~_BV(MOTOR3_B);
	changeDirection();

	// Motor 4
	_shiftState &= ~_BV(MOTOR4_A);
	_shiftState &= ~_BV(MOTOR4_B);
	changeDirection();
}

void Q4WD::changeDirection() {
	digitalWrite(MOTORCLK, LOW); // set the shift register clock pin low
	digitalWrite(MOTORLATCH, LOW); // set the storage register clock pin low (close the latch gate)

	for (int i=0; i<8; i++) {
		if (_shiftState & _BV(7-i)) {
			digitalWrite(MOTORDATA, HIGH); // set the serial data input pin high
		} else {
			digitalWrite(MOTORDATA, LOW); // set the serial data input pin low
		}

		digitalWrite(MOTORCLK, HIGH); // set the shift register clock pin high
		digitalWrite(MOTORCLK, LOW); // set the shift register clock pin low
	}

	digitalWrite(MOTORLATCH, HIGH); // set the storage register clock pin low (open the latch gate)
}

Timer1::Timer1() {
	pinMode(11, OUTPUT);

#if defined(__AVR_ATmega328P__)
	_prescalar = _BV(CS20); // no prescale
#elif defined(__AVR_ATmega32U4__)
	_prescalar = _BV(CS00); // no prescale
#endif
}

void Timer1::init() {
#if defined(__AVR_ATmega328P__) // Uno
	// (Arduino pin #11)
	TCCR2A |= _BV(COM2A1) | _BV(WGM20) | _BV(WGM21); // fast PWM, turn on oc2a
	TCCR2B = _prescalar & 0x7;
	OCR2A = 0;
#elif defined(__AVR_ATmega32U4__) // Leonardo
	// (Arduino pin #11)
	TCCR0A |= _BV(COM0A1) | _BV(WGM00) | _BV(WGM01); // fast PWM, turn on OC0A
	OCR0A = 0;
#else
	#error "This chip is not supported!"
#endif
}

void Timer1::pwm(uint8_t value) {
#if defined(__AVR_ATmega328P__) // Uno
	// (Arduino pin #11)
	OCR2A = value;
#elif defined(__AVR_ATmega32U4__) // Leonardo
	// (Arduino pin #11)
	OCR0A = value;
#else
   #error "This chip is not supported!"
#endif
}

Timer2::Timer2() {
	pinMode(3, OUTPUT);

#if defined(__AVR_ATmega328P__)
	_prescalar = _BV(CS20); // no prescale
#elif defined(__AVR_ATmega32U4__)
	_prescalar = _BV(CS00); // no prescale
#endif
}

void Timer2::init() {
#if defined(__AVR_ATmega328P__) // Uno
	// (Arduino pin #3)
	TCCR2A |= _BV(COM2B1) | _BV(WGM20) | _BV(WGM21); // fast PWM, turn on oc2b
	TCCR2B = _prescalar & 0x7;
	OCR2B = 0;
#elif defined(__AVR_ATmega32U4__) // Leonardo
	// (Arduino pin #3)
	TCCR0A |= _BV(COM0B1) | _BV(WGM00) | _BV(WGM01); // fast PWM, turn on OC0B
	OCR0B = 0;
#else
	#error "This chip is not supported!"
#endif
}

void Timer2::pwm(uint8_t value) {
#if defined(__AVR_ATmega328P__) // Uno
	// (Arduino pin #3)
	OCR2B = value;
#elif defined(__AVR_ATmega32U4__) // Leonardo
	// (Arduino pin #3)
	OCR0B = value;
#else
   #error "This chip is not supported!"
#endif
}

Timer3::Timer3() {
	pinMode(6, OUTPUT);

#if defined(__AVR_ATmega328P__)
	_prescalar = _BV(CS00); // no prescale
#else
	_prescalar = _BV(CS40); // no prescale
#endif
}

void Timer3::init() {
#if defined(__AVR_ATmega328P__) // Uno
	// (Arduino pin #6)
	TCCR0A |= _BV(COM0A1) | _BV(WGM00) | _BV(WGM01); // fast PWM, turn on OC0A
	OCR0A = 0;
#elif defined(__AVR_ATmega32U4__) // Leonardo
	// (Arduino pin #6)
	TCCR4B = _BV(CS42) | _BV(CS41);
	TCCR4C = _BV(PWM4D) |_BV(COM4D1);
	TCCR4D = _BV(WGM40);
#else
	#error "This chip is not supported!"
#endif
}

void Timer3::pwm(uint8_t value) {
#if defined(__AVR_ATmega328P__) // Uno
	// (Arduino pin #6)
	OCR0A = value;
#elif defined(__AVR_ATmega32U4__) // Leonardo
	// (Arduino pin #6)
	OCR4D = value;
#else
   #error "This chip is not supported!"
#endif
}

Timer4::Timer4() {
	pinMode(5, OUTPUT);

#if defined(__AVR_ATmega328P__)
	_prescalar = _BV(CS00); // no prescale
#else
	_prescalar = _BV(CS40); // no prescale
#endif
}

void Timer4::init() {
#if defined(__AVR_ATmega328P__) // Uno
	// (Arduino pin #5)
	TCCR0A |= _BV(COM0B1) | _BV(WGM00) | _BV(WGM01); // fast PWM, turn on oc0a
	OCR0B = 0;
#elif defined(__AVR_ATmega32U4__) // Leonardo
	// (Arduino pin #5)
	TCCR3B = 0;
	TCCR3B |=  _BV(CS31) | _BV(CS30);
	TCCR3A = 0;
	TCCR3A |= _BV(COM3A1) | _BV(WGM30);
	OCR3A = 0;
#else
	#error "This chip is not supported!"
#endif
}

void Timer4::pwm(uint8_t value) {
#if defined(__AVR_ATmega328P__) // Uno
	// (Arduino pin #5)
	OCR0B = value;
#elif defined(__AVR_ATmega32U4__) // Leonardo
	// (Arduino pin #5)
	OCR3A = value;
#else
   #error "This chip is not supported!"
#endif
}