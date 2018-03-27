/*********************************************************************
This is humidity and temperature sensors station

*********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <dht.h>


#define DHT11_PIN 2
#define OLED_RESET 4

Adafruit_SSD1306 display(OLED_RESET);
dht DHT_11;


#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup()   {                
  Serial.begin(115200);
  Serial.println("DHT TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT_LIB_VERSION);
  Serial.println();
  Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done
  
  // Clear the display buffer.
  display.clearDisplay();
  display.display();
}


void loop() {
  // READ DATA
  Serial.print("DHT11, \t");
  int chk = DHT_11.read11(DHT11_PIN);
  switch (chk)
  {
    case DHTLIB_OK:  
	//	printError("OK,\t"); 
                printData(DHT_11);
		break;
    case DHTLIB_ERROR_CHECKSUM: 
		printError("Checksum error,\t"); 
		break;
    case DHTLIB_ERROR_TIMEOUT: 
		printError("Time out error,\t"); 
		break;
    case DHTLIB_ERROR_CONNECT:
        printError("Connect error,\t");
        break;
    case DHTLIB_ERROR_ACK_L:
        printError("Ack Low error,\t");
        break;
    case DHTLIB_ERROR_ACK_H:
        printError("Ack High error,\t");
        break;
    default: 
		printError("Unknown error,\t"); 
		break;
  }
  
  // DISPLAY DATA
  

  delay(2000);
}

void printError(char *error) {
  Serial.print(error); 
  display.clearDisplay();
  // text display tests
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.setTextSize(2);
  display.print(error);
  display.display();
}

void printData(dht &DHT) {
  Serial.print(DHT.humidity, 1);
  Serial.print(",\t");
  Serial.println(DHT.temperature, 1);
  // text display tests
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("T = ");
  display.print(DHT.temperature, 0);
  display.println("C");
  display.print("H = ");
  display.print(DHT.humidity, 0);
  display.println("%");
  display.display();
  
}

