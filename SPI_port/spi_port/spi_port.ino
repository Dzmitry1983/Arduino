/*
Example for test a com port by PC and transfer data between PC and arduino
*/
#include <Arduino.h>
#include <TM1637Display.h>
#include <SPI.h>

//defines
#define CLK_PIN 2
#define DIO_PIN 3
#define LED_PIN 13
#define SPEED_BITS_PER_SECOND 115200


TM1637Display display(CLK_PIN, DIO_PIN);

String inString = "";	// string to hold input
volatile int data = 0;
volatile int countInterrupts = 0;

// SPI interrupt routine
ISR (SPI_STC_vect)
{
	byte c = SPDR;  // grab byte from SPI Data Register
	data = c;
	countInterrupts++;
}  //

// the setup routine runs once when you press reset:
void setup() {				
	Serial.begin(SPEED_BITS_PER_SECOND);
	Serial.println("DHT TEST PROGRAM ");
	Serial.println("LIBRARY VERSION: ");
	// initialize the digital pin as an output.
	pinMode(LED_PIN, OUTPUT);

	display.setBrightness(0x0f);

	SPI.begin();
	SPI.setBitOrder(LSBFIRST);
	SPI.setDataMode (SPI_MODE0);
//	SPI.usingInterrupt(17);
 	SPI.attachInterrupt();
  	
}

// the loop routine runs over and over again forever:
void loop() {

	static int number = 0;
	static uint8_t readedNumber = 0;
	static int readCount = 0;
	uint8_t sendNumber = 0;
	bool isNeedToSend = 0;
	
	
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
		sendNumber = inString.toInt();
			// clear the string for new input:
			inString = "";
			isNeedToSend = true;
	}
	
	if (isNeedToSend) {
		SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE1));
		SPI.transfer(sendNumber);
		SPI.endTransaction();
		SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE1));
		Serial.print("Send = ");
		unsigned int t = SPI.transfer(0);
		Serial.print(t, BIN);
		Serial.print("(");
		Serial.print(t);
		Serial.print(") = ");
		
		Serial.println(sendNumber, BIN);

//		for (int i = 0; i < 10; i++) {
//			Serial.println(SPDR);
//			Serial.println(SPSR);
//			Serial.println(SPIF);
//			SPDR = 0;
//		}
		SPI.endTransaction();

		
		
	}

		SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE1));
		unsigned int t = SPI.transfer(0);
		Serial.print(t, BIN);
		Serial.print("(");
		Serial.print(t);
		Serial.println(") = ");
		
		SPI.endTransaction();

	
//	while (readedNumber = SPI.transfer(0x00)) {
//		Serial.print("Recived ");
//		Serial.print(readCount);
//		Serial.print(" - ");
//		Serial.println(readedNumber, HEX);
//	}
	
	readedNumber = data;
	display.showNumberDec(readedNumber);

}
