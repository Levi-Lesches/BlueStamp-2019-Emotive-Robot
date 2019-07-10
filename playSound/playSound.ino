/*
	REMINDER: File names need to be all-caps
	TODO: replace WaveHC library
	TODO: Check wave.play for references to FatVolume and SdReader 

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

// 2, 2, 2, 2, 2
// servo (2), ultrasonic (2), LCD (2), LED (4)
// = 2 + 2 + 2 + 4 = 6 + 4 = 10

#include <WaveHC.h>

FatReader root;
WaveHC wave;  // store audio data
FatReader file;  // declare the file
FatVolume volume = FatVolume();  // Needed for reading the SD card
SdReader sdCard = SdReader();  // declare the SD card

void play(String stringName, bool interrupt) {
	// Copy the filename to a char array
	int length = stringName.length() + 2;  // get the length of the filename
	char name[length];  // declare the array
	stringName.toCharArray(name, length);  // store the string in the array

	// Actually play the file
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

void setup() {
	Serial.begin (9600);
	Serial.println("Initializing SD card...");

	delay (1000);
	if (!sdCard.init()) Serial.println ("Card could not be read");
	sdCard.partialBlockRead(true);  // Cool little trick for faster reading
	// This just initializes file reading and writing
	if (!volume.init(sdCard)) Serial.println ("Volume could not be opened");
	if (!root.openRoot(volume)) Serial.println ("Root could not be opened");
	Serial.println ("Card initialized");
}

void loop(){
	play("TEMP.WAV", true);
}