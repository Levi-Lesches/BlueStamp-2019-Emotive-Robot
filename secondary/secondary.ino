// SCL to A5
// SDA to A4

#include <Adafruit_LEDBackpack.h>
#include <LiquidCrystal_I2C.h>

#define SIGNAL 4

bool processing = false;
Adafruit_8x8matrix matrix = Adafruit_8x8matrix();
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

// https://xantorohara.github.io/led-matrix-editor/#3c4299a581a5423c
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

void drawBitmap(uint8_t bitmap[8]) {
	Serial.println("Showing smiley face");
	matrix.drawBitmap (0, 0, bitmap, 8, 8, LED_ON);
	matrix.writeDisplay();
}

void setMatrix() {
	drawBitmap(smile);
	digitalWrite (13, HIGH);
}

void clearMatrix() {
	matrix.clear();
	matrix.writeDisplay();
	digitalWrite(13, LOW);
	Serial.println("Clearing matrix");
}

void setLCD() {
	lcd.print ("Danger!");
}

void clearLCD() {
	lcd.clear();
}

bool gotSignal() {
	int signal = digitalRead(SIGNAL);
	return signal == HIGH;
}

void setup() {
	Serial.begin(9600);
	matrix.begin(0x70);
	lcd.init();
	lcd.backlight();
	lcd.clear();
	pinMode (SIGNAL, INPUT);
	pinMode (13, OUTPUT);
}

void loop() {
	if (processing) {  // busy
		if (!gotSignal()) {  // shouldn't be busy anymore
			Serial.println ("Clearing up");
			processing = false;
			clearMatrix();
			clearLCD();
		}
	} else if (gotSignal()) {  // not busy but should be
		Serial.println ("Setting up");
		processing = true;
		setMatrix();
		setLCD();
	}
	delay (10);
}

// some other methods include: 
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