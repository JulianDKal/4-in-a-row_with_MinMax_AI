#include "computerGame.h"
#include "resources.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

#define MAX_DEPTH 3
char emptyBoard[6][7] = {
	{' ', ' ', ' ', ' ', ' ', ' ', ' '},
	{' ', ' ', ' ', ' ', ' ', ' ', ' '},
	{' ', ' ', ' ', ' ', ' ', ' ', ' '},
	{' ', ' ', ' ', ' ', ' ', ' ', ' '},
	{' ', ' ', ' ', ' ', ' ', ' ', ' '},
	{' ', ' ', ' ', ' ', ' ', ' ', ' '}
};
char exampleBoard1[6][7] = {
	{' ', ' ', ' ', ' ', ' ', ' ', ' '},
	{' ', ' ', ' ', ' ', ' ', ' ', ' '},
	{' ', ' ', ' ', ' ', ' ', ' ', ' '},
	{' ', ' ', ' ', ' ', ' ', ' ', ' '},
	{' ', ' ', 'X', 'X', ' ', ' ', ' '},
	{' ', 'X', 'X', 'O', 'O', 'O', ' '}
};
char exampleBoard2[6][7] = {
	{' ', ' ', ' ', ' ', ' ', ' ', ' '},
	{' ', ' ', ' ', ' ', ' ', ' ', ' '},
	{' ', ' ', ' ', ' ', 'O', ' ', ' '},
	{' ', ' ', ' ', 'X', 'O', ' ', ' '},
	{' ', 'O', 'O', 'X', 'X', ' ', ' '},
	{' ', 'X', 'X', 'O', 'O', 'O', 'X'}
};
char exampleBoard3[6][7] = {
	{' ', ' ', ' ', ' ', ' ', ' ', ' '},
	{' ', ' ', ' ', ' ', ' ', ' ', ' '},
	{'O', 'X', ' ', ' ', ' ', ' ', ' '},
	{'X', 'O', ' ', ' ', ' ', ' ', ' '},
	{'O', 'X', ' ', ' ', ' ', ' ', ' '},
	{'X', 'O', 'X', 'O', 'X', ' ', ' '}
};


void startComputerGame(char loadGrid[6][7], int loaded, char cP)
{
	int rounds = 0;

	char currentPlayer = 'X';
	char grid[6][7] = { 0 };

	for (int i = 0; i < 7; i++)
	{
		printf("%d ", evaluateGrid(exampleBoard1, i, 'O'));
		printf("\n");
	}
	printf("\n");
	for (int i = 0; i < 7; i++)
	{
		printf("%d ", evaluateGrid(exampleBoard2, i, 'O'));
		printf("\n");
	}
	printf("\n");
	for (int i = 0; i < 7; i++)
	{
		printf("%d ", evaluateGrid(exampleBoard3, i, 'O'));
		printf("\n");
	}

	//fill up the grids
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (grid[i][j] == 0) {
				grid[i][j] = ' ';
			}
		}
	}
	char gridCopy[6][7];
	copyGrid(grid, gridCopy);

	//initiate the tree, creating the tree structure in the first place
	treeNode* rootNode = (treeNode*)malloc(sizeof(treeNode));
	if (rootNode == NULL) {
		printf("error creating the root node");
		return;
	}
	rootNode->depth = 0;
	if (!initiateTree(rootNode)) {
		printf("Something went wrong with creating the AI tree! Please try to restart the game");
		return;
	};

	while (!gameIsOver(grid))
	{
		int inputNumber = 0;
		printBoard(grid);
		printMessage(currentPlayer);

		if (currentPlayer == 'X') {
		#pragma region Human Move Code
			while (1)
			{
				if (scanf_s("%d", &inputNumber) == 1 && (inputNumber >= 1 && inputNumber <= 7)) break;
				while (fgetc(stdin) != '\n')
					;

				printBoard(grid);
				printf("Can't take this input. Please try again >> ");
			}

			if (placeInput(currentPlayer, grid, inputNumber) == 1) { //successful
				currentPlayer = changePlayer(currentPlayer);
				rounds++;
			}
			else
			{
				printf("This column is full. Please try again");
				Sleep(1000);
			}

			//The board is full
			if (rounds == 42) {
				printf("\nDraw! The game is now over :) ");
				return;
			}

			#pragma endregion Human Move Code
		}
		else 
		{
			makeComputerMove(grid, rootNode);
			currentPlayer = changePlayer(currentPlayer);
		}
	}
	//currentPlayer has to get changed back, because it already gets changed after the winning move
	currentPlayer = changePlayer(currentPlayer);
	if (currentPlayer == 'X') {
		printf("\nGood Job! You won against the computer :)");
	}
	else {
		printf("\nOh no, the computer won :(");
	}
}

void makeComputerMove(char grid[6][7], treeNode* root)
{
	Sleep(500);
	char currentPlayer = 'O';
	populateTree(root, grid);
	int move = minMax(root);
	placeInput('O', grid, move + 1);
}

//fill up the outer nodes (leaves) of the tree
void populateTree(treeNode* root, char currentState[6][7])
{
	char fakeGrid[6][7];
	copyGrid(currentState, fakeGrid);

	for (int i = 0; i < 7; i++)
	{
		copyGrid(currentState, fakeGrid); //set the fake grid to its original form
		placeInput('O', fakeGrid, i);
		
		for (int j = 0; j < 7; j++)
		{
			placeInput('X', fakeGrid, j);
			for (int k = 0; k < 7; k++)
			{
				root->children[i]->children[j]->children[k]->value = evaluateGrid(fakeGrid,k, 'O');
			}
		}
	}
	//printTree(root);
}

