/*
	This is code for the master Arduino. 

	It is in charge of emotion handling, which includes: 
		1. eyebrows
		2. eyes
		3. head/neck movements
		4. proximity sensor (to be surprised)

	So far, the head moves horizontally and vertically by the same random angle.
	If the ultrasonic sensor is triggered, the head moves more frequently, and
	the eyes get wider. After CALM_DOWN_DELAY, these behaviors stop. 

	NOTE: AUDIO IS DISABLED AS IT COULD NOT BE READY IN TIME FOR THE DEMO
	Audio is handled by the slave Arduino, which is equipped with a Wave Shield
	Commands are sent using an analog signal which is used as an index to a list
	More details can be found in the code for the slave Arduino

	Wiring for the master Arduino:
		A3 --> proximity sensor echo (ECHO)
		A2 --> proximity sensor trigger (TRIGGER)
		2 --> pan servo (PAN)
		3 --> tilt servo (TILT)
		4 --> left eyebrow servo (LEFT_EYEBROW)
		5 --> right eyebrow servo (RIGHT_EYEBROW)

	Specs: 
		Storage space: 42% 
		Dynamic memory: 43%
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
#define ECHO A3
#define TRIGGER A2
#define SENSOR_THRESHOLD 7  // cm, I believe

// Blinking
#define BLINK_DELAY 50
#define BLINK_INTERVAL 3000

// Moving the head
#define HEAD_MIN_ANGLE 60
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
	ALERT, NATURAL,
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
unsigned long lastBlink, lastMove, lastScared;
int movementDelay; // seconds
Emotion emotion;
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
	Serial.print("Moving to ");
	Serial.println(angle);
	pan.write(angle);
	tilt.write(angle);
	sendSignal(MOVING_SOUNDS[emotion]);
}

void setEyebrows(bool direction = NULL);
void setEyebrows(bool direction) {
	if (direction == NULL) direction = EYEBROW_DIRECTIONS[emotion];

	int angle = direction == NULL ? 90
		: direction ? 0 : 160;

	Serial.print("Moving eyebrows ");
	Serial.println(angle);

	int rightEyebrowAngle = abs (180 - angle);
	leftEyebrow.write(angle);
	rightEyebrow.write(rightEyebrowAngle);
}

void drawBitmap(uint8_t bitmap[8] = NULL);  // default argument
void drawBitmap(uint8_t bitmap[8]) {
	/* This function draws a bitmap onto the LED Matrix */

	if (bitmap == NULL) bitmap = BITMAPS[emotion];

	// drapBitmap(int x, int y, int bitmap[8], int width, int height, bool status)
	matrix.clear();
	matrix.drawBitmap (0, 0, bitmap, 8, 8, LED_ON);
	matrix.writeDisplay();  // needed to update matrix
}

void blink() {
	Serial.println ("Blinking");
	drawBitmap(BLINK);
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
	Serial.print("Changing emotion to ");
	Serial.println(newEmotion);
	emotion = newEmotion;
	movementFrequency = MOVEMENT_FREQUENCIES [emotion];
	drawBitmap();
	setEyebrows();
	printMessage();
	sendSignal(SOUNDS[emotion]);
}

void setMovementFrequency(MovementFrequency frequency) {
	Serial.print ("Changing movement frequency to ");
	Serial.println(frequency);
	movementFrequency = frequency;
	movementDelay = MOVEMENT_FREQUENCIES[frequency];
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
	Serial.println ("Setup...");

	// Servo setup
	pan.attach(PAN);
	tilt.attach(TILT);
	leftEyebrow.attach(LEFT_EYEBROW);
	rightEyebrow.attach(RIGHT_EYEBROW);

	// I2C devices
	Serial.println("Initializing matrices");
	matrix.begin(0x70);
	Serial.println("Matrices initialized");
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

	// Set initial state
	setEmotion(NEUTRAL);
	setMovementFrequency(NATURAL);
}

void loop() {
	Serial.println("Processing");
	int time = millis();
	Serial.print("Timestamp: ");
	Serial.println(time);
	if (time - lastBlink >= BLINK_INTERVAL) {
		Serial.println("Blink time");
		blink();
		lastBlink = time;
	}
	if (time - lastMove >= movementDelay * 1000) {
		Serial.println("Time to move the head");
		moveHead();
		lastMove = time;
	}
	if (isApproaching()) {
		Serial.println("Something's here");
		if (emotion != SCARED) setEmotion(SCARED);
	}
	else if (emotion == SCARED && lastScared - time >= CALM_DOWN_DELAY) {
		Serial.println("Everything's OK");
		setEmotion(NEUTRAL);
	}
}
