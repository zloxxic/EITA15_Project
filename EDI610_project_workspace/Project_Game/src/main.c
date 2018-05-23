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
//Currently pressed button on FPGA
unsigned int currentButtonsState = 0;
//Previously pressed button on FPGA
unsigned int prevButtonsState = 0;

//Coordinates for cursor
unsigned int x = 0, y = 0, old_x, old_y;

//Currently pressed key on keyboard
unsigned int currentKeyCode = 0;
//Previously pressed key on keyboard
unsigned int lastKeyCode = 0;

//Menu variable used to navigate game
unsigned char menu = 0;

//Variable used to toggle if it's players turn
unsigned char player = 0;

//Score for player (Default 1000)
unsigned int scorePlayer;
//Life for player (Default 16)
unsigned char lifePlayer;
//Life for opponent (Default 16)
unsigned char lifeOpponent;

//Variables used to convert INT to String/char-array
unsigned char scorePlayerTemp[12];
unsigned char lifePlayerTemp[12];
unsigned char lifeOpponentTemp[12];
unsigned char highTemp[12];
unsigned char roundTemp[12];

//Stores the scores from the last games
unsigned int highScore[10];
unsigned char round = 0;

unsigned long timeSinceStart = 0;

//Nets for storing where boats are and what tiles are hit etc.
int playerNet[12][12];
int opponentNet[12][12];

/*
 * Function prototypes
 */
void buttonsInterruptHandler() __attribute__((fast_interrupt));
void keyboardInterruptHandler() __attribute__((fast_interrupt));
void timer1ClockInterruptHandler() __attribute__((fast_interrupt));

/*
 * Init functions
 */
void setup();
int random();
void resetStats();

int main(void) {
	setup();
	while (1 == 1) {

		//Removes unused displays when displaying scorePlayer.
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
		displayNumber(scorePlayer);
		//displayNumber(timeSinceStart);
		//displayNumber(currentKeyCode);

		/*
		 * Switch for all different screens and modes in the game
		 * 0 - Main menu
		 * 1 - Game
		 * 2 - Match won
		 * 3 - Match lost
		 * 4 - Match tie
		 * 5 - Highscore
		 *
		 * **States often changed in interrupts**
		 */
		switch (menu) {

		/*
		 * MAIN MENU
		 *
		 * Used to display welcome text and to start the game (or highscore)
		 */
		case 0:

			//Welcome text displayed in the top left corner
			drawString("Welcome to BATTLESHIPS!", 10, 10);
			drawString("Are YOU ready for the challenge of your life?", 10, 20);
			drawString("GOOD! Press ENTER to START!", 10, 30);
			drawString("Press 'H' for highscore", 10, 50);

			//Credit text displayed in the bottom left corner
			drawString("Made by", 0, 460);
			drawString("Simon, Oskar, Victor, Richard", 0, 470);

			//If the menu changes from 0 to 1 (enters game-mode)
			if(menu == 1){
				clearScreen(COLOR_BLACK);
				placeBoard(playerNet);
				placeBoard(opponentNet);
				reDraw();
			}
			//If the menu changes to Highscore-menu
			else if(menu == 5){
				clearScreen(COLOR_BLACK);
			}
			break;

			/*
			 * GAME MENU
			 *
			 * Everything regarding the game runs from here
			 */
		case 1:

			//if the previous key is 'R' then do
			if(lastKeyCode == 0x2d ){
				placeBoard(playerNet);
				placeBoard(opponentNet);
				lastKeyCode = 0;
				reDraw();
			}

			//if player variable is 1 (last shot was a successful/not invalid one)
			if (player == 1) {

				//Run game with low-level Ai (Dumb Ai)
//				while(computer == 0){
//					computer = Bomb(playerNet, (random() % 10) + 1,((random() / 10) % 10) + 1);
//				}
				//Run game with high-level Ai (smart Ai)
				guessIndex(playerNet);

				//reset players shot
				player = 0;
				reDraw();

			}
			//If the cursor is moved, then update screen.
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
			//Convert int to string/char-array
			sprintf(scorePlayerTemp, "%d", scorePlayer);
			drawString("Your score was: ", 10, 30);
			drawString(scorePlayerTemp, 146, 30);
			drawString("Press ENTER to continue", 10, 50);

			//If menu isn't this menu anymore
			if(menu != 2){
				highScore[round] = scorePlayer;
				round++;
				clearScreen(COLOR_BLACK);
			}
			break;

			//MATCH FINISHED OPPONENT WON
		case 3:
			drawString("YOU DIED", 10, 10);
			//Convert int to string/char-array
			sprintf(scorePlayerTemp, "%d", scorePlayer);
			drawString("Your score was: ", 10, 30);
			drawString(scorePlayerTemp, 146, 30);
			drawString("Press ENTER to continue", 10, 50);

			//If menu isn't this menu anymore
			if(menu != 3){
				clearScreen(COLOR_BLACK);
			}
			break;
			//MATCH FINISHED TIE
		case 4:
			drawString("WELL PLAYED!", 10, 10);
			drawString("ITS A TIE", 10, 20);
			//Convert int to string/char-array
			sprintf(scorePlayerTemp, "%d", scorePlayer);
			drawString("Your score was: ", 10, 30);
			drawString(scorePlayerTemp, 146, 30);
			drawString("Press ENTER to continue", 10, 50);

			//If menu isn't this menu anymore
			if(menu != 4){
				clearScreen(COLOR_BLACK);
			}
			break;
			//SHOW HIGHSCORE
		case 5:
			//Writes the last 10 scores stacked ontop of eachother
			for(int i = 0; i < 10; i++){
				sprintf(highTemp, "%d", highScore[i]);
				sprintf(roundTemp, "%d", i+1);
				drawString(roundTemp, 45, 20+10*i);
				drawString(highTemp, 110, 20+10*i);
			}

			drawString("Game", 34, 10);
			drawString("Score", 94, 10);
			drawString("Press ESC to Exit", 10, 20+10*10+20);
			//If menu isn't this menu anymore
			if(menu != 5){
				clearScreen(COLOR_BLACK);
			}
			break;
		//If menu isn't a valid number
		default:
			break;

		}

		//drawString("GAME OVER", 270, 200, charMap);
		//drawTexture(0,0,TILE_NEUTRAL);
	}
}

