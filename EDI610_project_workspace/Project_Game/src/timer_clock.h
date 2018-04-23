#ifndef ALARM_CLOCK_H_
#define ALARM_CLOCK_H_
/*
 *  Description: Initializes the two timers, such that
 *  	(1)  TIMER1 generates periodic interrupts at a rate once in every 10ms
 *  	(2)  TIMER2 generates a single interrupt 5s after this timer is enabled
 *  Hints:
 *  	(1) Write the corresponding value in the LOAD register for each the timers
 *  	(2) Write the corresponding value in the CONTROL/STATUS register for each of the timers
 *  		- Set the counting direction
 *  		- Set the operation (reload or hold upon overflow)
 *  		- Acknowledge the interrupts
 *  		- Enable the interrupts
 *  		- Load the value of the LOAD register into the COUNTER register
 */
void initTimers_alarmClock();


/*
 *  Description: Initializes the interrupt controller, such that:
 *  	(1) The interrupts from TIMER1 and TIMER2 are enabled
 *  	(2) "timer1AlarmClockInterruptHandler()" is the interrupt handler for the interrupts generated by TIMER1
 *  	(3) "timer2AlarmClockInterruptHandler()" is the interrupt handler for the interrupts generated by TIMER2 should be the routine
 *  	(4) The interrupt controller is able to capture interrupt requests from the timers, and it is able to signal interrupts to the MicroBlaze processor
 *  Hints:
 *  	Interrupts from TIMER1 are mapped to bit position 1 in the ISR of the controller, while interrupts from TIMER2 are mapped to bit position 2. The least significant bit in the ISR is at bit position 0.
 *  	Enabling of interrupts is done by writing to the IER.
 *  	The address of the interrupt handler for TIMER1 should be written in IVAR1
 *  	The address of the interrupt handler for TIMER2 should be written in IVAR2
 *  	The MER register should be modified such that interrupt requests are captured and enable the controller to generate interrupts
 *
 */
void initController_alarmClock();


/*
 *  Description: Enables TIMER1
 *  Hints:
 *  	Write the corresponding value in the CONTROL/STATUS register of TIMER1
 */
void enableTimer1();


/*
 *  Description: Enables TIMER2
 *  Hints:
 *  	Write the corresponding values in the CONTROL/STATUS register of TIMER2
 *  	Since the counter of this timer halts upon an overflow, don't forget to load the value of the LOAD register into the COUNTER register, before the timer is enabled
 *
 */
void enableTimer2();


/*
 *  Description: Displays the time on the eight seven-segment displays.
 *               The time is provided as an array, where the element at index 0 represents the hours, while the element at index 3 represents the hundreds of a second.
 *               Each field should be presented with two digits.
 *               Hours are presented on the two leftmost 7-segment displays, followed by minutes, followed by seconds, and finally hundreds of a second presented on the two rightmost 7-segment displays.
 *               In case a field has only one digit, the field is presented with a leading 0. For example, if  time_array[0]==3 the hours are presented as "03" on the two leftmost displays.
 *  Hints:  You can reuse the function displayNumberAtIndex().
 *
 *
 */
void display(unsigned char *time_array);


/*
 *  Description: Compares two time arrays and returns 1 if the arrays are have the same elements; otherwise returns 0
 */
unsigned int compareTimeArray(unsigned char *time_array1,unsigned char *time_array2);


/*
 *  Description: The elements of the time array oldTime_array are copied into the time array newTime_array
 */
void setTime(unsigned char *newTime_array, unsigned char *oldTime_array);


/*
 *  Description: Interrupt handler that is executed once every 10ms.
 *  			 (1) Acknowledges the interrupts in TIMER1
 *               (1) Updates the time array "currentTime"
 *               (2) Checks if 20ms have elapsed. If so, the state of the pushbuttons are stored in  "currentButtonsState"
 *               (3) Checks if 250ms have elapsed. If so, inverts all bits in the variable "blinkerOnOff"
 *               (4) Acknowledges the interrupts for TIMER1 in the interrupt controller (modifies the IAR register)
 */
void timer1ClockInterruptHandler() __attribute__((fast_interrupt));


/*
 *  Description: Interrupt handler that is executed 5s after the function enableTimer2() has been invoked .
 *  			 (1) Acknowledges the interrupts in TIMER2
 *               (2) Sets the alarm-clock state to RUN
 *               (3) Acknowledges the interrupts for TIMER2 in the interrupt controller (modifies the IAR register) *
 */
void timer2ClockInterruptHandler() __attribute__((fast_interrupt));

void incrementArray(unsigned char *timeArray, unsigned int x);

#endif /* ALARM_CLOCK_H_ */
