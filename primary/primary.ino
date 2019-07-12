/*
	SOLUTIONS FOR WORKING WITH I2C AND WAVE SHIELD AT THE SAME TIME
		sdcard.init(*TRUE*)
		move all sound-related variables to global
		replace the timer for the WaveHC library

	TODO: replace WaveHC library
	TODO: replace specs
	TODO: re-theme Arduino syntax

	Specs on UNO
		Storage space: 33%
		dynamic memory: 75%

	pin wiring: 
		A0 --> BUILTIN
		A1 --> Free
		A2 --> Ultrasonic trigger [TRIGGER]
		A3 --> Ultrasonic echo [ECHO]
		A4 --> I2C SDA -- temporarily moved to slave Arduino
		A5 --> I2C SCL -- temporarily moved to slave Arduino
		0 --> RX (Serial output)
		1 --> TX (Serial input -- will be used for Bluetooth)
		2 --> Wave Shield
		3 --> Wave Shield
		4 --> Wave Shield
		5 --> Wave Shield
		6 --> Pivot servo [PIVOT]
		7 --> Lift servo [LEFI]
		8 --> Left eyebrow servo [LEFT_EYEBROW]
		9 --> Right eyebrow servo [RIGHT_EYEBROW]
		10 --> Wave Shield
		11 --> Wave Shield
		12 --> Wave Shield
		13 --> Wave Shield
*/

#include <WaveHC.h>
#include <ServoTimer2.h>

#define PIVOT 6
#define LIFT 7
#define BUILTIN A0
#define TRIGGER A2
#define ECHO A3
#define LEFT_EYEBROW 8
#define RIGHT_EYEBROW 9

#define SENSOR_THRESHOLD 7

// When using ServoTimer2, you need to send the width of the voltage pulse,
// not an angle. These are used in getPulse(int) to calculate the width 
// based on the desired angle 
const float slope = (float)25/3;
const float intercept = 750;

// Using ServoTimer2 instead of Servo since 
// the Wave Shield uses the default timer
ServoTimer2 pivot, lift, left_eyebrow, right_eyebrow;

// Variables for reading the SD card
FatReader root;     // keep a reference to the root directory
FatVolume volume;  // Needed for reading the SD card
SdReader sdCard;  // declare the SD card

bool isApproaching() {
	/* This function returns whether the user is close to the sensor */

	// Flush the current 
	digitalWrite(TRIGGER, LOW);
	delayMicroseconds(2);

	// Trigger a 10 micro-second sound wave
	digitalWrite(TRIGGER, HIGH);
	delayMicroseconds(10);
	digitalWrite(TRIGGER, LOW);

	// Read the incoming sound wave
	long distance = pulseIn(ECHO, HIGH) * 0.034 / 2;

	// Returns the distance (I'm relatively sure it's cm)
	return distance < SENSOR_THRESHOLD;
}

void play(String stringName, bool interrupt) {
	// Plays the name of this file 
	// REMINDER: Wave File names need to be all-caps

	// Copy the filename to a char array
	int length = stringName.length() + 2;  // get the length of the filename
	char name[length];  // declare the array
	stringName.toCharArray(name, length);  // store the string in the array

	// Actually play the file
	WaveHC wave;  // store audio data
	FatReader file;  // declare the file
	Serial.print("Playing ");
	Serial.println(name);
	if (!file.open (root, name)) {  // store data in the file
		Serial.print(stringName); 
		Serial.println (" does not exist");
		return;
	}

	if (!wave.create (file))  {  // Invalid .wav file
		Serial.println ("Not a valid wave file");
		return;
	}
	wave.play();  // actually play the .wav file
	if (interrupt) {  // need to stop all other code
		while (wave.isplaying) {  // print dots to the Serial monitor
			Serial.print(".");
			delay (100);
		}
	}
}

float getPulse (int angle) {
	// ServoTimers need a pulse width instead of an angle
	// formula here is: (25/3)(angle) + 750
	// constants are defined at the top of the file
	return (slope * angle) + intercept;
}

void setup() {
	Serial.begin (9600);
	pinMode (BUILTIN, OUTPUT);

	// Setup SD card
	Serial.println("Initializing SD card...");
	if (!sdCard.init()) Serial.println ("Card could not be read");
	sdCard.partialBlockRead(true);  // Cool little trick for faster reading
	// This just initializes file reading and writing
	if (!volume.init(sdCard)) Serial.println ("Volume could not be opened");
	if (!root.openRoot(volume)) Serial.println ("Root could not be opened");
	Serial.println ("Card initialized");

	// Setup servos 
	pivot.attach (PIVOT);
	lift.attach (LIFT);
	left_eyebrow.attach (LEFT_EYEBROW);
	right_eyebrow.attach (RIGHT_EYEBROW);

	// Sensor setup
	pinMode (TRIGGER, OUTPUT);
	pinMode (ECHO, INPUT);
}


void loop(){
	if (isApproaching()) {
		Serial.println ("Sensor triggered");
		// Audio control

		play("TEMP.WAV", false);

		// Servo sweep
		digitalWrite(BUILTIN, HIGH);
		float pulse;
		// set servos to 0
		pulse = getPulse(0);
		pivot.write (pulse);
		lift.write (pulse);
		left_eyebrow.write (pulse);
		right_eyebrow.write (pulse);
		delay (1000);  // allow time from 180 to 0
		// set servos to 90
		pulse = getPulse(90);
		pivot.write (pulse);
		lift.write (pulse);
		left_eyebrow.write (pulse);
		right_eyebrow.write (pulse);
		delay (500);	
		// Set servos to 180
		pulse = getPulse(180);
		pivot.write (pulse);
		lift.write (pulse);
		left_eyebrow.write (pulse);
		right_eyebrow.write (pulse);
		delay (500);	
		digitalWrite(BUILTIN, LOW);
	}
}