#include <stdio.h>
#include "humanGame.h"
//#include "computerGame.h"
#include "resources.h"
#include <string.h>


int main()
{
	printf("Welcome to 4 in a row! Would you like to play against another human, the computer or load a previous game?\n");
	char input;
	while (1)
	{
	printf("Please input h(human) for a new game or l(load) to load a previous game>> ");
	if (scanf_s("%c", &input, (unsigned)sizeof(char)) == 1 && (input == 'h' || input == 'c' || input == 'l')) break;
	while (fgetc(stdin) != '\n')
		;
	}
	char grid[6][7];
	if (input == 'h') startHumanGame(grid, 0, 'X');
	//else if (input == 'c') startComputerGame(grid, 0, 'X');
	else if (input == 'l') {
		loadGameState();
	}
	else {
		printf("Oh no! Something went wrong :( Please try to restart the game");
		return 1;
	}

	return 0;
}
