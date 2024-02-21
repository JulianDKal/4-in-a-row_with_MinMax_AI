#include "computerGame.h"
#include "resources.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void startComputerGame(char loadGrid[6][7], int loaded, char cP)
{
	int backupped = 0;
	int* backuppedPtr = &backupped;
	int rounds = 0;
	int* roundsPtr = &rounds;

	char currentPlayer = 'X';
	char grid[6][7] = { 0 };
	char backup[6][7] = { 0 };
	//fill up the grids
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (grid[i][j] == 0) {
				grid[i][j] = ' ';
				backup[i][j] = ' ';
			}
		}
	}
	char gridCopy[6][7];
	copyGrid(grid, gridCopy);

	if (loaded)
	{
		copyGrid(loadGrid, grid);
		if (currentPlayer != cP) currentPlayer = changePlayer(currentPlayer);
	}

	//initiate the tree, creating the tree structure in the first place
	treeNode* rootNode = (treeNode*)malloc(sizeof(treeNode));
	if (!initiateTree(rootNode)) {
		printf("Something went wrong with creating the AI tree! Please try to restart the game");
		return;
	};

	while (!gameIsOver(grid))
	{
		rounds++;
		int inputNumber = 0;
		printBoard(grid);
		printMessage(currentPlayer);

		if (currentPlayer == 'X') {
		#pragma region Human Move Code
			while (1)
			{
				if (scanf_s("%d", &inputNumber) == 1 && ((inputNumber >= 1 && inputNumber <= 7) || inputNumber == -1 || inputNumber == -2)) break;
				while (fgetc(stdin) != '\n')
					;

				printBoard(grid);
				printf("Can't take this input. Please try again >> ");
			}
			//undo option
			if (inputNumber == -1) {
				if (Undo(roundsPtr, backuppedPtr, grid, backup) == 1) //Undo fehlgeschlagen
				{
					currentPlayer = changePlayer(currentPlayer);
					continue;
				}

				else {
					rounds--;
					Sleep(1000);
					continue;
				}
			}
			//save option
			if (inputNumber == -2) {
				if (saveGameState(grid, 'h', currentPlayer) == 0) {
					rounds--;
					continue;
				}
				else {
					printf("Fehler beim Speichern des Spiels!");
					return;
				}
			}

			if (placeInput(currentPlayer, grid, backup, inputNumber) == 1) { //successful
				currentPlayer = changePlayer(currentPlayer);
				backupped = 0; //reset the backup checker
			}
			else
			{
				rounds--;
				printf("This column is full. Please try again");
				Sleep(1000);
			}

			//Das Spielfeld ist voll
			if (rounds == 42) {
				printf("\nDraw! Congratulations to you both :) ");
				return;
			}

			#pragma endregion
		}
		else 
		{
			makeComputerMove(grid, backup, rootNode);
			currentPlayer = changePlayer(currentPlayer);
		}
	}
	currentPlayer = changePlayer(currentPlayer);
	if (currentPlayer == 'X') {
		printf("\nGood Job! You won against the computer :)");
	}
	else {
		printf("\nOh no, the computer won :(");
	}
}

void makeComputerMove(char grid[6][7], char gridCopy[6][7], treeNode* root)
{
	Sleep(500);
	char currentPlayer = 'O';
	srand((unsigned int)time(NULL));
	while (1)
	{
		int randomNumber = rand() % 7 + 1;
		if (placeInput(currentPlayer, grid, gridCopy, randomNumber) != 0){
			break;
		}
	}
	populateTree(root, gridCopy);
	//evaluate Best Play
	//placeInput
}


void populateTree(treeNode* root, char currentState[6][7])
{
	char fakeGrid[6][7];
	copyGrid(currentState, fakeGrid);
	int count = 0;

	for (int i = 0; i < 7; i++)
	{
		copyGrid(currentState, fakeGrid); //set the fake grid to its original form
		placeMockInput(fakeGrid, i, 'O');

		for (int j = 0; j < 7; j++)
		{
			placeMockInput(fakeGrid, i, 'X');
			for (int k = 0; k < 7; k++)
			{
				placeMockInput(fakeGrid, i, 'O');
				count++;
				root->children[i]->children[j]->children[k]->value = evaluateGrid(fakeGrid, i);
				printf("%d ", /*root->children[i]->children[j]->children[k]->value*/count);
			}
		}
	}
}


