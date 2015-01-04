/*
Motor Shield Library

MIT License (MIT)
Copyright (c) 2015 Q-Smith
*/

#ifndef _Q4WD_h_
#define _Q4WD_h_

#include <avr/io.h>

#define MOTOR1_A 2
#define MOTOR1_B 3
#define MOTOR2_A 1
#define MOTOR2_B 4
#define MOTOR4_A 0
#define MOTOR4_B 6
#define MOTOR3_A 5
#define MOTOR3_B 7

#define MOTORCLK 4
#define MOTORENABLE 7
#define MOTORDATA 8
#define MOTORLATCH 12

class Timer1 {
public:
	Timer1(); // constructor
	void init(); // sets the timers prescalar
	void pwm(uint8_t value); // sets the compare register
private:
	uint8_t _prescalar;
};

class Timer2 {
public:
	Timer2(); // constructor
	void init(); // sets the timers prescalar
	void pwm(uint8_t value); // sets the compare register
private:
	uint8_t _prescalar;
};

class Timer3 {
public:
	Timer3(); // constructor
	void init(); // sets the timers prescalar
	void pwm(uint8_t value); // sets the compare register
private:
	uint8_t _prescalar;
};

class Timer4 {
public:
	Timer4(); // constructor
	void init(); // sets the timers prescalar
	void pwm(uint8_t value); // sets the compare register
private:
	uint8_t _prescalar;
};

class Q4WD {
public:
	Q4WD(); // constructor
	//void speed(uint8_t value); // controls the directions speed (0-255)
	void forward(); // move forward
	void backward(); // reverse and move backwards
	void left(); // turn left
	void right(); // turn right
	void stop(); // stops all motors
private:
	void changeDirection();
	uint8_t _shiftState;
	Timer1 _timer1;
	Timer2 _timer2;
	Timer3 _timer3;
	Timer4 _timer4;
};

#endif
