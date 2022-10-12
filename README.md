# PES_Assignment_4

**Developing Finite State Machines for a new Traffic Light System in Buffahiti 
Test Blinking Pattern and Touch Slider


**Author: Harsh Beriwal
IDE Used: MCUXpresso IDE v11.6.0 [Build 8187] [2022-07-13]
Github Link: https://github.com/harshberiwal/PES_Assignment_4
No leveraged code or files****

test.c includes the int main(); 
test.h includes all the libraries needed for test.c to work

touch.h includes all the declarations of functions related to Touch.
This include Touch_Init() and Touch_Poll()
touch.c includes all the definitions of functions declared in Touch.h
This include Touch_Init() and Touch_Poll() and its IRQHandler.

system.h includes all the declaration of initialzing the SysTick
This include initialize_Systick(), get_timer(), reset_timer()
and now() functions.
system.c includes all the definition of initialzing the SysTick
This include initialize_Systick(), get_timer(), reset_timer()
and the Systick Handler which replaces the Weak SystickIRQhandler
definition in the Startup file.

state_machine.c includes all the definitions of functions declared in
state_machine.h.
We used A Table Driven Approach for the State Machine which made the State_check
code concise and easy to follow.

pwm.h includes all the declarations given for TPM.
This include Init_TPM() and initializes all the peripherals used
for transitions and PWM in the State_Machine.

gpio.h includes all the declarations given for GPIO.
This include gpio_init() and the button_check function for GPIO external interrupt.
gpio.c includes all the definitions of functions in gpio.h
This include gpio_init() function for the Button Pressed.

Below are all the Functions Used

Initializes pins as GPIO input with interrupt enabled
Port A Pin 13 is used.
Parameters:
  None

Returns:
  None
void gpio_init();

Scans the capacitive Touch Slider and returns value corresponding to the touch strength and area.
Parameters:
None
Returns:
Scanned Value between 0 to 65535 
int Touch_Poll(void);


Initializes the capacitive Touch Slider and the GENCS and DATA register to get the Touch Value
Parameters:
None
Returns:
None 
void Touch_Init();

/*
 * Initializes SysTick and its registers like VAL, LOAD for precise delays
 *
 * Parameters:
 *   None
 *
 * Returns:
 *   None
 */
void initialize_SysTick();

/*
 * Returns the time code is running since boot.
 *
 * Parameters:
 *   None
 *
 * Returns:
 *   32-bit time its running since startup
 */
ticktime_t now();


/*
 * Resets the timer VAL register and timeout_timer for the next callback.
 *
 * Parameters:
 *   None
 *
 * Returns:
 *   None
 */
void reset_timer();


/*
 * Function to get the Timer Value from the current Systick Tick.
 *
 * Parameters:
 *   None
 *
 * Returns:
 *   32-bit time since the reset of the timer after state change
*/
ticktime_t get_timer();

/*
 * Table driven State machine for checking events based on Timeout or Button Pressed
 * and changing the states accordingly. This functions also polls the Touch Slider and
 * Prints debug messages during the DEBUG cycle.
 *
 * Parameters:
 *   THE TIME CURRENT STATE -GET_TIMER();
 *
 * Returns:
 *   None
 */
void state_check(uint16_t timeout_time_elapsed);


/*
 * When the Timeout occurs from GO, this function is called from the state machine using
 * a function pointer. The STOP transition takes place from the GO state and then it
 * stays in the STOP mode for 20 sec or 5 sec depending on the Production or DEBUg code
 * Respectively.
 *
 * It calls the Transition function given below.
 *
 * Parameters:
 *   TAKES THE LAST STATE (GO) and its transition colors
 *
 * Returns:
 *   None
 */
void Signal_Red_on(int last_st, uint32_t last_col);

/*
 * When the Timeout occurs from CROSSWALK OR STOP, this function is called from the
 * state machine using a function pointer. The GO transition takes place from the
 * CROSSWALK OR STOP state and then it stays in the GO mode for 20 sec or 5 sec depending
 * on the Production or DEBUg code Respectively.
 *
 * It calls the Transition function given below.
 *
 * Parameters:
 *   TAKES THE LAST STATE (CROSSWALK OR STOP) and its corresponding transition colors.
 *
 * Returns:
 *   None
 */
void Signal_Green_on(int last_st, uint32_t last_col);

