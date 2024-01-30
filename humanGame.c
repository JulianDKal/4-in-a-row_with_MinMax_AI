#pragma warning (disable: 4996)
#include "humanGame.h"
#include "resources.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

//Summary:
//Diese Datei enthält alle Funktionen und Variablen, die spezifisch und wichtig 
//für das Spiel mit zwei Menschen sind. Ich hatte auch schon damit begonnen, ein Spiel gegen den Computer
//zu implementieren, bin jedoch nicht damit fertig geworden. Die zwischen humanGame und computerGame
//geteilten Funktionen liegen in resources.c

void printMessage(char player) {
	printf("Player %c, input a number from 1 to 7 to set, -1 to undo or -2 to save>> ", player);
}

int Undo(int *rounds, int* backup, char grid[6][7], char backupGrid[6][7])
{
	if (*backup != 0) {
		printf("\nYou can only undo the last move!");
		return 0; //failure
	}
	if (*rounds == 1) {
		printf("\nNo moves made yet! Can't undo");
		return 0; //failure
	}
	(*rounds)--;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			grid[i][j] = backupGrid[i][j];
		}
	}
	*backup = 1;
	return 1; //success
}

void startHumanGame(char loadGrid[6][7], int loaded, char cP)
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
	if (loaded)
	{
		copyGrid(loadGrid, grid);
		if (currentPlayer != cP) currentPlayer = changePlayer(currentPlayer);
	}

	while (!gameIsOver(grid))
	{
		rounds++;
		int inputNumber = 0;
		printBoard(grid);
		printMessage(currentPlayer);

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

	}
	//weil nach dem setzen der Spieler geändert wird und die gameIsOver Kondition dann erst am Kopf der 
	//Schleife überprüft wird, muss hier noch einmal der Spieler geändert werden
	currentPlayer = changePlayer(currentPlayer);
	printBoard(grid);
	printf("Congratulations, Player %c! You won!", currentPlayer);

}
