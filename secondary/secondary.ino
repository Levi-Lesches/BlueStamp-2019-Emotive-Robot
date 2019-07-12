/* 
	Wiring diagram for the slave Arduino (not with the Wave Shield): 
	Arduino1 = Wave Shield (master), Arduino2 = other one (slave)
		Master GND to slave GND
		Master Power (3.3 is fine) to slave VIN 
		Master A0 to slave A0 (SIGNAL)
		I2C SCL to slave A5
		I2C SDA to slave A4

	Specs: 
		Storage space: 24%
		Dynamic memory: 16%
*/ 

#include <Adafruit_LEDBackpack.h>
#include <LiquidCrystal_I2C.h>

#define SIGNAL A0
#define BUILTIN 13

// There is a nice website for editing LED matrix bitmaps
// My collection is stored at this permalink
// https://xantorohara.github.io/led-matrix-editor/#003c425a5a423c00|7e8181999981817e|003c420000000000|003c435a5c483000|003cc25a3a120c00|007e425a5a241800|1824429999819966
const uint8_t smile[8] = {
  B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10100101,
  B10011001,
  B01000010,
  B00111100
};

Adafruit_8x8matrix matrix = Adafruit_8x8matrix();
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);
// This is to keep track of when the signal comes and goes
bool processing = false;

bool gotSignal() {
	// Reads the SIGNAL pin
	int signal = digitalRead(SIGNAL);
	return signal == HIGH;
}

void drawBitmap(uint8_t bitmap[8]) {
	/* This function draws a bitmap onto the LED Matrix */

	// drapBitmap(int x, int y, int bitmap[8], int width, int height, bool status)
	matrix.drawBitmap (0, 0, bitmap, 8, 8, LED_ON);
	matrix.writeDisplay();  // needed to update matrix
}

void clearMatrix() {
	matrix.clear();
	matrix.writeDisplay();  // refresh matrix
}

void activate() {
	drawBitmap(smile);
	lcd.print ("Danger!");
	digitalWrite (BUILTIN, HIGH);
}

void clear() {
	// Resets the matrix, LCD, and BUILTIN 
	clearMatrix();
	lcd.clear();
	digitalWrite(BUILTIN, LOW);
}


void setup() {
	// Basic setup
	matrix.begin(0x70);
	lcd.init();
	lcd.backlight();
	lcd.clear();
	pinMode (SIGNAL, INPUT);
	pinMode (BUILTIN, OUTPUT);
}

void loop() {
	/* 
		Here, we want to be sure not to act unless the signal changes
		Control flow is basically a truth table: 
			+-------+-------+
			|  Busy | Signal|
			+-------+-------+
			| True  | True  | ==> nothing
			+-------+-------+
			| True  | False | ==> clear devices
			+-------+-------+
			| False | True  | ==> Start devices
			+-------+-------+
			| False | False | ==> nothing
			+-------+-------+
	*/
	if (processing) {  // busy
		if (!gotSignal()) {  // shouldn't be busy anymore
			processing = false;
			clear();
		}
	} else if (gotSignal()) {  // not busy but should be
		processing = true;
		activate();
	}
	delay (100);  // IDK, just be a bit efficient
}

// some other matrix methods include: 
// .clear()
// .writeDisplay()
// .drawBitmap(x, y, uint8_t bitmap, width, height, LED_ON)
// .drawPixel (x, y, LED_ON/LED_OFF)
// .drawLine (x1, y1, x2, y2, LED_ON)
// .drawRect (x1, y1, x2, y2, LED_ON)
// .fillRect (x1, y1, x2, y2, LED_ON)
// .drawCircle (x, y, radius, LED_ON)
// .setTextSize (1)
// .setTextWrap (bool)
// .setTextColor(LED_ON)
// .setCursor (x, y)
// .setRotation (1-4)