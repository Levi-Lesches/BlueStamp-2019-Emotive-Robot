/*
	Code for the slave Arduino.
	This device *only* controls audio, nothing else
	Analog signals are sent from the master
	The signal sent corresponds with the index of the file in "filenames"

	It is important to keep audio functionality separate since the WaveHC 
	library uses a lot of memory thus straining any other functionality.  

	Specs: 
		Storage space: 29%
		Dynamic memory: 77% (told you)
*/

#include <WaveHC.h>

#define SIGNAL A0
#define NUM_FILES 5

// This must sync up with the master's Sounds enum
const String filenames[NUM_FILES] = {
	"BLINK.WAV", "NEUTRAL.WAV", "SCARED.WAV", "NEUTRMOV.WAV", "SCAREMOV.WAV"
};

FatReader root;
WaveHC wave;  // store audio data
FatReader file;  // declare the file
FatVolume volume;  // Needed for reading the SD card
SdReader sdCard;  // declare the SD card

int getSignal() {
	return analogRead(SIGNAL);
}

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
	int signal = getSignal();
	if (signal != 0)  // kill code
		// Use signal - 1 since we added 1 to avoid 0 (kill code)
		play(filenames [signal - 1], true);
	delay (500);
}
