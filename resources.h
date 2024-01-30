#pragma once

void copyGrid(char src[6][7], char destination[6][7]);
void printBoard(char grid[6][7]);
void printBreakLine();
void printHorizontalLine();
int gameIsOver(char grid[6][7]);
int placeInput(char currentPlayer, char grid[6][7], char copy[6][7], int column);
char changePlayer(char cP);
int saveGameState(char grid[6][7], char gameType, char currentPlayer);
int loadGameState();
char* substring(const char* str, int start, int length);
int checkForAlNum(const char* str);