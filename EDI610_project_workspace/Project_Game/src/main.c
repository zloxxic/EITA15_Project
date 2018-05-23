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
#include "address_mapping.h"
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
unsigned int currentButtonsState = 0;
unsigned int prevButtonsState = 0;

unsigned int x = 0, y = 0, old_x, old_y;
unsigned char color = 0;
unsigned int bounceCounter = 0;

unsigned int currentKeyCode = 0;
unsigned int lastKeyCode = 0;

unsigned char menu = 0;
unsigned char player = 0;
unsigned char computer = 0;

unsigned int scorePlayer;
unsigned int scoreOpponent;
unsigned char lifePlayer;
unsigned char lifeOpponent;
char scorePlayerTemp[12];
char scoreOpponentTemp[12];
char lifePlayerTemp[12];
char lifeOpponentTemp[12];

unsigned int highScore[10];
unsigned char highTemp[12];
unsigned char roundTemp[12];
unsigned char turn = 0;

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
int random();
void resetStats();

int main(void) {
	setup();
	while (1 == 1) {
		if(scorePlayer<1000){
			resetDisplayAtIndex(3);
		}
		if(scorePlayer<100){
			resetDisplayAtIndex(2);
		}
		if(scorePlayer<10){
			resetDisplayAtIndex(1);
			resetDisplayAtIndex(0);
		}

		//displayNumber(timeSinceStart);
		//displayNumber(currentKeyCode);
		displayNumber(scorePlayer);

		switch (menu) {

		//MAIN MENU
		case 0:

			drawString("Welcome to BATTLESHIPS!", 10, 10);
			drawString("Are YOU ready for the challenge of your life?", 10, 20);
			drawString("GOOD! Press ENTER to START!", 10, 30);
			drawString("Press 'H' for highscore", 10, 50);

			drawString("Made by", 0, 460);
			drawString("Simon, Oskar, Victor, Richard", 0, 470);
			if(menu == 1){
				clearScreen(COLOR_BLACK);
				placeBoard(playerNet);
				placeBoard(opponentNet);
				reDraw();
			}else if(menu == 5){
				clearScreen(COLOR_BLACK);
			}
			break;

			//GAME
		case 1:

			if(lastKeyCode == 0x2d ){
				placeBoard(playerNet);
				placeBoard(opponentNet);
				lastKeyCode = 0;
				reDraw();
			}

			if (player == 1) {

//				while(computer == 0){
//					computer = Bomb(playerNet, (random() % 10) + 1,((random() / 10) % 10) + 1);
//				}
				guessIndex(playerNet);
				player = 0;
				//computer = 0;
				reDraw();

			}
			if(x != old_x || y != old_y){
				reDraw();
				old_x = x;
				old_y = y;
			}


			break;

			//MATCH FINISHED PLAYER WON
		case 2:
			drawString("WELL PLAYED!", 10, 10);
			drawString("You defended Östersjön from the Ryssland!", 10, 20);
			sprintf(scorePlayerTemp, "%d", scorePlayer);
			drawString("Your score was: ", 10, 30);
			drawString("Press ENTER to continue", 10, 50);
			drawString(scorePlayerTemp, 146, 30);

			if(menu != 2){
				highScore[turn] = scorePlayer;
				turn++;
				clearScreen(COLOR_BLACK);
			}
			break;

			//MATCH FINISHED OPPONENT WON
		case 3:
			drawString("YOU DIED", 10, 10);
			sprintf(scorePlayerTemp, "%d", scorePlayer);
			drawString("Your score was: ", 10, 30);
			drawString(scorePlayerTemp, 146, 30);
			drawString("Press ENTER to continue", 10, 50);
			if(menu != 3){
				clearScreen(COLOR_BLACK);
			}
			break;
			//MATCH FINISHED OPPONENT WON
		case 4:
			drawString("WELL PLAYED!", 10, 10);
			drawString("ITS A TIE", 10, 20);
			sprintf(scorePlayerTemp, "%d", scorePlayer);
			drawString("Your score was: ", 10, 30);
			drawString(scorePlayerTemp, 146, 30);
			drawString("Press ENTER to continue", 10, 50);
			if(menu != 4){
				clearScreen(COLOR_BLACK);
			}
			break;
		case 5:
			for(int i = 0; i < 10; i++){
				sprintf(highTemp, "%d", highScore[i]);
				sprintf(roundTemp, "%d", i+1);
				drawString(roundTemp, 45, 20+10*i);
				drawString(highTemp, 110, 20+10*i);
			}

			drawString("Game", 34, 10);
			drawString("Score", 94, 10);
			drawString("Press ESC to Exit", 10, 20+10*10+20);
			if(menu != 5){
				clearScreen(COLOR_BLACK);
			}
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
	lifeOpponent = 16;
	lifePlayer = 16;
	scoreOpponent = 1000;
	scorePlayer = 1000;
	for(int i = 1; i <= 10; i++){
		for(int j = 1; j <= 10; j++){
			if(opponentNet[i][j] == 3 || opponentNet[i][j] == 4){
				lifeOpponent--;
			}
			if(playerNet[i][j] == 3 || playerNet[i][j] == 4){
				lifePlayer--;
			}
			if(opponentNet[i][j] == 2){
				scorePlayer-=10;
			}
			if(playerNet[i][j] == 2){
				scoreOpponent-=10;
			}

		}
	}

	//Guide
	drawString("Aim with the Arrows", 10, 10);
	drawString("Fire with 'ENTER'", 10, 20);
	drawString("Exit game with 'ESC'", 10, 30);
	drawString("Reset game with 'R'", 10, 40);

	drawString("PLAYER", 16, 130);
	drawString("COMPUTER", (640 - 16 * 11), 130);

	//Draw Life
	drawSquare(16, 150, 160, 158, COLOR_BLACK);
	drawSquare(620-16*8, 150, 620-16*4, 158, COLOR_BLACK);
	sprintf(lifePlayerTemp, "%d", lifePlayer);
	sprintf(lifeOpponentTemp, "%d", lifeOpponent);
	drawString("Life: ", 16, 150);
	drawString(lifePlayerTemp, 70, 150);
	drawString("Life: ", (640 - 16 * 11), 150);
	drawString(lifeOpponentTemp, 620-(16*7)+10, 150);


	//Draw Scores
	drawSquare(321,90, 450, 98, COLOR_BLACK);
//	drawSquare(620-16*8, 150, 620-16*4, 158, COLOR_BLACK);
	sprintf(scorePlayerTemp, "%d", scorePlayer);
//	sprintf(scoreOpponentTemp, "%d", scoreOpponent);
	drawString("Score: ", 273, 90);
	drawString(scorePlayerTemp, 321, 90);
//	drawString("Score: ", (640 - 16 * 11), 150);
//	drawString(scoreOpponentTemp, 620-(16*7)+10, 150);
	unsigned char SWITCHABLE_COLOR = 0;
	drawSquare(318, 100, 320, 480, COLOR_WHITE);
	drawSquare(0, 100, 639, 101, COLOR_WHITE);
	if(*SWITCHES_DATA != 0){
		SWITCHABLE_COLOR = COLOR_RED;
	}else{
		SWITCHABLE_COLOR = COLOR_YELLOW;

	}

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
				drawTexture(16 * j - 16 + 16, //x
						16 * i - 16 + 160, //y
						TILE_NEUTRAL);
				drawSquare(16 * j - 16 + 16 + 4, //x_start
						16 * i - 16 + 160+4, //y_start
						16 * j - 16 + 7 + 4 + 16, //x_end
						16 * i - 16 + 7 + 4 + 160, //y_end
						SWITCHABLE_COLOR);
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
				if (opponentNet[i][j] == 0) { //Tom ruta
					drawTexture(16 * j - 16 + (640 - 16 - 16*10), //x
							16 * i - 16 + 160, //y
							TILE_NEUTRAL);
				} else if (opponentNet[i][j] == 1) { //Båt
					drawTexture(16 * j - 16 + (640 - 16 - 16*10), //x
							16 * i - 16 + 160, //y
							TILE_NEUTRAL);
				} else if (opponentNet[i][j] == 2) { //missad ruta
					drawTexture(16 * j - 16 + (640 - 16 - 16*10), //x
							16 * i - 16 + 160, //y
							TILE_NEUTRAL);
					drawSquare(16 * j - 16 + (640 - 16 - 16*10)+4, //x_start
							16 * i - 16 + 160+4, //y_start
							16 * j - 16 + 7 + 4 + (640 - 16 - 16*10), //x_end
							16 * i - 16 + 7 + 4 + 160, //y_end
							SWITCHABLE_COLOR);
				} else if (opponentNet[i][j] == 3) { //träffad båt
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
int random(){
	int random = timeSinceStart%1000;
//	if(random <0)
//		random*=-1;
//	do{
//		random %= 10;
//	}while(random < 0 || random > max);
	return random;
}

void resetStats(){
	scorePlayer = 1000;
	scoreOpponent = 1000;
	lifePlayer = 16;
	lifeOpponent = 16;
	player = 0;
	computer = 0;

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
	currentButtonsState = *BUTTONS_DATA;
	for (int d = 1; d <= 12500; d++) {
		//EMPTY DELAY
	}
	if (currentButtonsState != prevButtonsState) {
		if (((currentButtonsState & MIDDLE)!= (prevButtonsState & MIDDLE)) && ((currentButtonsState & MIDDLE) != 0)){
			if (menu == 1) {
				if (opponentNet[x + 1][y + 1] == 0) {
					scorePlayer -= 10;
				}
				if (player == 0){
					player = Bomb(opponentNet, x + 1, y + 1);
				}

				reDraw();

				if(lifePlayer <= 0 && lifeOpponent <= 0){
					clearScreen(COLOR_BLACK);
					menu = 4;
				}else if(lifePlayer <= 0){
					clearScreen(COLOR_BLACK);
					menu = 3;
				}else if(lifeOpponent <= 0){
					clearScreen(COLOR_BLACK);
					menu = 2;
				}
			}
		}
		if (((currentButtonsState & LEFT) != (prevButtonsState & LEFT)) && ((currentButtonsState & LEFT) != 0)){
			if(x>0){
				x--;
			}
		}
		if (((currentButtonsState & RIGHT) != (prevButtonsState & RIGHT)) && ((currentButtonsState & RIGHT) != 0)){
			if(x<9){
				x++;
			}
		}
		if (((currentButtonsState & UP) != (prevButtonsState & UP)) && ((currentButtonsState & UP) != 0)){
			if(y>0){
				y--;
			}
		}
		if (((currentButtonsState & DOWN) != (prevButtonsState & DOWN)) && ((currentButtonsState & DOWN) != 0)){
			if(y<9){
				y++;
			}
		}
		prevButtonsState = currentButtonsState;
	}
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
			//ENTER
		case 0x5A:
			if(menu == 0){
				x = 0;
				y = 0;
				menu = 1;
				resetStats();
			}else if (menu == 1) {
				if (opponentNet[y + 1][x + 1] == 0) {
					scorePlayer -= 10;
				}
				if (player == 0){
					player = Bomb(opponentNet, x + 1, y + 1);
					if(opponentNet[y + 1][x + 1] == 3 || opponentNet[y + 1][x + 1] == 4){
						*LED_DATA = 0xFFFF;
						for (int d = 1; d <= 30000; d++) {
							//EMPTY DELAY
						}
						*LED_DATA = 0x0000;
					}
				}

				reDraw();

				if(lifePlayer <= 0 && lifeOpponent <= 0){
					clearScreen(COLOR_BLACK);
					menu = 4;
				}else if(lifePlayer <= 0){
					clearScreen(COLOR_BLACK);
					menu = 3;
				}else if(lifeOpponent <= 0){
					clearScreen(COLOR_BLACK);
					menu = 2;
				}
			}else if(menu == 2||menu == 3||menu == 4){
				menu = 0;
			}

			break;
			//ESC
		case 0x76:
			if(menu == 1){
				menu = 0;
				clearScreen(COLOR_BLACK);

			}else if(menu == 5){
				menu = 0;
			}

			break;
			//R
		case 0x2d:
			if(menu == 1){
				clearScreen(COLOR_BLACK);
				resetStats();
			}
			break;
		case 0x33:
			if(menu == 0){
				menu = 5;
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
