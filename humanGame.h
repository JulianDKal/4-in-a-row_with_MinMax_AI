#pragma once

void printMessage(char currentPlayer);
void startHumanGame(char loadedGrid[6][7], int loaded, char cP);
int Undo(int* rounds, int* backup, char grid[6][7], char backupGrid[6][7]);