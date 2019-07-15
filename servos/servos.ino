#include <ServoTimer2.h>

// for servo.write: 
// do 25/3 (angle) + 750

#define PIVOT 6
#define LIFT 7

ServoTimer2 pivot, lift;
// Servo pivot, lift;

const float slope = (float)25/3;
const float intercept = 750;

float getPulse (int angle) {
	return (slope * angle) + intercept;
}

void setup() {
	pivot.attach (PIVOT);
	lift.attach (LIFT);
	pinMode (13, OUTPUT);
}

void loop() {
	digitalWrite(13, HIGH);
	float pulse;
	// Go to 0
	pulse = getPulse(0);
	pivot.write (pulse);
	lift.write (pulse);
	delay (1000);  // break from 180 to 0
	// Go to 90
	pulse = getPulse(90);
	pivot.write (pulse);
	lift.write (pulse);
	delay (500);	
	// Go to 180
	pulse = getPulse(180);
	pivot.write (pulse);
	lift.write (pulse);
	digitalWrite(13, LOW);  // done
	delay (500);	
}
