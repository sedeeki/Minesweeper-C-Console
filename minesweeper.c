// Assignment 1 20T1 COMP1511: Minesweeper
// minesweeper.c
//
// This program was written by YOUR-NAME-HERE (z5555555)
// on INSERT-DATE-HERE
//
// Version 1.0.0 (2020-03-08): Assignment released.
// Version 1.0.1 (2020-03-08): Fix punctuation in comment.
// Version 1.0.2 (2020-03-08): Fix second line of header comment to say minesweeper.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Possible square states.
#define VISIBLE_SAFE    0
#define HIDDEN_SAFE     1
#define HIDDEN_MINE     2

// The size of the starting grid.
#define SIZE 8

// The possible command codes.
#define DETECT_ROW              1
#define DETECT_COL              2
#define DETECT_SQUARE           3
#define REVEAL_SQUARE           4
#define GAMEPLAY_MODE           5
#define DEBUG_MODE              6
#define REVEAL_RADIAL           7

// Add any extra #defines here.
#define HINTS	3

void initialise_field(int minefield[SIZE][SIZE]);
void print_debug_minefield(int minefield[SIZE][SIZE]);
void detect(int axis, int axisNumber, int minefield[SIZE][SIZE]);
void detectBox(int row, int column, int size, int minefield[SIZE][SIZE]);
int reveal(int row, int column, int minefield[SIZE][SIZE], int first);
void gameplayMode(int minefield[SIZE][SIZE], int lose);
int revealRadial(int row, int column, int minefield[SIZE][SIZE], int first);
int checkBox(int row, int column, int minefield[SIZE][SIZE]);
void checkDiagonal(int row, int column, int minefield[SIZE][SIZE], int direction);
void checkHorizontal(int row, int column, int minefield[SIZE][SIZE]);
void checkVertical(int row, int column, int minefield[SIZE][SIZE]);
void dropDown(int minefield[SIZE][SIZE]);
// Place your function prototyes here.


int hints = HINTS;
// My global variable for hints.

int main(void) {
	int mode = 0;
	int minefield[SIZE][SIZE];
     initialise_field(minefield);
     printf("Welcome to minesweeper!\n");
     int numberOfMines;
     printf("How many mines? ");
     scanf("%d",&numberOfMines);
     // TODO: Scan in the number of pairs of mines.

     printf("Enter pairs:\n");
     int i;
     int coordX;
     int coordY;
     for (i = 0; i < numberOfMines; i++)
     {
		scanf("%d %d",&coordX,&coordY);
		if (coordX < SIZE && coordY < SIZE && coordX > -1 && coordY > -1)
			minefield[coordX][coordY] = HIDDEN_MINE;
     }
     // TODO: Scan in the pairs of mines and place them on the grid.

     printf("Game Started\n");
     print_debug_minefield(minefield);
     
     
     
     // TODO: Scan in commands to play the game until the game ends.
     // A game ends when the player wins, loses, or enters EOF (Ctrl+D).
     // You should display the minefield after each command has been processed.
     char buff[10] = {0};
     int exit = 0;
     int lose = 0;
     int first = 1;
     while (exit == 0)
     {
         buff[0] = '0';
         //printf("%d\n",exit);
	     char c = 'x';
	     scanf("%s", buff);
	     int index = 0;
	     while (buff[index] != '\0' && index < 10)
	     	index++;
	     //printf("Length: %d\n",index);
	     while (c != '\n' && index < 10)
	     {
	     	scanf("%c",&c);
	     	buff[index++] = c;
		}
		buff[index - 1] = 0;
		//printf("%s",buff);
		if (atoi(&buff[0]) == 0)
			exit = 1;
		else
		{
			if (atoi(&buff[0]) < 3)
				detect(atoi(&buff[0]),atoi(&buff[2]),minefield);
			else if (atoi(&buff[0]) == 3)
				detectBox(atoi(&buff[2]),atoi(&buff[4]),atoi(&buff[6]),minefield);
			else if(atoi(&buff[0]) == 4)
			{
				lose = reveal(atoi(&buff[2]),atoi(&buff[4]),minefield,first);
				first = 0;
			}
			else if(atoi(&buff[0]) == 5)
			{
				mode = 1;
				printf("Gameplay Mode Activated\n");
			}
			else if(atoi(&buff[0]) == 6)
			{
				mode = 0;
				printf("Debug Mode Activated\n");
			}
			else if(atoi(&buff[0]) == 7)
			{
				lose = revealRadial(atoi(&buff[2]),atoi(&buff[4]),minefield,first);
				first = 0;
			}
			if (mode == 0)	
				print_debug_minefield(minefield);
			else if (mode == 1)
				gameplayMode(minefield,lose);
			if (lose == 1)
				exit = 1;
		}
     }
     return 0;
}

