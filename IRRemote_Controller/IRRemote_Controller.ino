/*
 * This skath is used to controll 4 chanels, where 2 chanels are PWM
 * Interupts are used
 * 
 * 2 and 4 pins need to be linked
 */

#include <IRremote.h>
#include <avr/sleep.h>

#define DEBUG_MODE 1

/*
 * enums
 * They code buttons codes
 */

enum EnumButtonType 
{ 
	EnumButtonTypeOk = 0xFF02FD,
	EnumButtonTypeOk_sleep = 0xFF02F,
	EnumButtonType1 = 0xFF6897,
	EnumButtonType1_sleep = 0xFF689,
	EnumButtonType2 = 0xFF9867,
	EnumButtonType2_sleep = 0xFF986,
	EnumButtonType3 = 0xFFB04F,
	EnumButtonType3_sleep = 0xFFB04,
	EnumButtonType4 = 0xFF30CF,
	EnumButtonType4_sleep = 0xFF30C,	
	
	EnumButtonTypeLevelUp = 0xFF629D,
	EnumButtonTypeLevelUp_sleep = 0xFF629, 
	EnumButtonTypeLevelDown = 0xFFA857,
	EnumButtonTypeLevelDown_sleep = 0xFFA85,
	EnumButtonTypeLevelUpBig = 0xFFC23D,
	EnumButtonTypeLevelUpBig_sleep = 0xFFC23, 
	EnumButtonTypeLevelDownBig = 0xFF22DD,
	EnumButtonTypeLevelDownBig_sleep = 0xFF22D,
	
	EnumButtonTypeLevelRepeat = 0xFFFFFFFF,
	EnumButtonTypeNone
} unsigned long; 


/*
 * sleep test code
 */

int wakePin = 2;                 // pin used for waking up

/*
 * constatns
 */

const int RECV_PIN = 4;
const int PINS_COUNT = 4;
const int OUTPUT_PINS[PINS_COUNT] = {3, 5, 6, 9};
const int OUTPUT_PMW_PINS[] = {5,6};

const int MAX_PMW = 255;
const int MIN_PMW = 0;
const int SMALL_PMW_STEP = 10;
const int BIG_PMW_STEP = 255;
const bool DEFAULT_PORT_VALUE = false;


const int delay_time = 50;
const int loops_count_to_sleep = 10;

/*
 * variables
 */

bool is_on_ports[PINS_COUNT] = {DEFAULT_PORT_VALUE, DEFAULT_PORT_VALUE, DEFAULT_PORT_VALUE, DEFAULT_PORT_VALUE};

bool is_same_thing_changed = false;
int pwm_value = 125;
int current_loops_count_to_sleep = 0;
bool is_sleep_enable = true;



enum EnumButtonType previusButtonType  = EnumButtonTypeNone;



IRrecv irrecv(RECV_PIN);
decode_results results;

/*
 * code section
 */

 void wakeUpNow()        // here the interrupt is handled after wakeup
{
  // execute code here after wake-up before returning to the loop() function
  // timers and code using timers (serial.print and more...) will not work here.
  // we don't really need to execute any special functions here, since we
  // just want the thing to wake up
}

void setup()
{

	#ifdef DEBUG_MODE 
		Serial.begin(9600);
	#endif
	
	// initialize digital pin LED_BUILTIN as an output.
  	pinMode(LED_BUILTIN, OUTPUT);
  	for (int i = 0; i < PINS_COUNT; i++) {
  		int pin_number = OUTPUT_PINS[i];
  		bool is_on = is_on_ports[i];
		pinMode(pin_number, OUTPUT);
		digitalWrite(pin_number, is_on);
		//analogWrite(pin_number, pwm_value);
  	}

	irrecv.enableIRIn(); // Start the receiver
}

