/*
Example for test a com port by PC and transfer data between PC and arduino
*/
#include <Arduino.h>
#include <TM1637Display.h>

//defines
#define CLK_PIN 2
#define DIO_PIN 3
#define LED_PIN 13
#define BUTTON_PIN 8
#define SPEED_BITS_PER_SECOND 115200


TM1637Display display(CLK_PIN, DIO_PIN);

String inString = "";	// string to hold input

// the setup routine runs once when you press reset:
void setup() {				
	Serial.begin(SPEED_BITS_PER_SECOND);
	Serial.println("DHT TEST PROGRAM ");
	Serial.println("LIBRARY VERSION: ");
	// initialize the digital pin as an output.
	pinMode(LED_PIN, OUTPUT);
	pinMode(BUTTON_PIN, INPUT);

	display.setBrightness(0x0f);
}

// the loop routine runs over and over again forever:
void loop() {
	static int lastButtonState = 0;
	int buttonValue = digitalRead(BUTTON_PIN);
	static int number = 0;
	static int readedNumber = 0;
	digitalWrite(LED_PIN, buttonValue);	 // turn the LED on (HIGH is the voltage level)

	if (lastButtonState != buttonValue) {
		lastButtonState = buttonValue;
		number ++;
		Serial.println(number);
	}


	
	
	// Read serial input:
	while (Serial.available() > 0) {
		int inChar = Serial.read();
		
		if (isDigit(inChar)) {
			// convert the incoming byte to a char and add it to the string:
			inString += (char)inChar;
		}
		// if you get a newline, print the string, then the string's value:
	}

	if (inString.length() > 0) {
		readedNumber = inString.toInt();
			// clear the string for new input:
			inString = "";
	}

	display.showNumberDec(readedNumber);
	delay(100);
}
