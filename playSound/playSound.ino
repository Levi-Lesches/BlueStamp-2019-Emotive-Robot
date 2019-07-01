
// REMINDER: File names need to be all-caps
// TODO: attach speaker

#include <WaveHC.h>
// #include <WaveUtil.h>
// #include <SD.h>

SdReader sdCard;
FatVolume volume;
FatReader fatRoot, file;
WaveHC wave;

// File root;

void play(String stringName, bool interrupt) {
	// Copy the filename to a char array
	int length = stringName.length() + 2;
	char name[length];
	stringName.toCharArray(name, length);

	// Actually play the file
	Serial.print("Playing ");
	Serial.println(name);
	file.open (fatRoot, name);
	if (!wave.create (file))  // Invalid WAVE file
		Serial.println ("Not a valid wave file");
	wave.play();
	if (interrupt) {  // need to stop all other code
		while (wave.isplaying) {
			Serial.print(".");
			delay (100);
		}
	}
}

void setup() {
	// if (!SD.begin()) {
	// 	Serial.println ("Cannot open SD card");
	// 	while(true);
	// }
	// root = SD.open ("/");
	Serial.begin (9600);
	Serial.println("Initializing SD card...");
	if (!sdCard.init()) Serial.println ("Card could not be read");
	sdCard.partialBlockRead(true);
	if (!volume.init(sdCard)) Serial.println ("Volume could not be opened");
	if (!fatRoot.openRoot(volume)) Serial.println ("Root could not be opened");
	Serial.println ("Card initialized");
}

void loop(){
	play("TEMP.WAV", true);
}