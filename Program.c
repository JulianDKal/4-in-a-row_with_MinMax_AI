#include <stdio.h>
#include "computerGame.h"
#include "resources.h"
#include <string.h>


int main()
{
	printf("Welcome to 4 in a row! \n");
	char grid[6][7];
	startComputerGame(grid, 0, 'X');

	return 0;
}
