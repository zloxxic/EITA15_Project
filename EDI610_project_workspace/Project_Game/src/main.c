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

#include <string.h>
#include "buttons.h"
#include "displays.h"
#include "vga.h"
#include "keyboard.h"
#include "timers.h"
#include "interrupt_controller.h"
#include "microBlaze.h"
#include "textures.h"
#include "string.h"
#include "ship.h"

/*
 * Global variables
 */
unsigned int x = 0, y = 0, old_x, old_y;
unsigned char color = 0;
unsigned int bounceCounter = 0;

unsigned int currentKeyCode = 0;
unsigned int lastKeyCode = 0;

unsigned char menu = 0;

unsigned int scorePlayer = 1000;
char scorePlayerTemp[12];

unsigned long timeSinceStart = 0;
/*
 * Spelvariabler
 */
int playerNet[12][12];
int opponentNet[12][12];

/*
 * Function prototypes
 */
void buttonsInterruptHandler() __attribute__((fast_interrupt));
void keyboardInterruptHandler() __attribute__((fast_interrupt));
void timer1ClockInterruptHandler() __attribute__((fast_interrupt));

void setup();
int random(int max,int seed);

int main(void) {
	setup();
	while (1 == 1) {
		resetDisplays();
		displayNumber(timeSinceStart);
		//clearScreen(COLOR_GREEN);
		//clearScreen(COLOR_BLACK);
		switch (menu) {

		//MAIN MENU
		case 0:

			drawString("Welcome to BATTLESHIPS!", 10, 10);
			drawString("Are YOU ready for the challange of your life?", 10, 20);
			drawString("GOOD! Press ENTER to START!", 10, 30);

			drawString("Made by", 0, 460);
			drawString("Simon, Oskar, Victor, Richard", 0, 470);
			if(menu != 0){
				clearScreen(COLOR_BLACK);
				placeBoard(playerNet);
				placeBoard(opponentNet);
				reDraw();
			}
			break;

			//GAME
		case 1:


			if(x != old_x || y != old_y){
				reDraw();
				old_x = x;
				old_y = y;
			}



			break;

			//HIGHSCORE
		case 2:
			break;

			//SETTINGS
		case 3:
			break;

		default:
			break;

		}

		//drawString("GAME OVER", 270, 200, charMap);
		//drawTexture(0,0,TILE_NEUTRAL);
	}
}

void setup() {
	/*
	 * Configure data direction for I/O devices
	 */
	*BUTTONS_CONTROL = 0x1F;
	initTimers_Clock();
	initController_Clock();
	/*
	 * Draw initial screen
	 */
	//x = VGA_WIDTH / 2;
	//y = VGA_HEIGHT / 2;
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
	enableTimer1();


	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 12; j++) {
			playerNet[i][j] = 0;
		}
	}

	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 12; j++) {
			opponentNet[i][j] = 0;
		}
	}



}

