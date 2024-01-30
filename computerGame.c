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

	//populate the tree
	treeNode* rootNode = (treeNode*)malloc(sizeof(treeNode));
	if (!initiateTree(rootNode)) {
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
			makeComputerMove(grid, backup);
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

void makeComputerMove(char grid[6][7], char gridCopy[6][7])
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
	//populate Tree
	//evaluate Best Play
	//placeInput
}


void populateTree(treeNode* root, char currentMove[6][7])
{
	char fakeGrid[6][7];
	copyGrid(currentMove, fakeGrid);

	for (int i = 0; i < 7; i++)
	{
		
		for (int j = 0; j < 7; j++)
		{

			for (int k = 0; k < 7; k++)
			{
				root->children[i]->children[j]->children[k]->value = evaluateGrid(currentMove);
				printf("%d ", root->children[i]->children[j]->children[k]->value);
			}
		}
	}
}


//apply minmax algorithm to the populated tree
int minMax(treeNode* root)
{

}

int evaluateGrid(char grid[6][7])
{

}

int placeMockInput(char grid[6][7], int column, char currentPlayer)
{
	for (int i = 5; i >= 0; i--)
	{
		if (grid[i][column - 1] != ' ') continue;
		else {
			grid[i][column - 1] = currentPlayer;
			return 1; //success
		}
	}
	return 0; //failure, can't place sign
}

//erstellt einen Baum mit insgesamt 399 Nodes
//Diesen Baum soll die AI bei jedem ihrer Züge ausgehend von dem jetzigen Spielstand mit neuen Werten versehen
//und dann mithilfe von MinMax durch den Baum traversieren und den besten Zug berechnen
int initiateTree(treeNode* root)
{
	for (int i = 0; i < 7; i++)
	{
		treeNode* newNode = (treeNode*)malloc(sizeof(treeNode));
		if (newNode == NULL) {
			printf("Fehler beim Erstellen des Baums");
			return 0; //Fehler
		}
		newNode->value = 0;
		root->children[i] = newNode;
		for (int j = 0; j < 7; j++)
		{
			treeNode* newNewNode = (treeNode*)malloc(sizeof(treeNode));
			if (newNewNode == NULL) {
				printf("Fehler beim Erstellen des Baums");
				return 0;
			}
			newNewNode->value = 0;
			newNode->children[j] = newNewNode;

			for (int k = 0; k < 7; k++)
			{
				treeNode* newNewNewNode = (treeNode*)malloc(sizeof(treeNode));
				if (newNewNewNode == NULL) {
					printf("Fehler beim Erstellen des Baums");
					return 0;
				}
				newNewNewNode->value = 0;
				newNewNode->children[k] = newNewNewNode;
			}
		}
	}

	return 1; //success
}