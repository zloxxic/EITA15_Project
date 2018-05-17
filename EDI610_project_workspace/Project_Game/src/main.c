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
#include "textures.h"
#include "timers.h"
//#include "timer_clock.h"

/*
 * Global variables
 */
unsigned int x,y, old_x, old_y;
unsigned char color = 0;
unsigned int bounceCounter = 0;

unsigned int currentKeyCode = 0;
unsigned int lastKeyCode = 0;
unsigned long timeSinceStart = 0;
unsigned char pixlar[40][40];


/*
 * Function prototypes
 */
void buttonsInterruptHandler() __attribute__((fast_interrupt));
void keyboardInterruptHandler() __attribute__((fast_interrupt));
void timer1ClockInterruptHandler() __attribute__((fast_interrupt));
void setup();
int random(int max);

int main(void){
	setup();
	while(1==1){
		displayNumber(*KB_DATA);
		//clearScreen(COLOR_GREEN);

		if(x != old_x || y != old_y){
			//reDraw();
			int r = random(10);
			for (int d = 1; d <= 12500; d++){
					//EMPTY DELAY
				}
			int b = random(10);
//			drawSquare(x+2, y+2, x+13, y+13, 224);
			drawSquare(r*16+2, b*16+2, r*16+13, b*16+13, 224);
//			drawPixel(old_x, old_y, COLOR_BLACK);
//			drawPixel(x, y, COLOR_GREEN);
			old_x = x;
			old_y = y;
		}



	}
}

void reDraw(){
	for (int yy = 0; yy < 10; yy++) {
			for (int xx = 0; xx < 10; xx++) {
				drawTexture(xx  * 16, yy * 16, TILE_NEUTRAL);
			}
		}
}
void setup(){
	/*
	 * Configure data direction for I/O devices
	 */
	initTimers_Clock();
	initController_Clock();
	*BUTTONS_CONTROL = 0x1F;

	/*
	 * Draw initial screen
	 */
	x = 0;
	y = 0;
	old_x = x;
	old_y = y;
	clearScreen(COLOR_BLACK);

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
	*IVAR1 = (unsigned int) timer1ClockInterruptHandler;
	*IVAR4 = (unsigned int) keyboardInterruptHandler;
	*MER |= 0b11;
	*IER = 0b10011;
	*IAR = 0b10001;

	/*
	 * Enable interrupts for processor
	 */
	enableMicroBlazeInterrupts();
	reDraw();
	enableTimer1();

	/*
	 * fill pixelarray
	 */
//	for(int i =0; i < 20;i++){
//		for(int j =0; j < 20;j++){
//			pixlar[i][j] = (i*j)/2;
//		}
//	}
//	for (int yy = 0; yy < 20; yy++) {
//		for (int xx = 0; xx < 20; xx++) {
//			drawPixel(xx * 5, yy * 5, pixlar[xx][yy]);
//			//drawTexture(xx*16, yy*16,TILE_NEUTRAL);
//		}
//	}


	drawSquare(x+2, y+2, x+13, y+13, 224);

}

void buttonsInterruptHandler(){
	// increment button interrupt counter. Note that a single press of a button can generate
	// multiple interrupts due to bouncing.
	bounceCounter = bounceCounter + 1;

	// Acknowledge the interrupt. Note that BUTTON_ISR is toggle on write!
	*BUTTONS_ISR &= 0x01;
	*IAR = 1 << BUTTONS_IRQ;

}

/*returnerar heltal mellan 0 och max.(använde tiden från senaste knapptryckningen % något tills det blir mindre än max*/
int random(int max){
	int random = 0;
	do{
		random = timeSinceStart%10;
	}while(random < 0 || random >= max);
	return random;
}

void initTimers_Clock() {
	*TIMER1_LOAD = 4359;
	*TIMER1_CTRL =(1<<8)|(1<<6)|(1<<5)|(1<<4)|(1<<1);

}

void enableTimer1() {
	*TIMER1_CTRL=(*TIMER1_CTRL|(1<<7))&(~(1<<5));

}

void initController_Clock() {
	*IER|=0b110;


}

void timer1ClockInterruptHandler() {
	timeSinceStart++;
//	displayNumber(timeSinceStart);
	*TIMER1_CTRL|=(1<<8);
	*IAR = 1 << TIMER1_IRQ;
}

void keyboardInterruptHandler(){
	// Read the newly received scan code
	lastKeyCode = *KB_DATA;
	int c, d;
	for (d = 1; d <= 12500; d++){
		//EMPTY DELAY
	}
	currentKeyCode = *KB_DATA;
	if(lastKeyCode == currentKeyCode){
		reDraw();
		// Check if any of the arrows where pressed
		switch(currentKeyCode){
		// Scan code of up arrow is 0x75
		case 0x75:
			//reDraw();
			y-=16;

			break;
		// Scan code of down arrow is 0x72
		case 0x72:
			//reDraw();
			y+=16;

			break;
		// Scan code of right arrow is 0x74
		case 0x74:
			//reDraw();
			x+=16;

			break;
		// Scan code of left arrow is 0x6B
		case 0x6B:
			//reDraw();
			x-=16;

			break;
		default:
			break;
		}
		bounceCounter++;
	}
	// Acknowledge the interrupt
	*KB_CTRL |= (1 << 2);
	*IAR = 1 << KEYBOARD_IRQ;
}