void reDraw(){
	drawSquare(16, 150, 160, 158, COLOR_BLACK);
	sprintf(scorePlayerTemp, "%d", scorePlayer);

	drawString("Round 12", 16, 130);
	drawString("Score ", 16, 150);
	drawString(scorePlayerTemp, 75, 150);
	drawString("Score: 1337", (640 - 16 * 11), 150);

	drawSquare(319, 100, 320, 480, COLOR_WHITE);
	drawSquare(0, 100, 639, 101, COLOR_WHITE);

	//Rita spelarens spelplan
	for (int i = 1; i < 11; i++) {

		for (int j = 1; j < 11; j++) {
			if (playerNet[i][j] == 0) {
				drawTexture(16 * j - 16 + 16, //x
						16 * i - 16 + 160, //y
						TILE_NEUTRAL);
			} else if (playerNet[i][j] == 1) {
				drawSquare(16 * j - 16 + 16, //x_start
						16 * i - 16 + 160, //y_start
						16 * j - 16 + 15 + 16, //x_end
						16 * i - 16 + 15 + 160, //y_end
						73);
			} else if (playerNet[i][j] == 2) {
				drawSquare(16 * j - 16 + 16, //x_start
						16 * i - 16 + 160, //y_start
						16 * j - 16 + 15 + 16, //x_end
						16 * i - 16 + 15 + 160, //y_end
						COLOR_YELLOW);
			} else if (playerNet[i][j] == 3) {
				drawSquare(16 * j - 16 + 16, //x_start
						16 * i - 16 + 160, //y_start
						16 * j - 16 + 15 + 16, //x_end
						16 * i - 16 + 15 + 160, //y_end
						COLOR_WHITE);

			} else {
				drawSquare(16 * j - 16 + 16, //x_start
						16 * i - 16 + 160, //y_start
						16 * j - 16 + 15 + 16, //x_end
						16 * i - 16 + 15 + 160, //y_end
						COLOR_GREEN);
			}
		}
	}

	//Rita opponentens spelplan
	for (int i = 1; i < 11; i++) {
			for (int j = 1; j < 11; j++) {
				if (opponentNet[i][j] == 0) {
					drawTexture(16 * j - 16 + (640 - 16 - 16*10), //x
							16 * i - 16 + 160, //y
							TILE_NEUTRAL);
				} else if (opponentNet[i][j] == 1) {
					drawTexture(16 * j - 16 + (640 - 16 - 16*10), //x
							16 * i - 16 + 160, //y
							TILE_NEUTRAL);
				} else if (opponentNet[i][j] == 2) {
					drawTexture(16 * j - 16 + (640 - 16 - 16*10), //x
							16 * i - 16 + 160, //y
							TILE_NEUTRAL);
					drawSquare(16 * j - 16 + (640 - 16 - 16*10)+4, //x_start
							16 * i - 16 + 160+4, //y_start
							16 * j - 16 + 7 + 4 + (640 - 16 - 16*10), //x_end
							16 * i - 16 + 7 + 4 + 160, //y_end
							COLOR_YELLOW);
				} else if (opponentNet[i][j] == 3) {
					drawSquare(16 * j - 16 + (640 - 16 - 16*10), //x_start
							16 * i - 16 + 160, //y_start
							16 * j - 16 + 15 + (640 - 16 - 16*10), //x_end
							16 * i - 16 + 15 + 160, //y_end
							COLOR_WHITE);

				} else {
					drawSquare(16 * j - 16 + (640 - 16 - 16*10), //x_start
							16 * i - 16 + 160, //y_start
							16 * j - 16 + 15 + (640 - 16 - 16*10), //x_end
							16 * i - 16 + 15 + 160, //y_end
							COLOR_GREEN);
				}
			}
		}

	for(int yy = 0; yy < 16; yy++){
		for(int xx = 0; xx < 16; xx++){
			if(yy == 0 || yy == 15 || xx == 0 || xx == 15   ){
				drawPixel(xx+x*16 + (640 - 16 - 16*10), yy+y*16 + 160, COLOR_RED);
			}
		}
	}
}

/*returnerar heltal mellan 0 och max.(använde tiden från senaste knapptryckningen % något tills det blir mindre än max*/
int random(int max,int seed){
	int random = timeSinceStart*7*seed;
	if(random <0)
		random*=-1;
	do{
		random %= 10;
	}while(random < 0 || random > max);
	return random;
}

void initTimers_Clock() {
	*TIMER1_LOAD = 359;
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
	//displayNumber(timeSinceStart);
	*TIMER1_CTRL|=(1<<8);

	*IAR = 1 << TIMER1_IRQ;
}

void buttonsInterruptHandler() {
	// increment button interrupt counter. Note that a single press of a button can generate
	// multiple interrupts due to bouncing.
	bounceCounter = bounceCounter + 1;

	// Acknowledge the interrupt. Note that BUTTON_ISR is toggle on write!
	*BUTTONS_ISR &= 0x01;
	*IAR = 1 << BUTTONS_IRQ;

}

void keyboardInterruptHandler() {
	// Read the newly received scan code
	lastKeyCode = *KB_DATA;
	int d;
	for (d = 1; d <= 12500; d++) {
		//EMPTY DELAY
	}
	currentKeyCode = *KB_DATA;
	if (lastKeyCode == currentKeyCode) {
		// Check if any of the arrows where pressed
		switch (currentKeyCode) {
		// Scan code of up arrow is 0x75
		case 0x75:
			if(y>0){
				y--;
			}

			break;
			// Scan code of down arrow is 0x72
		case 0x72:
			if(y<9){
				y++;
			}

			break;
			// Scan code of right arrow is 0x74
		case 0x74:
			if(x<9){
				x++;
			}

			break;
			// Scan code of left arrow is 0x6B
		case 0x6B:
			if(x>0){
				x--;
			}
			//x = random(10,65);
			//y = random(10,36);
			break;
		case 0x5A:
			if(menu == 0){
				x = 0;
				y = 0;
				menu = 1;
			}else if(menu == 1){
				Bomb(opponentNet, x+1, y+1);
				scorePlayer -= 10;
				reDraw();
			}

			break;
		case 0x76:
			if(menu == 1){
				menu = 0;
				clearScreen(COLOR_BLACK);
				scorePlayer = 1000;
			}

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