/*
 * When the Timeout occurs from GO, this function is called from the
 * state machine using a function pointer. The WARNING transition takes place from the
 * GO state and then it stays in the GO mode for 5 sec or 3 sec depending
 * on the Production or DEBUg code Respectively.
 *
 * It calls the Transition function given below.
 *
 * Parameters:
 *   TAKES THE LAST STATE (GO) and its corresponding transition colors.
 *
 * Returns:
 *   None
 */
void Signal_Yellow_on(int last_st, uint32_t last_col);

/*
 * When the Button is pressed, this function callback is generated from the
 * state machine using a function pointer. The CROSSWALK transition takes place from
 * any state and then it stays in the CROSSWALK mode for 10 sec.
 *
 * AFTER THE SMOOTH TRANSITION FOR THE BUFFAHITI PEOPLE, THE LED FOLLOWS a 250 msec off
 * and 750 msec on for 10 seconds.
 *
 * Parameters:
 *   TAKES THE LAST STATE (GO,STOP,WARNING) and its corresponding transition colors.
 *
 * Returns:
 *   None
 */
void Signal_Blue_on(int last_st, uint32_t last_col);

/*
 * This functions calculates the curr transition value on the basis of the formula
 * which includes the below parameters and returns the next transition color for that
 * particular LED.
 *
 * Parameters:
 *   FOUR
 *   1)THE COLOR OF STATE TO TRANSITION TO
 *   2)THE COLOR OF STATE TO TRANSITION FROM
 *   3)THE LED VALUE WE NEED (WHETHER RED GREEN OR BLUE)
 *   4)CORRESPONDING SHIFT AS PER THE LED TYPE
 *
 * Returns:
 *   The Required Transition color for each LED.
 */
int get_curr_val(uint32_t req_color, uint32_t last_color, uint32_t LED_Type, uint8_t shift);


/*
 * Takes the Current transition value of Red Green and Blue LED and returns
 * the updated Color if the button is pressed during the transition.
 *
 * Parameters:
 * 	 None
 *
 * Returns:
 *   The last transition color for RED GREEN And BLUE.
 */
uint32_t update_last_val();


/*
 * HANDLES THE TRANSITION BY PUTTING THE VALUES RECEIVED FROM get_curr_val
 * FUNCTION to the corresponding TPM channel for smooth Transitions.
 *
 * Parameters:
 * 	 1)CURRENT COLOR TO TRANSITION TO
 * 	 2)LAST COLOR TO TRANSTION FROM
 *
 * Returns:
 *   None
 */
void transitions(uint32_t Curr_Color, uint32_t last_color);

/*
 * Initializes pins as TPM output with desired clock frequency
 * Clock working at 750kHz.
 * Parameters:
 *   THE PERIOD IN THE MOD REGISTER
 *
 * Returns:
 *   None
 */
void Init_TPM(uint16_t period);


/*
 * Checks in the Button has been pressed.
 *
 * Parameters:
 *   None
 *
 * Returns:
 *   True if the button is pressed or false if it isn't.
 */
bool button_check();


##EXTRA CREDIT##

##PEER REVIEW##

Partner's Name: Shashank Chandrasekaran

Data and time of Review: 11th October, 11 pm. 

##Comments Received:##

Plus points:
1) Macros used everywhere
2) state_machine.c is very optimized with minimal redundancy

Suggestions:
1) Keep the #define values in brackets as per ESE guideline (eg: #define RED (18)) and in captital
2) gpio.h not used anywhere, if it is not going to be used afterwards,it can be removed.
3) led_sequence.c and led_sequence.h are blank files, kindly delete them.
4) In log.h, comments can be added for the calculation of stop,go and warning time
5) In pwm.c, the gpio_init() funtion,configuring pins as output and turning them low is not required.
   This can be omitted and GPIO function can be clubbed with TPM function.
6) test.c and test.h can be renamed to indicate the functionality.
7) Overall comments can be increased in code to give more clarity. Eg: Comment for #defines,functions
   in .h files can be added or elaborated.

##Following Changes were made post the PEER_REVIEW(SUMMARY)## 

1) Changed some #define that were required to be change. If the #define contains plain number, kept it the same. 

2) Gpio.h is used for GPIO Input External Interrupt. it has the function needed for GPIO button press check. 

3) Removed those files.

4) Good Point. Added the comments

5) Gpio_init was used for Gpio input button press interrupt. 
 
6) Not changed. 

7) Comments were increased to improve clarity to the reader. 

##EXTRA CREDIT##

THE GPIO_INPUT INTERRUPT WAS CONFIGURED AT PORT A PIN13 and was checked to be working properly. 
This was checked with a normal breadboard ciruit with a resistor to ground. 

