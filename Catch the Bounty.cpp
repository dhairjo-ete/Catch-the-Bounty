#include <iostream>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70
#define MAX_BASKET_SIZE 20

using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

int fruitY[3];
int fruitX[3];
bool fruitFlag[3];
char basket[MAX_BASKET_SIZE];
char fruits[3][2][3] = {
    { "##", "##" },
};

int basketPosX = WIN_WIDTH / 2;
int basketPosY = 23; // Position for the basket
int score = 0;
int level = 1;
int missedFruits = 0; // Track missed fruits
int maxMissedFruits = 5; // Maximum missed fruits allowed

void gotoxy(int x, int y) {
    CursorPosition.X = x;
    CursorPosition.Y = y;
    SetConsoleCursorPosition(console, CursorPosition);
}

void setcursor(bool visible, DWORD size) {
    if (size == 0) size = 20;

    CONSOLE_CURSOR_INFO lpCursor;
    lpCursor.bVisible = visible;
    lpCursor.dwSize = size;
    SetConsoleCursorInfo(console, &lpCursor);
}

void drawBorder() {
    // Draw the fixed top border
    gotoxy(0, 0);
    for (int j = 0; j < SCREEN_WIDTH; j++) {
        cout << "±"; // Top border
    }
}

void genFruit(int ind) {
    fruitX[ind] = 17 + rand() % (33);
}

void drawFruit(int ind) {
    if (fruitFlag[ind]) { // Using implicit boolean conversion
        gotoxy(fruitX[ind], fruitY[ind]); cout << fruits[ind][0]; // Draw upper part of fruit
        gotoxy(fruitX[ind], fruitY[ind] + 1); cout << fruits[ind][1]; // Draw lower part of fruit
    }
}

void eraseFruit(int ind) {
    if (fruitFlag[ind]) {
        gotoxy(fruitX[ind], fruitY[ind]); cout << "  "; // Erase upper part
        gotoxy(fruitX[ind], fruitY[ind] + 1); cout << "  "; // Erase lower part
    }
}

void resetFruit(int ind) {
    eraseFruit(ind);
    fruitY[ind] = 1; // Reset fruit to the top
    genFruit(ind);
}

void drawBasket() {
    // Draw the basket with brackets and underscores
    for (int i = 0; i < level + 3; i++) { // Adjust the basket size according to level
        if (i == 0) {
            gotoxy(basketPosX, basketPosY); cout << "["; // Left bracket
        } else if (i == level + 2) {
            gotoxy(basketPosX + i, basketPosY); cout << "]"; // Right bracket
        } else {
            gotoxy(basketPosX + i, basketPosY); cout << "_"; // Underscore for the basket
        }
    }
}

void eraseBasket() {
    // Erase the basket
    for (int i = 0; i < level + 3; i++) { // Adjust for the current basket size
        gotoxy(basketPosX + i, basketPosY);
        cout << " "; // Erase basket
    }
}

int collision(int ind) {
    if (fruitY[ind] >= basketPosY) { // Check if fruit has touched the basket level
        if (fruitX[ind] >= basketPosX && fruitX[ind] < basketPosX + level + 2) { // Check if it's in the basket's horizontal range
            return 1; // Collision detected
        }
    }
    return 0; // No collision
}

void gameover() {
    system("cls");
    cout << endl;
    cout << "\t\t--------------------------" << endl;
    cout << "\t\t------  Try Again:( ------" << endl;
    cout << "\t\t--------------------------" << endl << endl;
    cout << "\t\tScore: " << score << endl;
    cout << "\t\tLevel: " << level << endl;
    cout << "\t\tPress any key to go back to menu.";
    getch();
}

void updateScoreAndMissedFruits() {
    gotoxy(WIN_WIDTH + 7, 6); cout << "Score: " << score << endl;
    gotoxy(WIN_WIDTH + 7, 7); cout << "Level: " << level << endl;
    gotoxy(WIN_WIDTH + 7, 8); cout << "Lives: " << missedFruits << "/" << maxMissedFruits << endl;
}

void instructions() {
    system("cls");
    cout << "              Instructions                ";
    cout << "\n---------------------------------------";
    cout << "\n Catch falling fruits using the basket.";
    cout << "\n\n Press 'a' to move left";
    cout << "\n Press 'd' to move right";
    cout << "\n Press 'p' to pause & resume";
    cout << "\n Press 'escape' to exit";
    cout << "\n\n ## Miss 5 fruits and the game is over! ##";
    cout << "\n\n Press any key to go back to menu";
    getch();
}

void initializeBasket() {
    // Set the size of the basket based on the level
    int basketSize = level + 5; // Increase size of basket as the level increases

    // Ensure the basket starts with the left bracket
    basket[0] = '[';

    // Fill the middle with underscores according to basketSize
    for (int i = 1; i <= basketSize; i++) {
        basket[i] = '_';
    }

    // Ensure the basket ends with the right bracket
    basket[basketSize + 1] = ']';

    // Null-terminate the basket string (important for display)
    basket[basketSize + 2] = '\0';
}

