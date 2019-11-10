#include<Windows.h>
#include<chrono>

//DEFINES
#define WIDTH 150
#define HEIGHT 40
#define FRAME_RATE 30

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

//TYPE DEFS

typedef std::chrono::steady_clock::time_point Time;
typedef std::chrono::high_resolution_clock HiResClock;
typedef std::chrono::duration<float> TimeDiff;

//GAME Constraints
const int playerCharacter = '@';
//Even though @ should be with Char not Int when the code runs it will
//automatically become the corresponding ASCII characters &#64;
const int playerColour = 7;

const int KeyEsc = VK_ESCAPE;

//Like with @ these will all become there ASCII characters when running so these 
//can be int without worry
const int KeyW = 'W';
const int KeyA = 'A';
const int KeyS = 'S';
const int KeyD = 'D';

//GAME VARIABLES

int playerXPos = 0;
int playerYPos = 0;
float deltaTime = 0.0f;
unsigned int frameCounter = 0;
Time previousFrameTime = HiResClock::now();

// FUNCTIONS

int Clampint(int intToClamp, int lowerLimit, int upperLimit);




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
	
		// Calculate our delta time (time since last frame)
		Time currentFrameTime = HiResClock::now();
		TimeDiff diff = currentFrameTime - previousFrameTime; //How much Time has elapsed since last frame
		deltaTime = diff.count();                             //Has enough time passed for a new frame to be drawn
		frameCounter++;

		if (deltaTime >= (1.0f / FRAME_RATE))             //(Linked to above) has enough time passed according to frame rate
		{

			//None of the code in these brackets will work unless enough time has passed since the last frame
			//Cache the timestamp of this frame
			previousFrameTime = currentFrameTime;


			//Take the input
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

			//Clamp the input to ensure player isnt able to get outside of world bounds
			playerXPos = Clampint(playerXPos, 0, (WIDTH - 1));
			playerYPos = Clampint(playerYPos, 0, (HEIGHT - 1));

			//Clear previous frames
			for (int i = 0; i < (WIDTH * HEIGHT); ++i)
			{
				consoleBuffer[i].Char.AsciiChar = 0;
				consoleBuffer[i].Attributes = 0;
				//Completely cleans the buffer window
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
	}


	return 0;
}

int Clampint(int intToClamp, int lowerLimit, int upperLimit) {
	if (intToClamp <= lowerLimit) {
		return lowerLimit;
	}

	else if (intToClamp >= upperLimit) {
		return upperLimit;
	}

	else {
		return intToClamp;
	}
}
