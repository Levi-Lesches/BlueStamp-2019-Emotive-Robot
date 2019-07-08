/*
	REMINDER: Wave File names need to be all-caps
	TODO: replace WaveHC library
	TODO: update specs

	Specs on UNO
	Storage space: 27%
	dynamic memory: 71%

	pins that still work with audio: 
	A0, (ultra 1)
	A1, (ultra 2)
	A2, 
	A3, 
	A4,
	A5,
	6, (BUILTIN, 
	7,
	8,
	9,
*/

#include <WaveHC.h>
#include <ServoTimer2.h>

#define PIVOT 6
#define LIFT 7

const float slope = (float)25/3;
const float intercept = 750;

ServoTimer2 pivot, lift;

FatReader root;
FatVolume volume;  // Needed for reading the SD card
SdReader sdCard;  // declare the SD card

void play(String stringName, bool interrupt) {
	// Copy the filename to a char array
	int length = stringName.length() + 2;  // get the length of the filename
	char name[length];  // declare the array
	stringName.toCharArray(name, length);  // store the string in the array

	// Actually play the file
	WaveHC wave;  // store audio data
	FatReader file;  // declare the file

	Serial.print("Playing ");
	Serial.println(name);
	file.open (root, name);  // store data in the file
	if (!wave.create (file))  // Invalid .wav file
		Serial.println ("Not a valid wave file");
	wave.play();  // actually play the .wav file
	if (interrupt) {  // need to stop all other code
		while (wave.isplaying) {  // print dots to the Serial monitor
			Serial.print(".");
			delay (100);
		}
	}
}

float getPulse (int angle) {
	return (slope * angle) + intercept;
}


void setup() {
	Serial.begin (9600);

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
}

void loop(){
	// Audio control
	play("TEMP.WAV", false);

	// Servo sweep
	float pulse;
	// set both servos to 0
	pulse = getPulse(0);
	pivot.write (pulse);
	lift.write (pulse);
	delay (1000);  // allow time from 180 to 0
	// set both servos to 90
	pulse = getPulse(90);
	pivot.write (pulse);
	lift.write (pulse);
	delay (500);	
	// Set both servos to 180
	pulse = getPulse(180);
	pivot.write (pulse);
	lift.write (pulse);
	delay (500);	
}