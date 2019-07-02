// REMINDER: File names need to be all-caps

// Specs on UNO
// Storage space: 27%
// dynamic memory: 69%

#include <WaveHC.h>

FatReader root;

void play(String stringName, bool interrupt) {
	// Copy the filename to a char array
	int length = stringName.length() + 2;  // get the length of the filename
	char name[length];  // declare the array
	stringName.toCharArray(name, length);  // store the string in the array

	// Actually play the file
	WaveHC wave;  // store audio data
	Serial.print("Playing ");
	Serial.println(name);
	FatReader file;  // declare the file
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
	SdReader sdCard;  // declare the SD card
	FatVolume volume;  // Needed for reading the SD card
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