void increaseBasketSize() {
    // Increase the basket size when the level increases
    int newBasketSize = level + 5; // Base size + 5 for each level

    // Ensure the new basket size does not exceed the max basket size
    if (newBasketSize > MAX_BASKET_SIZE - 2) {
        newBasketSize = MAX_BASKET_SIZE - 2; // Reserve space for '[' and ']'
    }

    // Adjust the middle part of the basket with underscores
    for (int i = 1; i < newBasketSize + 1; i++) {
        basket[i] = '_';
    }

    // Keep the first and last part as the left '[' and right ']' brackets
    basket[0] = '[';
    basket[newBasketSize + 1] = ']';

    // Clear out any extra characters beyond the new basket size
    for (int i = newBasketSize + 2; i < MAX_BASKET_SIZE; i++) {
        basket[i] = ' ';
    }
}

void drawFruits() {
    for (int i = 0; i < 3; i++) {
        drawFruit(i); // Draw each fruit
    }
}

void eraseFruits() {
    for (int i = 0; i < 3; i++) {
        eraseFruit(i); // Erase each fruit
    }
}

void play() {
    basketPosX = WIN_WIDTH / 2;
    basketPosY = 23; // Initial basket position
    score = 0;
    level = 1;
    missedFruits = 0;
    bool isPaused = false; // Pause variable

    initializeBasket(); // Initialize basket

    fruitFlag[0] = true; // Start with the first fruit
    fruitFlag[1] = false;
    fruitFlag[2] = false;
    fruitY[0] = fruitY[1] = fruitY[2] = 1; // Initialize fruit positions

    system("cls");
    drawBorder();
    updateScoreAndMissedFruits();
    genFruit(0);
    genFruit(1);

    gotoxy(WIN_WIDTH + 7, 4); cout << "CATCH THE BOUNTY";
    gotoxy(WIN_WIDTH + 7, 5.8); cout << "----------------";
    gotoxy(WIN_WIDTH + 7, 11); cout << "Control";
    gotoxy(WIN_WIDTH + 7, 12.8); cout << "-------";
    gotoxy(WIN_WIDTH + 7, 13); cout << "A Key - Left ";
    gotoxy(WIN_WIDTH + 7, 14); cout << "D Key - Right";
    gotoxy(WIN_WIDTH + 7, 15); cout << "P Key - Pause";
    gotoxy(WIN_WIDTH + 7, 16); cout << "Esc - Exit   ";

    while (true) {
        if (_kbhit()) {
            char ch = _getch(); // Get the key pressed

            if (ch == 27) break; // ESC key to exit
            if (ch == 'p') { // Toggle pause
                isPaused = !isPaused; // Toggle the pause state
                if (isPaused) {
                    gotoxy(WIN_WIDTH + 7, 18); cout << "Paused... Press 'P' to resume."; // Indicate pause state
                } else {
                    gotoxy(WIN_WIDTH + 7, 18); cout << "                              "; // Clear the pause message
                }
            }

            // Move the basket only if not paused
            if (!isPaused) {
                if (ch == 'a') { // Move left
                    if (basketPosX > 0) {
                        eraseBasket(); // Erase old basket
                        basketPosX -= 2; // Move left
                        drawBasket(); // Draw new basket
                    }
                }
                if (ch == 'd') { // Move right
                    if (basketPosX + level + 2 < WIN_WIDTH) {
                        eraseBasket(); // Erase old basket
                        basketPosX += 2; // Move right
                        drawBasket(); // Draw new basket
                    }
                }
            }
        }

        if (!isPaused) { // Fruit fall logic only if not paused
            // Fruit fall logic
            for (int i = 0; i < 3; i++) {
                if (fruitFlag[i]) {
                    eraseFruit(i); // Erase old fruit
                    fruitY[i]++; // Move fruit down

                    // Check collision
                    if (collision(i) == 1) {
                        score += 10; // Update score
                        resetFruit(i); // Reset fruit position
                        updateScoreAndMissedFruits(); // Update display
                        }

                    else if (fruitY[i] >= SCREEN_HEIGHT) {
                        missedFruits++; // Increment missed fruits
                        resetFruit(i); // Reset fruit position
                        updateScoreAndMissedFruits(); // Update display

                        // Check if missed fruits exceed max limit
                        if (missedFruits >= maxMissedFruits) {
                            gameover(); // Game over
                            return; // Exit play
                        }
                    }
                }
            }

            if (score / 30 > level) {
                level++; // Increase level
                increaseBasketSize(); // Increase basket size
                updateScoreAndMissedFruits(); // Update display
            }

            drawFruits(); // Draw current fruits
            drawBasket(); // Draw basket
        }

        Sleep(100); // Control speed of falling fruits
    }
}


int main()
{
	setcursor(0,0);
	srand( (unsigned)time(NULL));

	do{
		system("cls");
		gotoxy(10,5); cout<<" -------------------------- ";
		gotoxy(10,6); cout<<" |    CATCH THE BOUNTY    | ";
		gotoxy(10,7); cout<<" --------------------------";
		gotoxy(10,9); cout<<" 1. Let's catch & collect";
		gotoxy(10,11); cout<<" 2. Instructions";
		gotoxy(10,13); cout<<" 3. Quit";
		gotoxy(10,16); cout<<" Select option: ";
		char op = getche();

		if( op=='1') play();
		else if( op=='2') instructions();
		else if( op=='3') exit(0);

	}while(1);

	return 0;
}