// Set the entire minefield to HIDDEN_SAFE.
void initialise_field(int minefield[SIZE][SIZE]) 
{
     int i = 0;
     while (i < SIZE) {
     	int j = 0;
        	while (j < SIZE) {
            	minefield[i][j] = HIDDEN_SAFE;
            	j++;
     	}
        	i++;
    }
}

// Print out the actual values of the minefield.
void print_debug_minefield(int minefield[SIZE][SIZE]) 
{
     int i = 0;
     while (i < SIZE) {
        	int j = 0;
        	while (j < SIZE) {
            	printf("%d ", minefield[i][j]);
            	j++;
        	}
        	printf("\n");
        	i++;
     }
}


// Detect mines in Row or Column.
void detect(int axis, int axisNumber, int minefield[SIZE][SIZE])
{
	if (hints == 0)
	{
		printf("Help already used\n");
		return;
	}
	hints--;
	int i;
	int mines = 0;
	//printf("%d %d",axis,axisNumber);
	if (axis == 1)
	{
		for (i = 0; i < SIZE; i++)
			if (minefield[axisNumber][i] == HIDDEN_MINE)
				mines++;
		printf("There are %d mine(s) in row %d\n", mines, axisNumber);
	}
	else if (axis == 2)
	{
		for (i = 0; i < SIZE; i++)
			if (minefield[i][axisNumber] == HIDDEN_MINE)
				mines++;
		printf("There are %d mine(s) in column %d\n", mines, axisNumber);	
	}
}


// detecting mines in adjacent places of some x,y coordinate

void detectBox(int row, int column, int size, int minefield[SIZE][SIZE])
{
	if (hints == 0)
	{
		printf("Help already used");
		return;
	}
	hints--;
	//printf("Values: %d %d %d",row,column,size);
	size = size/2;
	int x = row - size;
	int y = column - size;
	int i;
	int j;
	int mines = 0;
	for (i = x; i < x + 2*size + 1; i++)
		for (j = y; j < x + 2*size + 1; j++)
			if (minefield[i][j] == HIDDEN_MINE)
				mines++;
	size = 2*size + 1;
	printf("There are %d mine(s) in the square centered at row %d, column %d of size %d\n", mines, row, column, size);		
}


void dropDown(int minefield[SIZE][SIZE])
{
	int temp[SIZE] = {0};
	int i = SIZE - 1;
	int j;
	
	for (j = 0; j < SIZE; j++)
		temp[j] = minefield[i][j];
	for (i = SIZE - 1; i > 0; i--)
		for (j = 0; j < SIZE; j++)
			 minefield[i][j] = minefield[i-1][j];
	for (j = 0; j < SIZE; j++)
		minefield[0][j] = temp[j];
}


// reveal function for playing the game

