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
	Serial.begin (9600);
	pivot.attach (PIVOT);
	lift.attach (LIFT);
	pinMode (13, OUTPUT);
	Serial.print("Slope is: ");
	Serial.println(slope);
}

void loop() {
	// for (int angle = 0; angle < 181; angle+=10) {
	// 	digitalWrite (13, HIGH);
	// 	float pulse = (slope * angle) + intercept;
	// 	Serial.print("Writing to ");
	// 	Serial.print(angle);
	// 	Serial.print (", which is ");
	// 	Serial.println (pulse);
	// 	pivot.write (pulse);
	// 	lift.write (pulse);
	// 	digitalWrite(13, LOW);
	// 	delay (10);
	// }
	float pulse;
	digitalWrite(13, HIGH);
	pulse = getPulse(0);
	pivot.write (pulse);
	lift.write (pulse);
	delay (1000);
	pulse = getPulse(90);
	pivot.write (pulse);
	lift.write (pulse);
	delay (500);	
	pulse = getPulse(180);
	pivot.write (pulse);
	lift.write (pulse);
	digitalWrite(13, LOW);
	delay (1000);	
}

// 0 -> 750
// 90 -> 1500
// 180 -> 2250