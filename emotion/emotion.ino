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
		| True  | False | True  | True  |
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

#define PIN1 A0
#define PIN2 A1
#define PIN3 A2
#define PIN4 A3

enum Emotion {
	BLINK,
	HAPPY, 
	SAD,
	ANGRY,
	LOVE
	// confused
};

Emotion emotion = BLINK;

void setup() {
	pinMode (PIN1, INPUT);
	pinMode (PIN2, INPUT);
	pinMode (PIN3, INPUT);
	pinMode (PIN4, INPUT);
}

void loop() {}

void blink() {}
void happy() {}
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
	if (!pin1 || !pin2) return NULL;
	if (pin3) return pin4 ? HAPPY : SAD;
	else return pin4 ? ANGRY : LOVE;
}

void switchEmotion(Emotion emotion) {
	switch (emotion) {
		case BLINK: return blink();
		case HAPPY: return happy();
		case SAD: return sad();
		case ANGRY: return angry();
		case LOVE: return love();
	}
}