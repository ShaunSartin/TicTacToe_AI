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


/*
 * Function Purpose: Create a random number between values
 * Parameters:
 * 	max: The maximum acceptable value
 * Returns: 
 * 	A pseudo-random long between [0, max] which refers to the index of the 'possibleMoves' array
 * Stolen-From: http://stackoverflow.com/questions/2509679/how-to-generate-a-random-number-from-within-a-range
 */
long random_at_most(long max) {
  unsigned long
    num_bins = (unsigned long) max + 1,
    num_rand = (unsigned long) RAND_MAX + 1,
    bin_size = num_rand / num_bins,
    defect   = num_rand % num_bins;

  long x;
  do {
   x = random();
  }

  while (num_rand - defect <= (unsigned long)x);

  return x/bin_size;
}
int main()
{

	// user-inputted value specifying where to put their marker
	int userMove;

	// NOTE: may have an issue with this later 
	int possibleMoves[9] = {-1,-1,-1,-1,-1,-1,-1,-1,-1};
	int pmIndex = 0;
	int chosenMove;

	// Keep track of the moves used by the CPU
	// This will be written/updated in the logfile
	int strategyUsed[5] = {-1,-1,-1,-1,-1};
	int suIndex = 0;

	FILE *fp;


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
			
			// Analyze the current board to see what moves are possible
			for(i = 0; i < 9; i++)
			{
				if(currentBoard[i] == '-')
				{
					possibleMoves[pmIndex] = i;
					//printf("%d", possibleMoves[pmIndex]);
					pmIndex++;		
					//printf("%d\t", pmIndex);
				}	
			}

			fp = fopen("log.txt", "r");
			if (fp == NULL)
			{
				// Random strategy
				chosenMove = possibleMoves[random_at_most(pmIndex - 1)];
				printf("%d", chosenMove);	
			}
				
			// If a file with a positive victory count exists, choose the greatest one.
			// Else, choose randomly

			printf("\n");
			makeNewBoard(chosenMove, 'x', currentBoard);

			// Reset data regarding current state of the board, as it will no longer be valid	
			for(i = 0; i <= pmIndex; i++)
			{
				possibleMoves[i] = -1;
			}
			pmIndex = 0;

		}

		// User turn
		else
		{
			printf("YOUR TURN: ");
			scanf("%d", &userMove);
			printf("\n");	
	
			makeNewBoard(userMove, 'o', currentBoard);
		}
		if((((currentBoard[0] == currentBoard[1]) && (currentBoard[1] == currentBoard[2])) && (currentBoard[0] == 'x')) 
			|| ((currentBoard[3] == currentBoard[4]) && (currentBoard[4] == currentBoard[5])) && (currentBoard[4] == 'x') 
			|| ((currentBoard[6] == currentBoard[7]) && (currentBoard[7] == currentBoard[8])) && (currentBoard[6] == 'x') 
			|| ((currentBoard[0] == currentBoard[3]) && (currentBoard[3] == currentBoard[6])) && (currentBoard[0] == 'x')
			|| ((currentBoard[1] == currentBoard[4]) && (currentBoard[4] == currentBoard[7])) && (currentBoard[1] == 'x')	
			|| ((currentBoard[2] == currentBoard[5]) && (currentBoard[5] == currentBoard[8])) && (currentBoard[2] == 'x')
			|| ((currentBoard[0] == currentBoard[4]) && (currentBoard[4] == currentBoard[8])) && (currentBoard[0] == 'x')
			|| ((currentBoard[2] == currentBoard[4]) && (currentBoard[4] == currentBoard[6])) && (currentBoard[2] == 'x'))
		{
			printf("Computer Victory!\n");
			break;
		}
		else if((((currentBoard[0] == currentBoard[1]) && (currentBoard[1] == currentBoard[2])) && (currentBoard[0] == 'o')) 
			|| ((currentBoard[3] == currentBoard[4]) && (currentBoard[4] == currentBoard[5])) && (currentBoard[4] == 'o') 
			|| ((currentBoard[6] == currentBoard[7]) && (currentBoard[7] == currentBoard[8])) && (currentBoard[6] == 'o') 
			|| ((currentBoard[0] == currentBoard[3]) && (currentBoard[3] == currentBoard[6])) && (currentBoard[0] == 'o')
			|| ((currentBoard[1] == currentBoard[4]) && (currentBoard[4] == currentBoard[7])) && (currentBoard[1] == 'o')	
			|| ((currentBoard[2] == currentBoard[5]) && (currentBoard[5] == currentBoard[8])) && (currentBoard[2] == 'o')
			|| ((currentBoard[0] == currentBoard[4]) && (currentBoard[4] == currentBoard[8])) && (currentBoard[0] == 'o')
			|| ((currentBoard[2] == currentBoard[4]) && (currentBoard[4] == currentBoard[6])) && (currentBoard[2] == 'o'))
		{
			printf("Player Victory!\n");
			break;
		}
			
	}
	
	return 0;
}

