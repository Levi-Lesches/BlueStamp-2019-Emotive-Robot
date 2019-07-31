/* 
	This sketch is used to test whether something is approaching the sensor
	It prints "DANGER!" to the Serial monitor if something is within THRESHOLD cm
	
*/

#define ECHO A3
#define TRIGGER A2
#define THRESHOLD 6

bool isApproaching() {
	// Flush the trigger's power
	digitalWrite(TRIGGER, LOW);
	delayMicroseconds(2);

	// Trigger a 10 micro-second sound wave pulse
	digitalWrite(TRIGGER, HIGH);
	delayMicroseconds(10);
	digitalWrite(TRIGGER, LOW);

	// Calculate the distance based on the incoming sound wave
	long distance = pulseIn(ECHO, HIGH) * 0.034 / 2;

	// Determine whether soemthing is approaching
	return distance < THRESHOLD;
}

void setup() {
	Serial.begin (9600);
	pinMode (TRIGGER, OUTPUT);
	pinMode (ECHO, INPUT);
	pinMode (13, OUTPUT);
}

void loop() {
	if (isApproaching()) {
		Serial.println ("DANGER!");
		digitalWrite(13, HIGH);
	} else digitalWrite(13, LOW);
	delay (200);
}