/*
 * Runs at the start of the program
 */
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

	/*
	 * Enable interrupts from timer
	 */
	enableTimer1();

	/*
	 * Init playerNet
	 */
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 12; j++) {
			playerNet[i][j] = 0;
		}
	}

	/*
	 * Init opponentNet
	 */
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 12; j++) {
			opponentNet[i][j] = 0;
		}
	}



}

/*
 * Draws everything from the game-screen
 * Used to update the screen
 */
void reDraw(){
	lifeOpponent = 16;
	lifePlayer = 16;
	scorePlayer = 1000;
	/*
	 * Scans the nets and adjusts the scores and life of player and opponent
	 */
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

		}
	}

	//Guide to what buttons can be pressed
	drawString("Aim with the Arrows", 10, 10);
	drawString("Fire with 'ENTER'", 10, 20);
	drawString("Exit game with 'ESC'", 10, 30);
	drawString("Reset game with 'R'", 10, 40);

	//Visual for who the values and net belong to
	drawString("PLAYER", 16, 130);
	drawString("COMPUTER", (640 - 16 * 11), 130);

	//Draw Life for both player and opponent
	drawSquare(16, 150, 160, 158, COLOR_BLACK);
	drawSquare(620-16*8, 150, 620-16*4, 158, COLOR_BLACK);
	sprintf(lifePlayerTemp, "%d", lifePlayer);
	sprintf(lifeOpponentTemp, "%d", lifeOpponent);
	drawString("Life: ", 16, 150);
	drawString(lifePlayerTemp, 70, 150);
	drawString("Life: ", (640 - 16 * 11), 150);
	drawString(lifeOpponentTemp, 620-(16*7)+10, 150);


	//Draw Score for player
	drawSquare(321,90, 450, 98, COLOR_BLACK);
	sprintf(scorePlayerTemp, "%d", scorePlayer);
	drawString("Score: ", 273, 90);
	drawString(scorePlayerTemp, 321, 90);

	//Divider lines
	drawSquare(318, 100, 320, 480, COLOR_WHITE);
	drawSquare(0, 100, 639, 101, COLOR_WHITE);

	//Switch the color of the missed missiles with the switch while in-game
	unsigned char SWITCHABLE_COLOR = 0;
	if(*SWITCHES_DATA != 0){
		SWITCHABLE_COLOR = COLOR_RED;
	}else{
		SWITCHABLE_COLOR = COLOR_YELLOW;

	}

	//Draws the players net
	for (int i = 1; i < 11; i++) {
		for (int j = 1; j < 11; j++) {
			if (playerNet[i][j] == 0) { //Neutral
				drawTexture(16 * j - 16 + 16, //x
						16 * i - 16 + 160, //y
						TILE_NEUTRAL);
			} else if (playerNet[i][j] == 1) { //Boat
				drawSquare(16 * j - 16 + 16, //x_start
						16 * i - 16 + 160, //y_start
						16 * j - 16 + 15 + 16, //x_end
						16 * i - 16 + 15 + 160, //y_end
						73);
			} else if (playerNet[i][j] == 2) {//Miss
				drawTexture(16 * j - 16 + 16, //x
						16 * i - 16 + 160, //y
						TILE_NEUTRAL);
				drawSquare(16 * j - 16 + 16 + 4, //x_start
						16 * i - 16 + 160+4, //y_start
						16 * j - 16 + 7 + 4 + 16, //x_end
						16 * i - 16 + 7 + 4 + 160, //y_end
						SWITCHABLE_COLOR);
			} else if (playerNet[i][j] == 3) {//Hit
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

	//Draw the opponents net
	for (int i = 1; i < 11; i++) {
			for (int j = 1; j < 11; j++) {
				if (opponentNet[i][j] == 0) { //Neutral
					drawTexture(16 * j - 16 + (640 - 16 - 16*10), //x
							16 * i - 16 + 160, //y
							TILE_NEUTRAL);
				} else if (opponentNet[i][j] == 1) { //Boat (Drawn the same as neutral)
					drawTexture(16 * j - 16 + (640 - 16 - 16*10), //x
							16 * i - 16 + 160, //y
							TILE_NEUTRAL);
				} else if (opponentNet[i][j] == 2) { //Miss (Draws a small square over a neutral tile)
					drawTexture(16 * j - 16 + (640 - 16 - 16*10), //x
							16 * i - 16 + 160, //y
							TILE_NEUTRAL);
					drawSquare(16 * j - 16 + (640 - 16 - 16*10)+4, //x_start
							16 * i - 16 + 160+4, //y_start
							16 * j - 16 + 7 + 4 + (640 - 16 - 16*10), //x_end
							16 * i - 16 + 7 + 4 + 160, //y_end
							SWITCHABLE_COLOR);
				} else if (opponentNet[i][j] == 3) { //Hit
					drawSquare(16 * j - 16 + (640 - 16 - 16*10), //x_start
							16 * i - 16 + 160, //y_start
							16 * j - 16 + 15 + (640 - 16 - 16*10), //x_end
							16 * i - 16 + 15 + 160, //y_end
							COLOR_WHITE);

				} else { //if the isn't a set type for a tile
					drawSquare(16 * j - 16 + (640 - 16 - 16*10), //x_start
							16 * i - 16 + 160, //y_start
							16 * j - 16 + 15 + (640 - 16 - 16*10), //x_end
							16 * i - 16 + 15 + 160, //y_end
							COLOR_GREEN);
				}
			}
		}

	/*
	 * Draw the cursor on the opponents net (Red square with no infill)
	 */
	for(int yy = 0; yy < 16; yy++){
		for(int xx = 0; xx < 16; xx++){
			if(yy == 0 || yy == 15 || xx == 0 || xx == 15   ){
				drawPixel(xx+x*16 + (640 - 16 - 16*10), yy+y*16 + 160, COLOR_RED);
			}
		}
	}
}

/*
 * Returns INTs depending on the time since the start of the game (since the main menu opend)
 */
int random(){
	int random = timeSinceStart%1000;
//	if(random <0)
//		random*=-1;
//	do{
//		random %= 10;
//	}while(random < 0 || random > max);
	return random;
}

/*
 * Resets variables and nets between rounds
 */
void resetStats(){
	x = 0;
	y = 0;
	scorePlayer = 1000;
	lifePlayer = 16;
	lifeOpponent = 16;
	player = 0;

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
	*TIMER1_CTRL|=(1<<8);

	*IAR = 1 << TIMER1_IRQ;
}

/*
 * Interrupts for the buttons on the FPGA
 * Can be used as an alternative to the keyboard while a game is started
 */
void buttonsInterruptHandler() {
	currentButtonsState = *BUTTONS_DATA;

	//prevent bounceing
	for (int d = 1; d <= 12500; d++) {
		//EMPTY DELAY
	}


	if (currentButtonsState != prevButtonsState) {
		//Middle button pressed - Same functions as ENTER
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

		/*
		 * UP, DOWN, LEFT, RIGHT buttons used as an alternative to
		 * the arrows on the keyboard. They have the same functionality
		 * when a game is started.
		 */
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

/*
 * Interrupts for the keyboard connected with USB to FPGA
 */
void keyboardInterruptHandler() {
	// Read the newly received scan code
	lastKeyCode = *KB_DATA;

	//prevent bounceing
	for (int d = 1; d <= 12500; d++) {
		//EMPTY DELAY
	}
	currentKeyCode = *KB_DATA;
	if (lastKeyCode == currentKeyCode) {
		// Check if any of the arrows where pressed
		switch (currentKeyCode) {
			// UP-ARROW
			case 0x75:
				if(y>0){
					y--;
				}

				break;
				// DOWN-ARROW
			case 0x72:
				if(y<9){
					y++;
				}

				break;
				// RIGHT-ARROW
			case 0x74:
				if(x<9){
					x++;
				}

				break;
				// LEFT-ARROW
			case 0x6B:
				if(x>0){
					x--;
				}
				//x = random(10,65);
				//y = random(10,36);
				break;
				//ENTER
			case 0x5A:
				//If current menu is 0, then run this code (change menu to 1)
				if(menu == 0){
					menu = 1;
					resetStats();
				}
				//If current menu is 1 (game) change functionality of enter from select to fire missile.
				else if (menu == 1) {
					//if the player hasn't shot a valid missile
					if (player == 0){
						//try to shoot
						player = Bomb(opponentNet, x + 1, y + 1);
						//If its a hit then flash leds on FPGA
						if(opponentNet[y + 1][x + 1] == 3 || opponentNet[y + 1][x + 1] == 4){
							*LED_DATA = 0xFFFF;
							for (int d = 1; d <= 30000; d++) {
								//EMPTY DELAY
							}
							*LED_DATA = 0x0000;
						}
					}

					reDraw();

					/*
					 * Check if anyone has died
					 */
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
				//If enter pressed in menu 2,3,4 then go to menu 0
				else if(menu == 2||menu == 3||menu == 4){
					menu = 0;
				}

				break;
				//ESC
			case 0x76:
				//if ESC pressed in game then exit game by going to Main menu (menu 0)
				if(menu == 1){
					menu = 0;
					clearScreen(COLOR_BLACK);
				//if ESC pressed in highscore-menu (menu 5) then go to main menu (menu 0)
				}else if(menu == 5){
					menu = 0;
				}

				break;
				//R
			case 0x2d:
				//reset game if pressed in the game-menu (menu 1)
				if(menu == 1){
					clearScreen(COLOR_BLACK);
					resetStats();
				}
				break;
				//H
			case 0x33:
				//Go to highscore-menu if H is pressed in main menu (menu 0)
				if(menu == 0){
					menu = 5;
				}
				break;
			default:
				break;
			}
	}
	// Acknowledge the interrupt
	*KB_CTRL |= (1 << 2);
	*IAR = 1 << KEYBOARD_IRQ;
}
