#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*

The mapping between integers and positions is as follows: 
_0_|_1_|_2_
_3_|_4_|_5_
 6 | 7 | 8

Therefore the string:
oooxxxooo

maps to the tic-tac-toe board:
_o_|_o_|_o_
_x_|_x_|_x_
 o | o | o

*/

/*
 * Function Purpose: Display current state of board to user
 * Parameters: 
 * 	pos: refers to the position of the new mark
 *	val: refers to the symbol to be marked
 *	board: refers to the state of the board before the mark is placed 
 */
void makeNewBoard(int pos, char val, char *board)
{
	board[pos] = val;
	
	int i;
	for(i = 0; i < 3; i++)
	{
		printf("%c", board[i]);	
	}
	printf("\n");
	
	for(; i < 6; i++)
	{
		printf("%c", board[i]);	
	}
	printf("\n");

	for(; i < 9; i++)
	{
		printf("%c", board[i]);	
	}
	printf("\n");	
}

int main()
{

	// user-inputted value specifying where to put their marker
	int userMove;

	// Display initial message to user
	printf("The board is set up as follows:\n");
	printf("012\n");
	printf("345\n");
	printf("678\n");
	printf("You must enter a number corresponding to the location you wish to place your marker.\n");
	printf("Your marker is an 'o'\n\n");


	// Set up the initial tic-tac-toe board and show it to the user
	int i;
	char currentBoard[] = "---------";
	makeNewBoard(0, '-', currentBoard);
	
	// Begin the match
 	int turnCounter = 0;
	for(; turnCounter < 9; turnCounter++)
	{
		// CPU turn
		if ((turnCounter % 2) == 0)
		{
			printf("COMPUTER'S TURN: ");
			// CHOOSE POSITION
			// PRINT POSITON ALONG WITH \n
			// CALL makeNewBoard(pos, 'x', currentBoard);
			// DELETE THIS LINE
			printf("\n");
		}

		// User turn
		else
		{
			printf("YOUR TURN: ");
			scanf("%d", &userMove);
			printf("\n");	
	
			makeNewBoard(userMove, 'o', currentBoard);
		}
		if((((currentBoard[0] == currentBoard[1]) && (currentBoard[1] == currentBoard[2])) && (currentBoard[0] != '-')) 
			|| ((currentBoard[3] == currentBoard[4]) && (currentBoard[4] == currentBoard[5])) && (currentBoard[4] != '-') 
			|| ((currentBoard[6] == currentBoard[7]) && (currentBoard[7] == currentBoard[8])) && (currentBoard[6] != '-') 
			|| ((currentBoard[0] == currentBoard[3]) && (currentBoard[3] == currentBoard[6])) && (currentBoard[0] != '-')
			|| ((currentBoard[1] == currentBoard[4]) && (currentBoard[4] == currentBoard[7])) && (currentBoard[1] != '-')	
			|| ((currentBoard[2] == currentBoard[5]) && (currentBoard[5] == currentBoard[8])) && (currentBoard[2] != '-')
			|| ((currentBoard[0] == currentBoard[4]) && (currentBoard[4] == currentBoard[8])) && (currentBoard[0] != '-')
			|| ((currentBoard[2] == currentBoard[4]) && (currentBoard[4] == currentBoard[6])) && (currentBoard[2] != '-'))
		{
			break;
		}	
	}
	printf("GAME END!\n");
	
	return 0;
}

