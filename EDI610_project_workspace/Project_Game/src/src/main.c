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
char score[] = "Score: ";
char scoreDef[] = "Score: ";
/*
 * Spelvariabler
 */
int net[12][12];

/*
 * Function prototypes
 */
void buttonsInterruptHandler() __attribute__((fast_interrupt));
void keyboardInterruptHandler() __attribute__((fast_interrupt));
void setup();

int main(void) {
	setup();
	while (1 == 1) {
		resetDisplays();
		displayNumber(scorePlayer);
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
			}
			break;

			//GAME
		case 1:


			if(x != old_x || y != old_y){
				reDraw();
				old_x = x;
				old_y = y;
			}


			if(menu != 1){
				clearScreen(COLOR_BLACK);
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
	*IVAR4 = (unsigned int) keyboardInterruptHandler;
	*MER |= 0b11;
	*IER = 0b10001;
	*IAR = 0b10001;

	/*
	 * Enable interrupts for processor
	 */
	enableMicroBlazeInterrupts();

	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 12; j++) {
			net[i][j] = 0;
		}
	}

	Ship* ship = Ship_create(1, 10, 10, 0);
	Ship* ship1 = Ship_create(3, 1, 6, 1);
	Ship* ship2 = Ship_create(10, 1, 10, 2);
	Ship* ship3 = Ship_create(8, 10, 6, 3);

	Ship_place(ship, net);
	Ship_place(ship1, net);
	Ship_place(ship2, net);
	Ship_place(ship3, net);

	Bomb(net, 1, 1);
	Bomb(net, 1, 2);
	Bomb(net, 1, 3);
	Bomb(net, 1, 4);
	Bomb(net, 1, 5);
	Bomb(net, 1, 6);
	Bomb(net, 1, 7);
	Bomb(net, 1, 8);
	Bomb(net, 1, 9);
	Bomb(net, 1, 10);
	Bomb(net, 1, 11);

}

void reDraw(){
	drawSquare(16, 150, 160, 158, COLOR_BLACK);
	sprintf(scorePlayerTemp, "%d", scorePlayer);

	strcpy(score, scoreDef);
	drawString("Round 12", 16, 130);
	drawString(strcat(score, scorePlayerTemp), 16, 150);
	drawString("Score: 1337", (640 - 16 * 11), 150);

	drawSquare(319, 100, 320, 480, COLOR_WHITE);
	drawSquare(0, 100, 639, 101, COLOR_WHITE);

	for (int i = 1; i < 11; i++) {

		for (int j = 1; j < 11; j++) {
			if (net[i][j] == 0) {
				drawTexture(16 * j - 16 + 16, 16 * i - 16 + 160,
						TILE_NEUTRAL);
			} else if (net[i][j] == 1) {
				drawSquare(16 * j - 16 + 16, 16 * i - 16 + 160,
						16 * j - 16 + 15 + 16, 16 * i - 16 + 15 + 160,
						73);
			} else if (net[i][j] == 2) {
				drawSquare(16 * j - 16 + 16, 16 * i - 16 + 160,
						16 * j - 16 + 15 + 16, 16 * i - 16 + 15 + 160,
						COLOR_YELLOW);
			} else if (net[i][j] == 3) {
				drawSquare(16 * j - 16 + 16, 16 * i - 16 + 160,
						16 * j - 16 + 15 + 16, 16 * i - 16 + 15 + 160,
						COLOR_WHITE);

			} else {
				drawSquare(16 * j - 16 + 16, 16 * i - 16 + 160,
						16 * j - 16 + 15 + 16, 16 * i - 16 + 15 + 160,
						COLOR_GREEN);
			}
		}
	}
	for(int yy = 0; yy < 16; yy++){
		for(int xx = 0; xx < 16; xx++){
			if(yy == 0 || yy == 15 || xx == 0 || xx == 15   ){
				drawPixel(xx+x*16 + 16, yy+y*16 + 160, COLOR_RED);
			}
		}
	}
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
			if(y>0)
			y -= 1;
			break;
			// Scan code of down arrow is 0x72
		case 0x72:
			if(y<9)
			y += 1;
			break;
			// Scan code of right arrow is 0x74
		case 0x74:
			if(x<9)
			x += 1;
			break;
			// Scan code of left arrow is 0x6B
		case 0x6B:
			if(x>0)
			x -= 1;
			break;
		case 0x5A:
			if(menu == 0){
				x = 0;
				y = 0;
				menu = 1;
				reDraw();
			}else if(menu == 1){
				Bomb(net, x+1, y+1);
				scorePlayer -= 10;
				reDraw();
			}

			break;
		case 0x76:
			if(menu == 1){
				menu = 0;
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