void loop()
{
	bool signal_led_value = false;
	bool is_ok_pressed = false;
	if (irrecv.decode(&results))
	{
		#ifdef DEBUG_MODE 
			Serial.println(results.value, HEX);
		#endif
		
		switch (results.value) 
		{
			case EnumButtonTypeOk:
			case EnumButtonTypeOk_sleep:
				is_ok_pressed = true;
				is_same_thing_changed = true;
			break;
			case EnumButtonType1:
			case EnumButtonType1_sleep:
				previusButtonType = results.value;
				is_on_ports[0] = !is_on_ports[0];
				is_same_thing_changed = true;
			break;
			case EnumButtonType2:
			case EnumButtonType2_sleep:
				previusButtonType = results.value;
				is_on_ports[1] = !is_on_ports[1];
				is_same_thing_changed = true;
			break;
			case EnumButtonType3:
			case EnumButtonType3_sleep:
				previusButtonType = results.value;
				is_on_ports[2] = !is_on_ports[2];
				is_same_thing_changed = true;
			break;
			case EnumButtonType4:
			case EnumButtonType4_sleep:
				previusButtonType = results.value;
				is_on_ports[3] = !is_on_ports[3];
				is_same_thing_changed = true;
			break;
			case EnumButtonTypeLevelUp:
			case EnumButtonTypeLevelDown:
			case EnumButtonTypeLevelUpBig:
			case EnumButtonTypeLevelDownBig:
			case EnumButtonTypeLevelUp_sleep:
			case EnumButtonTypeLevelDown_sleep:
			case EnumButtonTypeLevelUpBig_sleep:
			case EnumButtonTypeLevelDownBig_sleep:
				previusButtonType = results.value;
				is_same_thing_changed = updatePMW(previusButtonType, pwm_value);
			break;
			case EnumButtonTypeLevelRepeat:
				is_same_thing_changed = updatePMW(previusButtonType, pwm_value);
			break;
			default:
				previusButtonType = EnumButtonTypeNone;
			break;
		}
		irrecv.resume(); // Receive the next value
	}
	if (is_same_thing_changed) {
		is_same_thing_changed = false;
		current_loops_count_to_sleep = 0;
		
		signal_led_value = true;
		digitalWrite(LED_BUILTIN, signal_led_value);

		if (is_ok_pressed) {
			bool is_on = true;
			for (int i = 0; i < PINS_COUNT; i++) {
				if (is_on_ports[i]) {
					is_on = false;
					break;
				}
			}
			for (int i = 0; i < PINS_COUNT; i++) {
				is_on_ports[i] = is_on;
			}
		}

		for (int i = 0; i < PINS_COUNT; i++) {
			bool is_on = is_on_ports[i];
			int pin_number = OUTPUT_PINS[i];
			if (is_on) {
				bool is_analog = false;
				int analog_count = sizeof(OUTPUT_PMW_PINS) / sizeof(OUTPUT_PMW_PINS[0]);
				for (int a = 0; a < analog_count; a++) {
					if (OUTPUT_PMW_PINS[a] == pin_number) {
						is_analog = true;
						break;
					}
				}
				
				if (is_analog) {
					analogWrite(pin_number, pwm_value);
				}
				else {
					digitalWrite(pin_number, is_on);
				}
				
			}
			else {
				digitalWrite(pin_number, is_on);
			}
  		}
	}

	bool is_on = false;
	for (int i = 0; i < PINS_COUNT; i++) {
		if (is_on_ports[i]) {
			is_on = true;
			break;
		}
	}
	
	is_sleep_enable = !is_on;

	if (current_loops_count_to_sleep < loops_count_to_sleep || !is_sleep_enable) {
		delay(delay_time);
		if (signal_led_value) {
			signal_led_value = false;
			digitalWrite(LED_BUILTIN, signal_led_value);
		}
		current_loops_count_to_sleep++;
	}
	else {
		if (signal_led_value) {
			signal_led_value = false;
			digitalWrite(LED_BUILTIN, signal_led_value);
		}
		current_loops_count_to_sleep = 0;
		sleepNow();
	}
}

/*
 * Function wich change pwm level if sound_up or sound_down were clicked
 */
bool updatePMW(EnumButtonType buttonType, int &pwm_value) 
{
	bool returnValue = true;
	switch (buttonType) {
		case EnumButtonTypeLevelUp:
			pwm_value = min(pwm_value + SMALL_PMW_STEP, MAX_PMW);
		break;
		case EnumButtonTypeLevelDown:
			pwm_value = max(pwm_value - SMALL_PMW_STEP, MIN_PMW);
		break;
		case EnumButtonTypeLevelUpBig:
			pwm_value = min(pwm_value + BIG_PMW_STEP, MAX_PMW);
		break;
		case EnumButtonTypeLevelDownBig:
			pwm_value = max(pwm_value - BIG_PMW_STEP, MIN_PMW);
		break;
		default:
			returnValue = false;
		break;
	}
	
	return returnValue;
}






void sleepNow()         // here we put the arduino to sleep
{
    /* Now is the time to set the sleep mode. In the Atmega8 datasheet
     * http://www.atmel.com/dyn/resources/prod_documents/doc2486.pdf on page 35
     * there is a list of sleep modes which explains which clocks and
     * wake up sources are available in which sleep mode.
     *
     * In the avr/sleep.h file, the call names of these sleep modes are to be found:
     *
     * The 5 different modes are:
     *     SLEEP_MODE_IDLE         -the least power savings
     *     SLEEP_MODE_ADC
     *     SLEEP_MODE_PWR_SAVE
     *     SLEEP_MODE_STANDBY
     *     SLEEP_MODE_PWR_DOWN     -the most power savings
     *
     * For now, we want as much power savings as possible, so we
     * choose the according
     * sleep mode: SLEEP_MODE_PWR_DOWN
     *
     */  
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here
 
    sleep_enable();          // enables the sleep bit in the mcucr register
                             // so sleep is possible. just a safety pin
 
    /* Now it is time to enable an interrupt. We do it here so an
     * accidentally pushed interrupt button doesn't interrupt
     * our running program. if you want to be able to run
     * interrupt code besides the sleep function, place it in
     * setup() for example.
     *
     * In the function call attachInterrupt(A, B, C)
     * A   can be either 0 or 1 for interrupts on pin 2 or 3.  
     *
     * B   Name of a function you want to execute at interrupt for A.
     *
     * C   Trigger mode of the interrupt pin. can be:
     *             LOW        a low level triggers
     *             CHANGE     a change in level triggers
     *             RISING     a rising edge of a level triggers
     *             FALLING    a falling edge of a level triggers
     *
     * In all but the IDLE sleep modes only LOW can be used.
     */
 	pinMode (wakePin, INPUT_PULLUP);
    attachInterrupt(0,wakeUpNow, HIGH); // use interrupt 0 (pin 2) and run function
                                       // wakeUpNow when pin 2 gets LOW
 
    sleep_mode();            // here the device is actually put to sleep!!
                             // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP
 
    sleep_disable();         // first thing after waking from sleep:
                             // disable sleep...
    detachInterrupt(0);      // disables interrupt 0 on pin 2 so the
                             // wakeUpNow code will not be executed
                             // during normal running time.
 
}

