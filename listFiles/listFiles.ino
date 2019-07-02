#include <SD.h>

File root;

void listFiles() {
	while (true) {
		File file = root.openNextFile();
		if (!file)
			break;
		Serial.println (file.name());
		file.close();
	}
}

void setup() {
	Serial.begin (9600);
	Serial.println("Initializing SD card...");
	if (!SD.begin()) {
		Serial.println ("Cannot initalize SD card");
		return;
	}
	root = SD.open ("/");
	if (!root) {
		Serial.println ("Cannot open SD card");
		return;
	}
	Serial.println ("Card initialized");
	listFiles();
}

void loop(){}