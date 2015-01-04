/*
Motor Shield Library

MIT License (MIT)
Copyright (c) 2015 Q-Smith
*/

#include <avr/io.h>
#include <Q4WD.h>

Q4WD car;

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println("backward");
  car.backward();
  delay(2000);
  Serial.println("forward");
  car.forward();
  delay(2000);
  Serial.println("left");
  car.left();
  delay(1200);
  Serial.println("forward");
  car.forward();
  delay(2000);
  Serial.println("right");
  car.right();
  delay(1200);
  Serial.println("forward");
  car.forward();
  delay(2000);
  Serial.println("stop");
  car.stop();
  delay(2000);
}
