# Introduction

This project is about how to control an Arduino Motor Shield using AVR C. As a complete beginner in microcontroller programming, this project will hopefully give me insight into the following:

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
<img src="/imgs/kit-top.png" alt="Top View" style="width:100;height:100"> | <img src="/imgs/kit-side.png" alt="Side View" style="width:100;height:100"> | <img src="/imgs/kit-bottom.png" alt="Bottom View" style="width:100;height:100">

Since I believe this little project covers a lot of topics, the information outlined below does not go into expert detail on each topic and therefore a lot of assumptions are made about your understanding. However, I do try to provide some links to other sites which may have better explanations.

Some later extensions to this project might be to allow the end user to control the Motor Shield via some wireless communication. But for now, the scope is to just get the Development Board and the Motor Shield work together.

# Components &amp; Tools

### [IDE's](http://en.wikipedia.org/wiki/Integrated_development_environment)

There are two main development IDE's (IMHO) namely the [Arduino IDE](http://arduino.cc/en/Main/Software) and the [Atmel Studio](http://www.atmel.com/microsite/atmel_studio6/). For the purpose of this project I'll be using the Arduino IDE.

### Components

Name | Description | Picture
------------ | ------------- | -------------
Arduino Development Board | Target board will be the [Arduino Leonardo](http://arduino.cc/en/Main/arduinoBoardLeonardo), but I'll extend support for [Arduino UNO](http://arduino.cc/en/Main/arduinoBoardUno) as well. | <img src="/imgs/board-arduino-leonardo.png" alt="Arduino Leonardo" style="width:50;height:50">
Motor Shield | In my toolbox is the DK Electronics Motor Shield (version 1) and I need to get this working. | <img src="/imgs/shield-dk-motorshield.png" alt="DK Electronics Motor Shield" style="width:50;height:50">
4 Wheel Drive Chassis | In my toolbox is the ["4WD Robot Smart Car Kits Chassis"](http://www.ebay.com/itm/4WD-Robot-Smart-Car-Chassis-Kits-car-with-Speed-Encoder-DC-3v-5V-6V-for-Arduino-/171122633517) | <img src="/imgs/chassis-4WD.png" alt="4 Wheel Drive Chassis" style="width:50;height:50">
4 AA Battery Holder | A Battery Holder | <img src="/imgs/power-battery-holder.png" alt="Battery Holder" style="width:50;height:50">

### Datasheets / Schematics

TODO!

