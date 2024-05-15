#pragma warning (disable: 4996)
#include "resources.h"
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include "humanGame.h"
#include <ctype.h>
#include "computerGame.h"
#include <string.h>

void copyGrid(char src[6][7], char destination[6][7])
{
	size_t size = sizeof(char) * 6 * 7;
	memcpy(destination, src, size);
}

void printBoard(char grid[6][7])
{
	system("cls");
	for (int i = 0; i < 7; i++)
	{
		printf("   %d  ", i + 1);
	}
	printf("\n");

	for (int i = 0; i < 6; i++)
	{
		printHorizontalLine();
		for (int j = 0; j < 7; j++)
		{
			printf("|  %c  ", grid[i][j]);
		}
		printf("|\n");
	}
	printHorizontalLine();
}

int placeInput(char currentPlayer, char grid[6][7], int column)
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

int gameIsOver(char grid[6][7])
{
	//vertikal
	for (int row = 0; row < 3; row++)
	{
		for (int column = 0; column < 7; column++)
		{
			if (grid[row][column] != ' ') {
				if (grid[row][column] == grid[row + 1][column]
					&& (grid[row][column] == grid[row + 2][column]) && (grid[row][column] == grid[row + 3][column]))
				{
					return 1;
				}
			}
			
		}
	}
	//horizontal
	for (int row = 0; row < 6; row++)
	{
		for (int column = 0; column < 4; column++)
		{
			if (grid[row][column] != ' ') {
				if (grid[row][column] == grid[row][column + 1]
					&& (grid[row][column] == grid[row][column + 2]) && (grid[row][column] == grid[row][column + 3]))
				{
					return 1;
				}
			}
		}
	}
	//diagonally downwards
	for (int row = 0; row < 3; row++)
	{
		for (int column = 0; column < 4; column++)
		{
			if (grid[row][column] != ' ') {
				if (grid[row][column] == grid[row + 1][column + 1]
					&& (grid[row][column] == grid[row + 2][column + 2]) && (grid[row][column] == grid[row + 3][column + 3]))
				{
					return 1;
				}
			}
		}
	}
	//diagonally upwards
	for (int row = 3; row < 6; row++)
	{
		for (int column = 0; column < 4; column++)
		{
			if (grid[row][column] != ' ') {
				if (grid[row][column] == grid[row - 1][column + 1]
					&& (grid[row][column] == grid[row - 2][column + 2]) && (grid[row][column] == grid[row - 3][column + 3]))
				{
					return 1;
				}
			}
		}
	}


	return 0;
}

char changePlayer(char cP) {
	if (cP == 'X') return 'O';
	else return 'X';
}

void printMessage(char player) {
	printf("Player %c, input a number from 1 to 7 to set, -1 to undo or -2 to save>> ", player);
}

void printHorizontalLine()
{
	printf("+");
	for (int h = 0; h < 41; h++)
	{
		printf("-");
	}
	printf("+\n");
}