//
// EDI610 Hardware Platform 2017 demo
//
// This application make use of the following devices of the hardware platform:
// 		-- Buttons Device
//		-- Seven Segment Controller
//		-- Interrupt Controller
//		-- VGA Controller
//		-- Keyboard Controller
//
//		The Buttons Device and Keyboard Controller are configured to generate interrupts.
//		Initially the screen is painted black with a blue bar at the top and a red bar at the
//		bottom. Then a single pixel is set to green. The coordinates of the green pixel is stored in
//		two global variables, x and y.
//
//		Once a new scan code is received from the keyboard the interrupt handler will check if
//		any of the arrow keys where pressed. If so the coordinates of the green pixel will be changed
//		according to the direction indicated by the arrow pressed. Note that the actual redrawing of
//		the green pixel is done in the main function. The purpose of this functionality is to demonstrate
//		how the VGA controller is used to draw things on a display.
//
//		Once any of the five pushbuttons is pressed or released an interrupt is generated. Each time
//		the interrupt handler for the buttons is executed a counter, bounceCounter, is incremented.
//		In the main function the value of bounceCounter is displayed on the seven segment displays.
//		The purpose of including this part is to demonstrate how to set up interrupts for the Buttons
//		Device. And also to highlight potential problems caused of button bouncing.
//
//		During the project you are free to make any modifications in the supplied code.
//
//		WARNING: Keep in mind that any new source file added to the project should be placed under the "src" folder.
//				 If you by mistake create a file in another folder:
//					1: Move the file to the "src" folder.
//					2: Delete "subdir.mk" located in the "Debug" folder.
//

#include "buttons.h"
#include "displays.h"
#include "vga.h"
#include "keyboard.h"
#include "interrupt_controller.h"
#include "microBlaze.h"

/*
 * Global variables
 */
unsigned int x,y, old_x, old_y;
unsigned char color = 0;
unsigned int bounceCounter = 0;

/*
 * Function prototypes
 */
void buttonsInterruptHandler() __attribute__((fast_interrupt));
void keyboardInterruptHandler() __attribute__((fast_interrupt));
void setup();

int main(void){
	setup();
	while(1==1){
		displayNumber(bounceCounter);
		if(x != old_x || y != old_y){
			drawPixel(old_x, old_y, COLOR_BLACK);
			drawPixel(x, y, COLOR_GREEN);
			old_x = x;
			old_y = y;
		}

	}
}

void setup(){
	/*
	 * Configure data direction for I/O devices
	 */
	*BUTTONS_CONTROL = 0x1F;

	/*
	 * Draw initial screen
	 */
	x = VGA_WIDTH/2;
	y = VGA_HEIGHT/2;
	old_x = x;
	old_y = y;
	clearScreen(COLOR_BLACK);
	drawSquare(0, 0, VGA_WIDTH-1, 5, COLOR_YELLOW);
	drawSquare(0, VGA_HEIGHT-6, VGA_WIDTH-1, VGA_HEIGHT-1, COLOR_RED);
	drawPixel(x,y,COLOR_GREEN);

	/*
	 * Configure interrupts for the five push buttons
	 */
	*BUTTONS_IER = 0x01;
	*BUTTONS_GIER = 1 << 31;
	*BUTTONS_ISR &= 0x01; // writing ones toggle the value of corresponding bits

	/*
	 * Configure keyboard controller
	 */
	*KB_CTRL = (1 << 1) | (1 << 0) | (1 << 2);

	/*
	 * Configure interrupt controller
	 */
	*IVAR0 = (unsigned int) buttonsInterruptHandler;
	*IVAR4 = (unsigned int) keyboardInterruptHandler;
	*MER |= 0b11;
	*IER = 0b10001;
	*IAR = 0b10001;

	/*
	 * Enable interrupts for processor
	 */
	enableMicroBlazeInterrupts();

}

void buttonsInterruptHandler(){
	// increment button interrupt counter. Note that a single press of a button can generate
	// multiple interrupts due to bouncing.
	bounceCounter = bounceCounter + 1;

	// Acknowledge the interrupt. Note that BUTTON_ISR is toggle on write!
	*BUTTONS_ISR &= 0x01;
	*IAR = 1 << BUTTONS_IRQ;

}

void keyboardInterruptHandler(){
	// Read the newly received scan code
	unsigned int keyCode = *KB_DATA;

	// Check if any of the arrows where pressed
	switch(keyCode){
	// Scan code of up arrow is 0x75
	case 0x75:
		y--;
		break;
	// Scan code of down arrow is 0x72
	case 0x72:
		y++;
		break;
	// Scan code of right arrow is 0x74
	case 0x74:
		x++;
		break;
	// Scan code of left arrow is 0x6B
	case 0x6B:
		x--;
		break;
	default:
		break;
	}
	// Acknowledge the interrupt
	*KB_CTRL |= (1 << 2);
	*IAR = 1 << KEYBOARD_IRQ;
}
