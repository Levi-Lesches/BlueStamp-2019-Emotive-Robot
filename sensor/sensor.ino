#define ECHO A3
#define TRIGGER A2
#define THRESHOLD 6

bool isApproaching() {
	digitalWrite(TRIGGER, LOW);
	delayMicroseconds(2);

	digitalWrite(TRIGGER, HIGH);
	delayMicroseconds(10);
	digitalWrite(TRIGGER, LOW);

	long distance = pulseIn(ECHO, HIGH) * 0.034 / 2;

	return distance < THRESHOLD;
}

void setup() {
	Serial.begin (9600);
	pinMode (TRIGGER, OUTPUT);
	pinMode (ECHO, INPUT);
	pinMode (9, OUTPUT);
}

void loop() {
	if (isApproaching()) {
		Serial.println ("DANGER!");
		digitalWrite(9, HIGH);
	} else digitalWrite(9, LOW);
	delay (200);
}


/*
* Ultrasonic Sensor HC-SR04 and Arduino Tutorial
*
* by Dejan Nedelkovski,
* www.HowToMechatronics.com
*
*/
// defines pins numbers
// const int trigPin = 9;
// const int echoPin = 10;
// // defines variables
// long duration;
// int distance;
// void setup() {
// pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
// pinMode(echoPin, INPUT); // Sets the echoPin as an Input
// Serial.begin(9600); // Starts the serial communication
// }
// void loop() {
// // Clears the trigPin
// digitalWrite(trigPin, LOW);
// delayMicroseconds(2);
// // Sets the trigPin on HIGH state for 10 micro seconds
// digitalWrite(trigPin, HIGH);
// delayMicroseconds(10);
// digitalWrite(trigPin, LOW);
// // Reads the echoPin, returns the sound wave travel time in microseconds
// duration = pulseIn(echoPin, HIGH);
// // Calculating the distance
// distance= duration*0.034/2;
// // Prints the distance on the Serial Monitor
// Serial.print("Distance: ");
// Serial.println(distance);
// }