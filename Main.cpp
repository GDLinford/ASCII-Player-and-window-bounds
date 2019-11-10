#include<Windows.h>

//DEFINES
#define WIDTH 150
#define HEIGHT 40

//Console Paramenters
#pragma region Console Buffer Params

//Window size Coordinate, be sure to start at zero
SMALL_RECT windowsize{ 0, 0, WIDTH - 1, HEIGHT - 1 };

// A COORD struct for specifying the console screen buffer
COORD bufferSize = { WIDTH, HEIGHT };
CONSOLE_SCREEN_BUFFER_INFO SBInfo;

// Setting up different variable for passing to WriteConsoleOutput
COORD characterBufferSize = { WIDTH, HEIGHT };
COORD charaterPosition = { 0, 0 };
SMALL_RECT ConsoleWriteArea = { 0, 0, WIDTH - 1, HEIGHT - 1 };

//A CHAR_INFO structure containing data about our frame data
CHAR_INFO consoleBuffer[WIDTH * HEIGHT];

//Initialise Handles
HANDLE wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE rHnd = GetStdHandle(STD_INPUT_HANDLE);
#pragma endregion

//GAME Constraints
const int playerCharacter = '@';
//Even though @ should be with Char not Int when the code runs it will
//automatically become the corresponding ASCII charactera &#64;
const int playerColour = 7;

const int KeyEsc = VK_ESCAPE;

//Like with @ these will all become there ASCII characters when running so these 
//can be int without worry
const int KeyW = 'W'
const int KeyA = 'A'
const int KeyS = 'S'
const int KeyD = 'D'

//GAME VARIABLE

int playerXPos = 0;
int playerYPos = 0;


int main() {
	//Set the console title
	SetConsoleTitle("Title of my console window");

	//Set screen Buffer size
	SetConsoleScreenBufferSize(wHnd, bufferSize);
	//Sets the size and position of the screen Bufer window
	SetConsoleWindowInfo(wHnd, TRUE, &windowsize);


	//This loop is of the game
	bool exitGame = false;
	//Checking to see if player has pressed escaped, if yes game exits
	while (!exitGame) 
	{
		if (GetAsyncKeyState(KeyEsc))
		{ 
			exitGame = true;
		}
		if (GetAsyncKeyState(KeyW))
		{
			--playerYPos;
		}
		if (GetAsyncKeyState(KeyA))
		{
			--playerXPos;
		}
		if (GetAsyncKeyState(KeyS))
		{
			++playerYPos;
		}
		if (GetAsyncKeyState(KeyD))
		{
			++playerXPos;
		}

		//The Character
		CHAR_INFO thisChar = consoleBuffer[playerXPos + WIDTH * playerYPos];

		//ASCII character and player colour
		thisChar.Char.AsciiChar = playerCharacter;
		thisChar.Attributes = playerColour;

		//thisChar givesd us the character then it goes into CHAR_INFO and then into the buffer


		//Override  that character in our buffer with new data
		consoleBuffer[playerXPos + WIDTH * playerYPos] = thisChar;

		WriteConsoleOutputA(wHnd, consoleBuffer, characterBufferSize, charaterPosition, &ConsoleWriteArea);
	}


	return 0;
}