int reveal(int row, int column, int minefield[SIZE][SIZE], int first)
{
     if (row >= SIZE || row < 0 || column < 0 || column >= SIZE)
     	return 0;
	int x = row - 1;
	int y = column - 1;
	int i;
	int j;
	int mines = 0;
	if (minefield[row][column] == HIDDEN_MINE)
	{	
		if (first == 0)
		{
			printf("Game over\n");
			return 1;
		}
		else
		{
			while(minefield[row][column] == HIDDEN_MINE)
				dropDown(minefield);
			minefield[row][column] = 0;
		}
	}
	for (i = x; i < x + 3; i++)
		for (j = y; j < y + 3; j++)
		    if (i > -1 && i < SIZE && j > -1 && j < SIZE)
			    if (minefield[i][j] == HIDDEN_MINE)
				    mines++;
	if (mines == 0)
	{
		for (i = x; i < x + 3; i++)
			for (j = y; j < y + 3; j++)
				if (i < SIZE && j < SIZE && i > -1 && j > -1)
					minefield[i][j] = VISIBLE_SAFE;
	}
	else
		minefield[row][column] = 0;
	//printf("\n");
	int remaining = 0;
	for (i = 0; i < SIZE; i++)
		for (j = 0; j < SIZE; j++)
			if(minefield[i][j] == 1)
				remaining++;
	if (remaining == 0)
	{
		printf("Game Won!\n");
		return 2;
	}
	return 0;
}

// Print out the gameplay values of the minefield.
void gameplayMode(int minefield[SIZE][SIZE], int lose) 
{
	if (lose != 1)
	{
		printf("..\n");
		printf("\\/\n");
	}
	else
	{
		printf("xx\n");
		printf("/\\\n");
	}
     int i = 0;
     int x;
     int y;
     int mines = 0;
     while (i < SIZE) 
	{
		if (i == 0)
		{
			printf("    ");
			int k = 0;
			for (k = 0; k < SIZE; k++)
				printf("0%d ",k);
			printf("\n");
			printf("   ");
			printf("-------------------------\n");
		}
        	int j = 0;
        	printf("0%d |",i);
        	while (j < SIZE) {
        	    mines = 0;
        		if (lose != 1)
        		{
	        		if (minefield[i][j] == HIDDEN_SAFE || minefield[i][j] == HIDDEN_MINE)
	            		printf("## ");
	            	else if (minefield[i][j] == VISIBLE_SAFE)
	            	{
	            		for (x = i-1; x < i + 2; x++)
						    for (y = j-1; y < j + 2; y++)
						        if (x > -1 && x < SIZE && y > -1 && y < SIZE)
						        	if (minefield[x][y] == 2)
						        		mines++;
						
					if (mines == 0)
						printf("   ");
					else
						printf("0%d ",mines);
				}
				//printf("|");
	            	j++;
	     	    }
	     	else
	     	{
	     		if (minefield[i][j] == HIDDEN_SAFE)
	            		printf("## ");
	            	else if (minefield[i][j] == HIDDEN_MINE)
	            		printf("() ");
		       	else if (minefield[i][j] == VISIBLE_SAFE)
	            	{
	            		for (x = i - 1; x < i + 2; x++)
	            		{
						    for (y = j - 1; y < j + 2; y++)
						        if (x > -1 && x < SIZE && y > -1 && y < SIZE)
						        	if (minefield[x][y] == HIDDEN_MINE)
						        		mines++;
	                    }
					if (mines == 0)
						printf("   ");
					else
						printf("0%d ",mines);
				}
				//printf("|");
	            	j++;
	            	
			}
     
        	
        	//i++;
     }
     printf("|");
        printf("\n");
     i++;
     
}
printf("   -------------------------\n");
}

int checkBox(int row, int column, int minefield[SIZE][SIZE])
{
	int x = row - 1;
	int y = column - 1;
	int i;
	int j;
	int mines = 0;
	for (i = x; i < x + 3; i++)
		for (j = y; j < y + 3; j++)
			if (i > -1 && i < SIZE && j > -1 && j < SIZE)
				if (minefield[i][j] == HIDDEN_MINE)
					mines++;
	return mines;
}

