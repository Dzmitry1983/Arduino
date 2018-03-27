/*
 Fade
 
 This example shows how to fade an LED on pin 9
 using the analogWrite() function.
 
 This example code is in the public domain.
 */


int rLed = 3;          // the pin that the LED is attached to
int gLed = 5;          // the pin that the LED is attached to
int bLed = 6;          // the pin that the LED is attached to
int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by

// the setup routine runs once when you press reset:
void setup()  { 
  // declare pin 9 to be an output:
  pinMode(rLed, OUTPUT);
  pinMode(gLed, OUTPUT);
  pinMode(bLed, OUTPUT);
//  Serial.begin(115200);
} 

// the loop routine runs over and over again forever:
void loop()  { 
  // set the brightness of pin 9:
  int r = random(0, 255);
  int g = random(0, 255);
  int b = random(0, 255);
  analogWrite(rLed, brightness);
//  analogWrite(rLed, r);
//  analogWrite(gLed, g);
//  analogWrite(bLed, b);
  //analogWrite(led, brightness);
//  Serial.println(brightness);

  // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade: 
  if (brightness == 0 || brightness == 255) {
    fadeAmount = -fadeAmount ; 
  }     
  // wait for 30 milliseconds to see the dimming effect    
  delay(30);                            
}

