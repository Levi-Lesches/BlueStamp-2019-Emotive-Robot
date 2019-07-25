/*
	This is code for the master Arduino. 

	It is in charge of emotion handling, which includes: 
		1. eyebrows
		2. eyes
		3. head/neck movements
		4. proximity sensor (to be surprised)

	Audio is handled by the slave Arduino, which is equipped with a Wave Shield
	Commands are sent using an analog signal which is used as an index to a list
	More details can be found in the code for the slave Arduino

	Wiring for the master Arduino:
		2 --> pan servo (PAN)
		3 --> tilt servo (TILT)
		4 --> left eyebrow servo (LEFT_EYEBROW)
		5 --> right eyebrow servo (RIGHT_EYEBROW)
		6 --> proximity sensor echo (ECHO)
		7 --> proximity sensor trigger (TRIGGER)

	Specs: 
		Storage space: 41% 
		Dynamic memory: 31%
*/

#include <Adafruit_LEDBackpack.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Servos
#define PAN 2
#define TILT 3
#define LEFT_EYEBROW 4
#define RIGHT_EYEBROW 5

// Proximity sensor
#define ECHO 6
#define TRIGGER 7
#define SENSOR_THRESHOLD 7  // cm, I believe

// Blinking
#define BLINK_DELAY 500
#define BLINK_INTERVAL 3000

// Moving the head
#define HEAD_MIN_ANGLE 20
#define HEAD_MAX_ANGLE 160

// Misc.
#define CALM_DOWN_DELAY 2500
#define NUM_EMOTIONS 2
#define NUM_MOVEMENT_FREQUENCIES 2
#define SIGNAL 11  // MUST BE PWM

// constants
const uint8_t BLINK[8] = {
	B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B01000010,
  B00111100,
  B00000000,
};

const uint8_t BITMAPS[NUM_EMOTIONS][8] = {
	{  // NEUTRAL
		B00000000,
	  B00111100,
	  B01000010,
	  B01011010,
	  B01011010,
	  B01000010,
	  B00111100,
	  B00000000
	}, {  // SCARED
	  B01111110,
	  B10000001,
	  B10000001,
	  B10011001,
	  B10011001,
	  B10000001,
	  B10000001,
	  B01111110
	},
};

enum Sound {
	BLINK_SOUND,
	NEUTRAL_SOUND,
	SCARED_SOUND,
	NEUTRAL_MOVING_SOUND,
	SCARED_MOVING_SOUND,
};

enum Emotion {
	NEUTRAL, 
	SCARED,
};

enum MovementFrequency {
	ALERT, 
	NATURAL, 
};

const int movementDelays[NUM_MOVEMENT_FREQUENCIES] = {2, 5};

const MovementFrequency MOVEMENT_FREQUENCIES[NUM_EMOTIONS] = {
	NATURAL, ALERT,
};

const bool EYEBROW_DIRECTIONS[NUM_EMOTIONS] = {
	// NULL -> neutral, true -> up, false -> down
	NULL, true
};

const String MESSAGES[NUM_EMOTIONS] = {
	"Hello there", "WHOA!"
};

const Sound SOUNDS[NUM_EMOTIONS] = {
	NEUTRAL_SOUND, SCARED_SOUND
};

const Sound MOVING_SOUNDS[NUM_EMOTIONS] ={
	NEUTRAL_MOVING_SOUND, SCARED_MOVING_SOUND
};

// Hardware variables
Adafruit_8x8matrix matrix;
LiquidCrystal_I2C lcd = LiquidCrystal_I2C (0x27, 16, 2);
Servo pan, tilt, leftEyebrow, rightEyebrow;

// Software variables 
float lastBlink, lastMove, lastScared;
int movementDelay = 5; // seconds, don't worry
Emotion emotion = NEUTRAL;
MovementFrequency movementFrequency = NATURAL;

void sendSignal (int value) {
	// 0 is neutral, anything else is valid
	// to avoid 0, add 1 to the value
	analogWrite (SIGNAL, value + 1);
	delay (500);
	analogWrite (SIGNAL, 0);
}

void moveHead() {
	int angle = random (HEAD_MIN_ANGLE, HEAD_MAX_ANGLE);
	pan.write(angle);
	tilt.write(angle);
	sendSignal(MOVING_SOUNDS[emotion]);
}

void setEyebrows(bool direction = NULL);
void setEyebrows(bool direction) {
	if (direction == NULL) direction = EYEBROW_DIRECTIONS[emotion];

	int angle = direction == NULL ? 90
		: direction ? 0 : 180;

	leftEyebrow.write(angle);
	rightEyebrow.write(angle);
}

void drawBitmap(uint8_t bitmap[8] = NULL);  // default argument
void drawBitmap(uint8_t bitmap[8]) {
	/* This function draws a bitmap onto the LED Matrix */

	if (bitmap == NULL) bitmap = BITMAPS[emotion];

	// drapBitmap(int x, int y, int bitmap[8], int width, int height, bool status)
	matrix.drawBitmap (0, 0, bitmap, 8, 8, LED_ON);
	matrix.writeDisplay();  // needed to update matrix
}

void blink() {
	drawBitmap(BITMAPS[0]);
	delay(BLINK_DELAY);
	sendSignal(BLINK_SOUND);
	drawBitmap(BITMAPS[emotion]);
}

void printMessage(String text = NULL);
void printMessage(String text) {
	if (text == NULL) text = MESSAGES[emotion];

	lcd.clear();  // just in case
	lcd.print(text);
	long distance = pulseIn(ECHO, HIGH) * 0.034 / 2;
}

void setEmotion(Emotion newEmotion) {
	emotion = newEmotion;
	movementFrequency = MOVEMENT_FREQUENCIES [emotion];
	drawBitmap();
	setEyebrows();
	printMessage();
	sendSignal(SOUNDS[emotion]);
}

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

void setup() {
	Serial.begin(9600);

	// Servo setup
	pan.attach(PAN);
	tilt.attach(TILT);
	leftEyebrow.attach(LEFT_EYEBROW);
	rightEyebrow.attach(RIGHT_EYEBROW);

	// I2C devices
	matrix.begin(0x70);
	lcd.init();
	lcd.backlight();
	lcd.clear();

	// Proximity sensor
	pinMode (TRIGGER, OUTPUT);
	pinMode (ECHO, INPUT);

	// Slave communication
	pinMode (SIGNAL, OUTPUT);

	// Delay-based actions
	lastBlink = lastMove = lastScared = millis();
}

void loop() {
	int time = millis();
	if (lastBlink - time >= BLINK_INTERVAL) {
		blink();
		lastBlink = time;
	}
	if (lastMove - time >= movementDelay * 1000) {
		moveHead();
		lastMove = time;
	}
	if (isApproaching()) setEmotion(SCARED);
	else if (emotion == SCARED && lastScared - time >= CALM_DOWN_DELAY)
		setEmotion(NEUTRAL);	
}
