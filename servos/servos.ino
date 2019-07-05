#include <Servo.h>

#define PIVOT 8
#define LIFT 9

Servo pivot, lift;

void setup() {
	pivot.attach (PIVOT);
	lift.attach (LIFT);
}

void loop() {
	pivot.write (0);
	lift.write (0);
	delay (1000);
	pivot.write (180);
	lift.write (180);
	delay (1000);
}