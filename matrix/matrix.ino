#include <Adafruit_LEDBackpack.h>
#include <Adafruit_GFX.h>

#define SIGNAL 4

bool processing = false;
Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

// SCL to A5
// SDA to A4

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

bool gotSignal() {
	int signal = digitalRead(SIGNAL);
	return signal == HIGH;
}

void setup() {
	Serial.begin(9600);
	matrix.begin(0x70);
	pinMode (SIGNAL, INPUT);
	pinMode (13, OUTPUT);
}

void loop() {
	Serial.print ("Processing? ");
	Serial.print (processing);
	Serial.println (". Hotel? Trivago.");
	if (processing) {  // busy
		if (!gotSignal()) {  // shouldn't be busy anymore
			processing = false;
			// clearMatrix();
			matrix.clear();
			Serial.println("Clearing matrix");
			matrix.writeDisplay();
			digitalWrite(13, LOW);
		}
	} else if (gotSignal()) {  // not busy but should be
		processing = true;
		setMatrix();
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