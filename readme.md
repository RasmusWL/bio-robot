# Chameleon robot

## Usage

Sketch should be running on the arduino, so just open the [Arduino IDE](http://www.arduino.cc/en/Main/Software#toc2) and start the `Serial Monitor` (found under tools).

It currently works so you can send "debug" commands to it:

* `M<left><right>` -- **Motor speeds**; both `<left>` and `<right>` must be within `[-255,255]`.
    Example: `M-127,255` means left motor half speed backwards, and right motor full speed ahead

* `P` -- **Proximity sensor**; get the distance from each sensor

* `CC` -- **Colour sensor calibration**; Put them over a white piece of paper.

* `CM` -- **colour sensor measurement**; will output someting like `s1 = 2 ~ 160 255 200 140`, meaning sensor 1 made a reading of (red = 160, green = 255, blue = 200, clear = 140) matching colour number 2 (blue).
*must calibrate before performing measurements*

* `D` -- **Disco time!**; have a party!

* `L?` -- **LED test**; Display one colour on the LEDs, either `R` `G` `B` or `W` for white, `D` for off.

## Colours

* 0. Red
* 1. Green
* 2. Blue
* 3. White
* 4. Orange
* 5. Yellow

## Changes

If you change the pin layout, edit the `pins.h` file and upload the sketch again.
