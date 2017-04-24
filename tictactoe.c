#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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


// NOTE: ADD DESCRIPTION
int findBestStrategy(FILE *fp, int turnNum, int *possibleMoves, int len)
{
	// In the file, the first value on any line is the 'score' of the corresponding strategy
	// This value will keep track of the highest value (assuming that it is greater than 0)
	int bestScore = 0;

	// This value will point to the starting position of the string of numbers corresponding to the best-scoring strategy
	int bestPos = 0;

	// It essentially acts a byte-offset which keeps track of the current position in the file
	int currPos = 0;

	// Acts as a "middle-man" when converting a number character from the file to an integer
	char convertMove;

	// After the conversion with convertMove, the value is stored in here
	int selectedMove;

	// Used as a counter in for-loops
	int i;

	// Used as a flag to determine whether or not the value in the file is a permitted option
	int moveFlag = 0;
	
	// The current character being looked at in the file
	char currChar = '0';

	while(currChar != EOF)
	{
		// Read the first character in the line (the 'score')
		currChar = fgetc(fp);
		currPos++;

		// Move the file pointer to the corresponding turn
		// and store the value.
		// Note that the decimal ASCII value of '0' is 48
		fseek(fp, turnNum, SEEK_CUR);
		fread(&convertMove, 1, 1, fp);
		selectedMove = (((int) convertMove) - 48);

		printf("SELECTED MOVE: %d\n", selectedMove);
	
		// Move back to the position before the last block of code was executed
		fseek(fp, currPos, SEEK_SET);
		
		// Check to see if the desired move in the file is a valid move
		for(i = 0; i < len; i++)
		{
			if(possibleMoves[i] == selectedMove)
			{
				moveFlag = 1;
				break;
			}
		}

		// Compare the current line's score with the best score so far.
		// If the current line's score is higher, store it as the best score so far.
		// Set the bestPos variable to this position to prevent searching through the file twice.
		if((((((int) currChar) - 48) - bestScore) > 0) && (moveFlag == 1))
		{
			bestScore = (((int) currChar) - 48);
			bestPos = currPos;
		}
		
		// Skip past the rest of the characters in the line
		while((currChar != EOF) && (currChar != '\n'))
		{
			currChar = fgetc(fp);
			currPos++;
			//printf("currPos: %d\n", currPos);
		}
		moveFlag = 0;
	}

	// Move the bestPos variable to the corresponding 'index'
	// The following table illustrates the outcome of the operation:
	// 
	//  turnNum    | 0 | 2 | 4 | 6 | 8 
	// -------------------------------
	//  bestPos += | 1 | 3 | 5 | 7 | 9
	// 
	// Note that this assumes that the computer always goes first.
	bestPos += (turnNum + 1);

	// If the bestScore is still zero, return -1
	// Otherwise, return the byte offset to the 'best' strategy
	if (bestScore == 0)
	{
		return -1;
	}
	else
	{
		return bestPos;
	}
}

// NOTE: ADD DESCRIPTION
void updateStrategy(FILE *fp, int *strategyUsed, int len, int increment)
{
	char strategyConv[len];
	
	char buffer[100];
	int bIndex = 0;

	char *start;
	int fileOffset = 0;

	char score;

	memset(buffer, 0, 100);

	//Convert strategyUsed to a corresponding string
	int i = 0;
	for(; i < len; i++)
	{
		strategyConv[i] = strategyUsed[i] + 48;
	}


	char newChar = '0';
	while(newChar != EOF)
	{
		newChar = fgetc(fp);
		fileOffset += 1;

		if(newChar == '\n')
		{
			//Begin searching through this line to see if strategy was used here
			start = strstr(buffer, strategyConv);	
			if(start != NULL)
			{
				//If the strategy was used, find the occurrence in the file and update it
				// DEBUG: printf("start: %s\n", start);
				break;
			}			

			memset(buffer, 0, 100);
			bIndex = 0;
		}
		else if(newChar != EOF)
		{
		 	buffer[bIndex] = newChar;
			bIndex++;
		}
	}	

	// If an occurrence was found, update the corresponding strategy in the file
	// Otherwise, append the new strategy
	if (start != NULL)
	{
		// NOTE: ERROR! If the player finishes the game earlier than the computer expects, it cant update its score properly.
		// Move back to the 'score'
		printf("LEN: %d\n", len);
		fileOffset -= (len + 3);
		fseek(fp, fileOffset, SEEK_SET);

		// Read the value, and increment/decrement it 
		fread(&score, 1, 1, fp);
		printf("SCORE: %d\n", (((int) score) - 48));
		fseek(fp, fileOffset, SEEK_SET);

		// Increment
		fprintf(fp, "%d", (((int) score) - 48 + increment));
	}
	else
	{
		//NOTE: APPEND STRATEGY 	
	}
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
	// The array is initialized with values of 9,
	// because that is the only single-digit int that does not correspond to a position on the board
	int strategyUsed[9] = {9,9,9,9,9,9,9,9,9};
	int suIndex = 0;

	FILE *fp;
	
	// This is the variable that is modified when findBestStrategy is called.
	// It holds the starting position to the 'best' strategy
	int offset;

	// Used for the conversion from char to integer when reading from the logfile
	char convert;


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
		fp = fopen("log.txt", "r+");

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

			// If the computer has no knowledge of which strategy to use, perform a random strategy
			if (fp == NULL)
			{
				// Random strategy
				chosenMove = possibleMoves[random_at_most(pmIndex - 1)];
				strategyUsed[suIndex] = chosenMove;
				suIndex++;
				printf("%d", chosenMove);	
			}
			
			// If a file with a positive victory count exists, choose the greatest one.
			else
			{
			
				// Chose highest winning strategy
				offset = findBestStrategy(fp, turnCounter, possibleMoves, pmIndex);
				// If no strategy has a score > 0, perform random strategy
				if (offset == -1)
				{
					chosenMove = possibleMoves[random_at_most(pmIndex - 1)];
					strategyUsed[suIndex] = chosenMove;
					suIndex++;
					printf("%d", chosenMove);
				}		

				//NOTE: What to do if a 9 in the file gets read?
				else
				{
					// Perform strategy
					fseek(fp, offset, SEEK_SET);
					fread(&convert, 1, 1, fp);
					chosenMove = (((int) convert) - 48);
					strategyUsed[suIndex] = chosenMove;
					suIndex++;
					printf("%d", chosenMove);	
				}

	
			}	

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
			
			strategyUsed[suIndex] = userMove;
			suIndex++;
	
			makeNewBoard(userMove, 'o', currentBoard);
		}

		// CPU Victory
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
			
			// If no logfile exists, create a new one and store data to it
			if(fp == NULL)
			{
				fp = fopen("log.txt", "w");
				fprintf(fp, "%d\t", 1);
				
				for(i = 0; i < 9; i++)
				{
					fprintf(fp, "%d", strategyUsed[i]);
				}
				fprintf(fp, "%s", "\n");

				fclose(fp);
			}

			// Otherwise, update the existing logfile
			// NOTE: seek through the file to see if a string matches the strategyUsed array
			else
			{
				fclose(fp);
				fp = fopen("log.txt", "r+");
				updateStrategy(fp, strategyUsed, 9, 1);	
				fclose(fp);
			}

			break;
		}

		// Player Victory
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
			
			// If the logfile exists, attempt to find which strategy was used, and update it
			if(fp != NULL)
			{
				fclose(fp);
				fp = fopen("log.txt", "r+");
				updateStrategy(fp, strategyUsed, suIndex, -1);
				fclose(fp);
			}
			break;
		}
			
	}
	
	return 0;
}