//apply minmax algorithm to the populated tree
int minMax(treeNode* root)
{

}

int evaluateGrid(char grid[6][7], int columnToPlace)
{
	//determine the row in which the new piece is placed
	int rowToPlace = -1;
	for (int i = 5; i >= 0; i--)
	{
		if (grid[i][columnToPlace] == ' ') rowToPlace = i;
	}

	if (rowToPlace == -1) {
		printf("grid couldn't be evaluated");
		return -1000;
	}
	
	//starting from the current piece, analyze in all directions the consequences of the move
	int xCount = 0;
	int oCount = 0;
	int emptyCount = 0;
	int result = 0;

	//vertical
	for (int i = 0; i < 4; i++)
	{
		if (rowToPlace + i <= 5)
		{
			if (grid[rowToPlace + i][columnToPlace] == 'X') xCount++;
			else if (grid[rowToPlace + i][columnToPlace] == 'O') oCount++;
		}
	}
	emptyCount = 4 - (xCount + oCount);
	result += evaluationOfFour(xCount, oCount, emptyCount);
	
	//horizontal right
	for (int i = 0; i < 4; i++)
	{
		if (columnToPlace + i <= 6)
		{
			if (grid[rowToPlace][columnToPlace + i] == 'X') xCount++;
			else if (grid[rowToPlace][columnToPlace + i] == 'O') oCount++;
		}
	}
	//horizontal left
	for (int i = 0; i < 4; i++)
	{
		if (columnToPlace + i >= 0)
		{
			if (grid[rowToPlace][columnToPlace - i] == 'X') xCount++;
			else if (grid[rowToPlace][columnToPlace - i] == 'O') oCount++;
		}
	}

	//diagonally left

	//diagonally right

	return 1;
}

int evaluationOfFour(int xCount, int oCount, int emptyCount)
{
	int result = 0;
	if (oCount == 4) {
		result = 500;
		return result;
	}
	else if (oCount == 3 && emptyCount == 1) result += 5;
	else if (oCount == 2 && emptyCount == 2) result += 3;
	else if (oCount == 2 && emptyCount <= 1) result += 1;
	else if (oCount == 1 && xCount >= 2) result += 5;

	return result;
}

//if possible, places an input on the copy of the current grid
int placeMockInput(char grid[6][7], int column, char currentPlayer)
{
	for (int i = 5; i >= 0; i--)
	{
		if (grid[i][column] != ' ') continue;
		else {
			grid[i][column] = currentPlayer;
			return 1; //success
		}
	}
	return 0; //failure, can't place sign
}

//creates a tree with a total of 399 nodes, only called once
int initiateTree(treeNode* root)
{
	for (int i = 0; i < 7; i++)
	{
		treeNode* newNode = (treeNode*)malloc(sizeof(treeNode));
		if (newNode == NULL) {
			printf("error creating the tree for the ai");
			return 0; //error
		}
		newNode->value = 0;
		root->children[i] = newNode;
		for (int j = 0; j < 7; j++)
		{
			treeNode* newNewNode = (treeNode*)malloc(sizeof(treeNode));
			if (newNewNode == NULL) {
				printf("error creating the tree for the ai");
				return 0;
			}
			newNewNode->value = 0;
			newNode->children[j] = newNewNode;

			for (int k = 0; k < 7; k++)
			{
				treeNode* newNewNewNode = (treeNode*)malloc(sizeof(treeNode));
				if (newNewNewNode == NULL) {
					printf("error creating the tree for the ai");
					return 0;
				}
				newNewNewNode->value = 0;
				newNewNode->children[k] = newNewNewNode;
			}
		}
	}

	return 1; //success
}


//improve this so that it returns a random node if several options are equal
int maxNode(treeNode* parent)
{
	int maxNode = 0;
	int maxValue = -10000;
	for (int i = 0; i < 7; i++)
	{
		if (parent->children[i]->value > maxValue) {
			maxValue = parent->children[i]->value;
			maxNode = i;
		}
	}
	return maxNode;
}

int minNode(treeNode* parent)
{
	int minNode = 0;
	int minValue = 10000;
	for (int i = 0; i < 7; i++)
	{
		if (parent->children[i]->value < minValue) {
			minValue = parent->children[i]->value;
			minNode = i;
		}
	}
	return minNode;
}