void checkDiagonal(int row, int column, int minefield[SIZE][SIZE], int direction)
{
	int i;
	int j;
	if (direction == 0)
	{
		i = row;
		j = column;
		int check = 0;
		while(i > -1 && j > -1 && check == 0)
		{
			check = checkBox(i,j,minefield);
			minefield[i][j] = VISIBLE_SAFE; 
			i--;
			j--;
		}
	}
	else if (direction == 1)
	{
		i = row;
		j = column;
		int check = 0;
		while(i > -1 && j < SIZE && check == 0)
		{
			check = checkBox(i,j,minefield);
			minefield[i][j] = VISIBLE_SAFE; 
			i--;
			j++;
		}
	}
	else if (direction == 2)
	{
		i = row;
		j = column;
		int check = 0;
		while(i < SIZE && j < SIZE && check == 0)
		{
			check = checkBox(i,j,minefield);
			minefield[i][j] = VISIBLE_SAFE; 
			i++;
			j++;
		}
	}
	else if (direction == 3)
	{
		i = row;
		j = column;
		int check = 0;
		while(i < SIZE && j > -1 && check == 0)
		{
			check = checkBox(i,j,minefield);
			minefield[i][j] = VISIBLE_SAFE; 
			i++;
			j--;
		}
	}
}

void checkVertical(int row, int column, int minefield[SIZE][SIZE])
{
	int i = row;
	int check = 0;
	while (i > -1 && check == 0)
	{
		check = checkBox(i,column,minefield);
		minefield[i][column] = VISIBLE_SAFE;
		i--;
	}
	i = row;
	check = 0;
	while (i < SIZE && check == 0)
	{
		check = checkBox(i,column,minefield);
		minefield[i][column] = VISIBLE_SAFE;
		i++;
	}
}

void checkHorizontal(int row, int column, int minefield[SIZE][SIZE])
{
	int i = column;
	int check = 0;
	while (i > -1 && check == 0)
	{
		check = checkBox(row,i,minefield);
		//printf("\n%d %d mines: %d\n",row,i,check);
		minefield[row][i] = VISIBLE_SAFE;
		i--;
	}
	i = column;
	check = 0;
	while (i < SIZE && check == 0)
	{
		check = checkBox(row,i,minefield);
		minefield[row][i] = VISIBLE_SAFE;
		i++;
	}
}

int revealRadial(int row, int column, int minefield[SIZE][SIZE], int first)
{
	int x = row - 1;
	int y = column - 1;
	int i;
	int j;
	int mines = 0;
	if (row < 0 && row >= SIZE && column < 0 && column >= SIZE)
		return 0;
	if (minefield[row][column] == HIDDEN_MINE)
	{	
		if (first == 0)
		{
			printf("Game over\n");
			return 1;
		}
		else
		{
			while(minefield[row][column] == HIDDEN_MINE)
				dropDown(minefield);
			minefield[row][column] = 0;
		}
	}
	for (i = x; i < x + 3; i++)
	{
		for (j = y; j < y + 3; j++)
			if (i > -1 && i < SIZE && j > -1 && j < SIZE)
				if (minefield[i][j] == HIDDEN_MINE)
					mines++;
	}
	if (mines == 0)
	{
		for (i = x; i < x + 3; i++)
		{
			for (j = y; j < y + 3; j++)
			{
				if (i > -1 && i < SIZE && j > -1 && j < SIZE)
					minefield[i][j] = VISIBLE_SAFE;
			}
		}
		checkDiagonal(row, column, minefield, 0);
		checkDiagonal(row, column, minefield, 1);
		checkDiagonal(row, column, minefield, 2);
		checkDiagonal(row, column, minefield, 3);
		checkHorizontal(row, column, minefield);
		checkVertical(row, column, minefield);
	}
	else
		minefield[row][column] = 0;
	printf("\n");
	int remaining = 0;
	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
			if(minefield[i][j] == 1)
				remaining++;
	}
	if (remaining == 0)
	{
		printf("You Won!");
		return 2;
	}
	return 0;
}







