#pragma once

struct treenode
{
	int value;
	int depth;
	struct treenode* children[7];
} typedef treeNode;

void startComputerGame(char loadGrid[6][7], int loaded, char cP);
void populateTree(treeNode* root, char grid[6][7]);
int minMax(treeNode* root);
void makeComputerMove(char grid[6][7], treeNode* root);
int initiateTree(treeNode* root);
int evaluateGrid(char grid[6][7], int columnToPlace, char currentPlayer);
int maxNode(treeNode* parent);
int minNode(treeNode* parent);