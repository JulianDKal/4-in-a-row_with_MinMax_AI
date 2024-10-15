#pragma once

void copyGrid(char src[6][7], char destination[6][7]);
void printMessage(char currentPlayer);
void printBoard(char grid[6][7]);
void printHorizontalLine();
int gameIsOver(char grid[6][7]);
int placeInput(char currentPlayer, char grid[6][7], int column);
char changePlayer(char cP);