//apply minmax algorithm to the populated tree
int minMax(treeNode* root)
{
	if (root->depth < (MAX_DEPTH - 1)) {
		for (int i = 0; i < 7; i++)
		{
			return minMax(root->children[i]);
		}
	}
	else {
		if (root->depth % 2 == 0) { //computer move, maximizing
			return maxNode(root);
		}
		else return minNode(root); //human move, minimizing
	}

}

//TODO - don't always count all 4 entries for every possibility
//TODO - maybe reduce recurrence after each possibility
//TODO - improve evaluationOfFour algorithm
//TODO - improve the gameOver algorithm so it only counts the current piece

int evaluateGrid(char grid[6][7], int columnToPlace, char cP)
{
	//determine the row in which the new piece is placed
	int rowToPlace = -1;
	for (int i = 5; i >= 0; i--)
	{
		if (grid[i][columnToPlace] == ' ') {
			rowToPlace = i;
			break;
		}
	}

	if (rowToPlace == -1) {
		printf("grid couldn't be evaluated");
		return -1000;
	}
	grid[rowToPlace][columnToPlace] = cP;
	
	//starting from the current piece, analyze in all directions the consequences of the move
	int xCount = 0;
	int oCount = 0;
	int emptyCount = 0;
	int result = 0;
	int overStepped = 0;

	//vertical
	for (int i = 0; i < 4; i++)
	{
		if (rowToPlace + i <= 5)
		{
			if (grid[rowToPlace + i][columnToPlace] == 'X') xCount++;
			else if (grid[rowToPlace + i][columnToPlace] == 'O') oCount++;
		}
		else {
			overStepped = 1;
			break;
		}
	}

	if (!overStepped) {
		emptyCount = 4 - (xCount + oCount);
		result += evaluationOfFour(xCount, oCount, emptyCount);
	}
		xCount = 0;
		oCount = 0;
	
	overStepped = 0;

	//horizontal right
	for (int i = 0; i < 4; i++)
	{
		if (columnToPlace + i <= 6)
		{
			if (grid[rowToPlace][columnToPlace + i] == 'X') xCount++;
			else if (grid[rowToPlace][columnToPlace + i] == 'O') oCount++;
		}
		else {
			overStepped = 1;
			break;
		}
	}
	if (!overStepped) {
		emptyCount = 4 - (xCount + oCount);
		result += evaluationOfFour(xCount, oCount, emptyCount);
	}
		xCount = 0;
		oCount = 0;
	
	overStepped = 0;
	//horizontal left
	for (int i = 0; i < 4; i++)
	{
		if ((columnToPlace - i) >= 0)
		{
			if (grid[rowToPlace][columnToPlace - i] == 'X') xCount++;
			else if (grid[rowToPlace][columnToPlace - i] == 'O') oCount++;
		}
		else {
			overStepped = 1;
			break;
		}
	}
	if (!overStepped) {
		emptyCount = 4 - (xCount + oCount);
		result += evaluationOfFour(xCount, oCount, emptyCount);
	}
		xCount = 0;
		oCount = 0;
	overStepped = 0;
	//diagonally left
	for (int i = 0; i < 4; i++)
	{
		if (columnToPlace - i >= 0 && rowToPlace + i <= 5)
		{
			if (grid[rowToPlace + i][columnToPlace - i] == 'X') xCount++;
			else if (grid[rowToPlace + i][columnToPlace - i] == 'O') oCount++;
		}
		else {
			overStepped = 1;
			break;
		}
	}
	if (!overStepped) {
		emptyCount = 4 - (xCount + oCount);
		result += evaluationOfFour(xCount, oCount, emptyCount);
	}
		xCount = 0;
		oCount = 0;
	
	overStepped = 0;
	//diagonally right
	for (int i = 0; i < 4; i++)
	{
		if (columnToPlace + i <= 6 && rowToPlace + i <= 5)
		{
			if (grid[rowToPlace + i][columnToPlace + i] == 'X') xCount++;
			else if (grid[rowToPlace + i][columnToPlace + i] == 'O') oCount++;
		}
		else {
			overStepped = 1;
			break;
		}
	}
	if (!overStepped) {
		emptyCount = 4 - (xCount + oCount);
		result += evaluationOfFour(xCount, oCount, emptyCount);
	}

	grid[rowToPlace][columnToPlace] = ' ';
	return result;
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
	else if (oCount == 1 && xCount >= 2) result += 7;

	return result;
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
		newNode->depth = 1;
		root->children[i] = newNode;
		for (int j = 0; j < 7; j++)
		{
			treeNode* newNewNode = (treeNode*)malloc(sizeof(treeNode));
			if (newNewNode == NULL) {
				printf("error creating the tree for the ai");
				return 0;
			}
			newNewNode->value = 0;
			newNewNode->depth = 2;
			newNode->children[j] = newNewNode;

			for (int k = 0; k < 7; k++)
			{
				treeNode* newNewNewNode = (treeNode*)malloc(sizeof(treeNode));
				if (newNewNewNode == NULL) {
					printf("error creating the tree for the ai");
					return 0;
				}
				newNewNewNode->value = 0;
				newNewNewNode->depth = 3;
				newNewNode->children[k] = newNewNewNode;
			}
		}
	}

	return 1; //success
}

//TODO - improve this so that it returns a random node if several options are equal
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

void printTree(treeNode* root)
{
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			printf("Nodes of %d | %d\n", i, j);
			for (int k = 0; k < 7; k++)
			{
				printf("%d ", root->children[i]->children[j]->children[k]->value);
			}
			printf("\n");
		}
	}
}
