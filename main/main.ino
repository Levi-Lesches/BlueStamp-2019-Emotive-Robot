/*
	SOLUTIONS: 
		sdcard.init(TRUE)
		move all sound-related variables to global

	TODO: replace WaveHC library
	TODO: re-theme Arduino
	TODO: replace specs
	TODO: remove <Adafruit_GFX.h>

	Specs on UNO
	Storage space: 50%
	dynamic memory: 90%

	pins that still work with audio: 
	A0,
	A1,
	A2, 
	A3, 
	A4, (I2C SDA)
	A5, (I2C SCL)
	6, (Servo pivot)
	7, (Servo lift)
	8,
	9, (BUILTIN)
*/

#include <WaveHC.h>
#include <ServoTimer2.h>
// #include <LiquidCrystal_I2C.h>
// #include <Adafruit_LEDBackpack.h>
// #include <Adafruit_GFX.h>  // for bitmap

#define PIVOT 6
#define LIFT 7
#define BUILTIN A0
#define TRIGGER A2
#define ECHO A3

#define THRESHOLD 7

// https://xantorohara.github.io/led-matrix-editor/#3c4299a581a5423c
// const uint8_t smile[8] = {
//   B00111100,
//   B01000010,
//   B10100101,
//   B10000001,
//   B10100101,
//   B10011001,
//   B01000010,
//   B00111100
// };

const float slope = (float)25/3;
const float intercept = 750;

// Adafruit_8x8matrix matrix = Adafruit_8x8matrix();
// LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

ServoTimer2 pivot, lift;

FatReader root;
FatVolume volume;  // Needed for reading the SD card
SdReader sdCard;  // declare the SD card

bool isApproaching() {
	digitalWrite(TRIGGER, LOW);
	delayMicroseconds(2);

	digitalWrite(TRIGGER, HIGH);
	delayMicroseconds(10);
	digitalWrite(TRIGGER, LOW);

	long distance = pulseIn(ECHO, HIGH) * 0.034 / 2;

	return distance < THRESHOLD;
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
		Serial.println ("Does not exist");
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
	return (slope * angle) + intercept;
}

// void drawBitmap(uint8_t bitmap[8]) {
// 	// Serial.println("Imagine this working...");
//   // matrix.drawBitmap (0, 0, bitmap, 8, 8, LED_ON);
//   // matrix.writeDisplay();
// }

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

	// Setup the LED matrices (they share the same pins)
	// matrix.begin();

	// LCD screen
	// lcd.init();
	// lcd.backlight();
	// lcd.clear();
	// lcd.print ("Hello, World!");

	// Sensor setup
	pinMode (TRIGGER, OUTPUT);
	pinMode (ECHO, INPUT);
}


void loop(){
	if (isApproaching()) {
		Serial.println ("Sensor triggered");
		// Audio control
		play("TEMP.WAV", false);
		// drawBitmap(smile);
		// lcd.print ("Danger!");
		digitalWrite(BUILTIN, HIGH);
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
		digitalWrite(BUILTIN, LOW);
	} else {
		// matrix.clear();
		// matrix.writeDisplay();
		// lcd.clear();
	}
}