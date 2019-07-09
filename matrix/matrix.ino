#include <Adafruit_LEDBackpack.h>
#include <Adafruit_GFX.h>

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
	matrix.drawBitmap (0, 0, bitmap, 8, 8, LED_ON);
	matrix.writeDisplay();
}

void setup() {
	matrix.begin(0x70);
}

void loop() {
	matrix.clear();
	matrix.writeDisplay();
	delay (500);
	drawBitmap(smile);
	delay (500);
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