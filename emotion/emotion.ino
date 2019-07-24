#include <Adafruit_LEDBackpack.h>
#include <Servo.h>

/*
	Here we have 4 pins to receive commands from (no analog pins)
	So... we can use binary to use these input efficiently!
	The table of commands is as follows (blinking can be managed locally)
		+-------+-------+-------+-------+
		|   1   |   2   |   3   |   4   |
		+-------+-------+-------+-------+
		| True  | True  | True  | True  | => happy
		+-------+-------+-------+-------+
		| True  | True  | True  | False | => sad
		+-------+-------+-------+-------+
		| True  | True  | False | True  | => angry
		+-------+-------+-------+-------+
		| True  | True  | False | False | => love
		+-------+-------+-------+-------+
		| True  | False | True  | True  | => neutral
		+-------+-------+-------+-------+
		| True  | False | True  | False |
		+-------+-------+-------+-------+
		| True  | False | False | True  |
		+-------+-------+-------+-------+
		| True  | False | False | False |
		+-------+-------+-------+-------+
		| False | True  | True  | True  |
		+-------+-------+-------+-------+
		| False | True  | True  | False |
		+-------+-------+-------+-------+
		| False | True  | False | True  |
		+-------+-------+-------+-------+
		| False | True  | False | False |
		+-------+-------+-------+-------+
		| False | False | True  | True  |
		+-------+-------+-------+-------+
		| False | False | True  | False |
		+-------+-------+-------+-------+
		| False | False | False | True  |
		+-------+-------+-------+-------+
		| False | False | False | False |
		+-------+-------+-------+-------+

*/

// Signal pins
#define PIN1 A0
#define PIN2 A1
#define PIN3 A2
#define PIN4 A3
// Servo pins
#define LEFT_EYEBROW 2
#define RIGHT_EYEBROW 3

#define NUM_EMOTIONS 6

#define BLINK_DELAY 500
#define BLINK_INTERVAL 3000

enum Emotion {
	BLINK,
	HAPPY, 
	SAD,
	ANGRY,
	LOVE,
	NEUTRAL,
	// confused
};

// Each index corresponds to an Emotion
const uint8_t BITMAPS[NUM_EMOTIONS][8] = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

// Eyebrow angles:
// 	true -- all the way up (think: happy)
// 	false -- all the way down (think: angry)
// 	NULL -- neutral
const bool EYEBROW_ANGLES[NUM_EMOTIONS] = {
	NULL, 
	true,
	true,
	false,
	true,
	NULL,
};


// Hardware
Adafruit_8x8matrix matrix;
Servo leftEyebrow, rightEyebrow;

// Software
Emotion currentEmotion = NEUTRAL;
int movementDelay = 5;  // always in seconds
int lastMove, lastBlink;

void setup() {
	Serial.begin(9600);
	pinMode (PIN1, INPUT);
	pinMode (PIN2, INPUT);
	pinMode (PIN3, INPUT);
	pinMode (PIN4, INPUT);
	matrix.begin(0x70);
	leftEyebrow.attach(LEFT_EYEBROW);
	rightEyebrow.attach(RIGHT_EYEBROW);
	lastMove = lastBlink = millis();
}

void loop() {
	int newTime = millis();
	if (newTime - lastMove >= movementDelay * 1000) {
		moveFace();
		lastMove = newTime;
	}
	if (newTime - lastBlink >= BLINK_INTERVAL) {
		blink();
		lastBlink = newTime;
	}
}

void setEye(uint8_t bitmap[8]) {
	matrix.clear();
	matrix.drawBitmap (0, 0, bitmap, 8, 8, LED_ON);
	matrix.writeDisplay();  // needed to update matrix
}

void setEyebrow(bool angle) {
	int actualAngle = angle == NULL ? 90
		: angle ? 0 : 180;
	Serial.print("Setting eyebrows to ");
	Serial.print(angle);
	Serial.println(" degrees");
	leftEyebrow.write(angle);
	rightEyebrow.write(angle);		
}

// Helper functions to dictate pan and tilt motion
void blink() {
	// No head movement here, just eyes
	setEye(BLINK);
	delay(BLINK_DELAY);
	setEye(currentEmotion);
}
void happy() {

}
void sad() {}
void angry() {}
void love() {}


Emotion getCommand() {
	// see truth table above for commands
	// returns emotion received from master
	bool pin1 = digitalRead(PIN1);
	bool pin2 = digitalRead(PIN2);
	bool pin3 = digitalRead(PIN3);
	bool pin4 = digitalRead(PIN4);
	if (!pin1) return NULL;
	 || !pin2 return NULL;
	if (pin3) return pin4 ? HAPPY : SAD;
	else return pin4 ? ANGRY : LOVE;
}

void switchEmotion(Emotion emotion) {
	currentEmotion = emotion
	switch (emotion) {
		case BLINK: return blink();
		case HAPPY: return happy();
		case SAD: return sad();
		case ANGRY: return angry();
		case LOVE: return love();
	}
	setEye (BITMAPS [emotion]);
	setEyebrow (EYEBROW_ANGLES [emotion]);
}