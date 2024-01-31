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

int saveGameState(char grid[6][7], char gameType, char currentPlayer)
{
	FILE* fp;
	char dateiName[70];
	printf("\nPlease input a name for the save file>> ");

	while (1)
	{
		if (scanf("%s", dateiName) == 1 && strlen(dateiName) >= 5 ) 
		{
			//create substrings for the start and end of the string
			char* subEnd = substring(dateiName, strlen(dateiName) - 4, 4);
			char* subStart = substring(dateiName, 0, strlen(dateiName) - 4);
			//printf("%s\n", subEnd);
			if (subEnd != NULL && strcmp(subEnd, ".txt") == 0 && subStart != NULL && checkForAlNum(subStart)) {
				break;
			}

		}

		while (fgetc(stdin) != '\n')
			;
			
		printf("\nInvalid. The input must look like this: xxxx.txt (xxxx can have an arbitrary length)");
		printf("\nPlease input a name for the save file>> ");
	}

	if ((fp = fopen(dateiName, "w+t")) == NULL) {
		return 1;
	}

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			fputc(grid[i][j], fp);
		}
		fputs("\n", fp);
	}
	fputc(gameType, fp);
	fputc(currentPlayer, fp);
	fclose(fp);
	return 0; //success
}

char* substring(const char* str, int start, int length)
{
	if (str == NULL || (start + length) > strlen(str)) return NULL;
	char* sub = (char*)malloc(length + 1);

	//str ab dem in start spezifizierten Eintrag in sub kopieren
	if(sub != NULL ) strncpy(sub, str + start, length);
	else {
		printf("Fehler beim Kopieren des substrings");
		return NULL;
	}
	sub[length] = '\0';

	return sub;
}

int checkForAlNum(const char* str)
{
	for (int i = 0; i < strlen(str); i++)
	{
		if (!isalnum(str[i])) return 0; //Fehler
	}

	//Erfolg, alle Charaktere im string sind alphanumerisch
	return 1;
}

int loadGameState()
{
	FILE* fp;
	char dateiName[50];
	printf("\nPlease input the name of the save file>> ");

	while (1)
	{
		if (scanf("%s", dateiName) == 1) {
			//Erfolg wenn die Datei im Verzeichnis existiert
			if (_access(dateiName, 0) == 0) break;
		}

		while (fgetc(stdin) != '\n')
			;

		printf("This save file doesn't exist. Please try again>> ");
	}

	if ((fp = fopen(dateiName, "r+")) == NULL) {
		printf("fopen error");
		return 1;
	}
	char loadedGrid[6][7] = { 0 };
	char c;
	if (fp == NULL) printf("Fopen error!!");

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 8; j++) //j < 8, da am Ende der Zeile immer der newline character eingelesen wird
		{
			c = fgetc(fp);
			if (c == '\n') continue;
			else loadedGrid[i][j] = c;
		}
	}
	//read in the save file character by character and save it in grid
	char gT = fgetc(fp);
	char cP = fgetc(fp);

	if (!isalpha(gT) || !isalpha(cP))
	{
		printf("Fehler beim Laden der Datei");
		return 1; //Fehler
	}

	fclose(fp);

	if (gT == 'h') startHumanGame(loadedGrid, 1, cP);
	else if (gT == 'c') startComputerGame(loadedGrid, 1, cP);

	return 0; //success
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

void printBreakLine() {
	for (int i = 0; i < 50; i++)
	{
		printf("-");
	}
	printf("\n");
}

int placeInput(char currentPlayer, char grid[6][7], char copy[6][7], int column)
{
	for (int i = 5; i >= 0; i--)
	{
		if (grid[i][column - 1] != ' ') continue;
		else {
			copyGrid(grid, copy);
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
	//schräg nach unten
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
	//schräg nach oben
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