// File name: Game.cpp

// # Defines
#define WON 1
#define LOST 0

#define MAXCOLS 11
#define MINCOLS 2
#define DEFCOLS 7

#define MAXBASE 100
#define MINBASE 2
#define DEFBASE 10

// #Include items
#include "general.h"
#include <iostream.h>

// Function Prototypes
uint Combination(uint N, uint R, uint *pResult);
uint NextPosition(uint *pPos, bool bLastPositionsResult);
void PrintCurrentPosition(uint *pPos);
uint DbMemoryRequirement(uint *pMemRequirement);
bool Compare(uint *pPos, uint **ppdataBase);

// Globals
uint uiTotalColumns = DEFCOLS;
uint uiBase = DEFBASE;
uint uiCurrentDbSize = 0;
bool bFirstGo = true;




// Function Combination
// Return Values:
//	OK		 - All went well
//	NOMEMORY - not enough memory
//	BADINPUT - A combination was attempted with R > N
//	OVERFLOW - The calculation overflowed
//
uint Combination(uint N, uint R, uint *pResult)
{
	uint *pStartOfArray,
		 *pEndOfArray,
		 *pVar, *pVar2;

	uint i = 999, j = 999, jcopy = 999;
	bool bDone = false;
	bool bKeepGoing = false;
	double dTemp = 999.0;	

	// Check it's a valid combination
	if(R > N)
	{
		return (BADINPUT);
	}

	// R==0 and R==1 don't need much work
	switch (R)
	{
	case 0:
		*pResult = 1;
		break;

	case 1:
		*pResult = N;
		break;

	default:
		R=(R > (N - R) ? (N - R) : R);
		
		// Allocate memory for the array of N - R
		// elements of uints
		if((pStartOfArray = (uint *) new uint[R]) == NULL)
		{
			return(NOMEMORY);
		}

		// Point pEnd to the array element beyond the 
		// end of the array
		pEndOfArray = pStartOfArray + R;

		// Set up a temporary variable pointer starting
		// off by equalling pStartOfArray
		pVar = pStartOfArray;

		//Initialise the new array
		while(pVar < pEndOfArray)
		{
			*pVar++ = N--;
		}

		// Start to divide the array elements
		for(j = R; j >= 2; j--)
		{
			for(bDone = false, pVar = pStartOfArray; pVar < pEndOfArray && bDone == false; pVar++)
			{
				if((*pVar) % j == 0)
				{
					*pVar /= j;
					bDone = true;
				}
			}
			// Here's where we check to see if it failed to divide. If it did fail
			// we'll have to use a blunderbuss approach to get rig of j.
			if(bDone == false)
			{
				// Make a copy of j so that we don't change j
				jcopy = j;
				// Loop whilst i <= jcopy
				for(i = 2; i <= jcopy; i++)
				{
					bKeepGoing = true;
					while(bKeepGoing && jcopy != 1)
					{
						// Go through the array and try dividing each one by i
						bKeepGoing = false;
						for(pVar2 = pStartOfArray; pVar2 < pEndOfArray && jcopy != 1; pVar2++)
						{
							// If jcopy and *pVar both divide by i then go ahead
							// and divide them. Seeing as it worked, we'll set
							// the bKeepGoing flag to true so that we can have
							// another go using the same i.
							if(jcopy % i == 0 && *pVar2 % i == 0)
							{
								jcopy /= i;
								*pVar2 /= i;
								bKeepGoing = true;
							}
						}
					}
				}
			}		
		}

		// Multiply all the remaining elements together to get the result
		for(*pResult = 1, pVar = pStartOfArray; pVar < pEndOfArray; pVar++)
		{
			*pResult *= (*pVar);
		}

		// Just make sure we've not overflowed
		dTemp = (double) *pResult;
		for(pVar = pStartOfArray; pVar < pEndOfArray; pVar++)
		{
			dTemp /= (double) *pVar;
		}
		if(dTemp != 1.0)
		{
			cout << "The Combination function has overflowed.\n";
			return(OVERFLOW);
		}

		delete pStartOfArray;

		break;
	}
	return (OK);
}



// Function name: NextPosition
// Return values
//	bMoreToDo - Boolean telling calling function if there are
//             any more positions
//
uint NextPosition(uint *pPos, bool bLastPosition)
{
	bool bKeepGoing = true, bMoreToDo = true;
	uint i = 0, j = 0;

	// If the LastPosition was LOST, there's no point in incrementing
	// the zeroth column, so start with the first column instead.
	if(bLastPosition == LOST)
		i++;

	// While we don't finish ...
	while(bKeepGoing == true && bMoreToDo == true)
	{
		// Increment the column being pointed to
		(*(pPos + i))++;
		if(*(pPos + i) != uiBase) // If it hasn't gone over the top...
		{
			if(i) // If we're not looking at the zeroth column...
			{
				// Set up a loop to set all earlier columns
				// to the same value as this new one
				for(j = 0; j <= (i - 1); j++)
				{
					*(pPos + j) = *(pPos + i);
				}
			}
			bKeepGoing = false;
		}
		else // If it has gone over the top...
		{
			i++; // Try the next column...
			// Just make sure we're not at the end...
			if(i == uiTotalColumns)
			{
				bMoreToDo = false;
				// Set the columns to maximum. Not really needed
				// but it guarantees not getting any higher if this
				// function's return code isn't checked for properly!
				for(i = 0; i < uiTotalColumns; i++)
				{
					(*(pPos + i))--;
				}
			}
		}
	}

	return (bMoreToDo);
}



// Function name: PrintCurrentPosition
// Return values
//
void PrintCurrentPosition(uint *pPos)
{
	uint i;
	
	for(i = uiTotalColumns - 1; i > 0; i--)
	{
		cout << *(pPos + i) << "\t";
	}
	cout << *pPos << "\n";
}



// Function name: DbMemoryRequirement
// Return values
//	OK			 - All went well
//	GENERALERROR - Either not enough memory or bad call to Combination
//
uint DbMemoryRequirement(uint *pMemRequirement)
{
	uint uiBaseTemp = 999,
		 p = 999;
	
	int i =999,
		iCount = 0;

	// Check that the number of colums are odd. If not, round up.
	uint uiTempColumns = 999;
	if(uiTotalColumns % 2 != 1)
	{
		uiTempColumns = uiTotalColumns + 1;
	}
	else
	{
		uiTempColumns = uiTotalColumns;
	}

	// Calculate "p"
	uiBaseTemp = uiBase - 1;
	while(uiBaseTemp != 1)
	{
		uiBaseTemp /= 2;
		iCount++;
	}
	
	for(i = 1; i <= iCount; i++)
	{
		uiBaseTemp *= 2;
	}
	p = uiBaseTemp;

	// Calculate the number of losing positions.
	// NOTE: this formula only works for odd numbers. Even numbers
	// are assumed to be the next highest odd number. You should be 
	// able to work out what the formula is from the code because the
	// code follows the formula exactly!

	uint x = 999;
	uint part1, part2;
	uint uiTotalSoFar = 0;

	for(x = 1; x <= (uiTempColumns + 1) / 2 ; x++)
	{
		if((Combination((p + 2 * x - 2), p, &part1)) != OK)
			return (GENERALERROR);
		if((Combination((uiBase - p + uiTempColumns - 2 * x), (uiTempColumns + 1 - 2 * x), &part2)) != OK)
			return (GENERALERROR);
		uiTotalSoFar += (part1 * part2 / (2 * x - 1));
	}

	// Get ready to send the result back to calling function
	*pMemRequirement = uiTotalSoFar;

	return (OK);
}



// Function name: Compare
// Return values
//	LOST - The position tested is lost
//	WON  - The position tested is won
//
bool Compare(uint *pPos, uint **ppDataBase)
{
	// The placeholder keeps track of whether how many matches are found
	// between the database and the current position
	// No need to do a memory check (excessive)!
	uint i = 999;
	uint uiItems = 999;

	uint iPos = 999, iDbCol = 999, iDbRow = 999;
	uint uiMatches = 999;
	bool bDone = false;
	bool bPosition = LOST;
	
	int *pPlaceHolder = new int[uiTotalColumns];

	// Zero all the placeholders. Use the loop to see if it's the first go.
	for(iPos = 0; iPos < uiTotalColumns; iPos++)
	{
		pPlaceHolder[iPos] = 0;
	}


	// If it's the fisrt go: make an exception to the general case.
	if(bFirstGo)
	{
		bFirstGo = false;
		for(i = 0; i < uiTotalColumns; i++)
		{
			*(ppDataBase[uiCurrentDbSize] + i) = *(pPos + i);
		}
		PrintCurrentPosition(ppDataBase[uiCurrentDbSize]);
		uiCurrentDbSize++;
		
		// In this special case, we exit out early
		return (LOST);
	}


	// For every other situation...
	// For every row in the database...
	bPosition = LOST;
	for(iDbRow = 0; iDbRow <= (uiCurrentDbSize - 1) && bPosition == LOST; iDbRow++)
	{
		uiMatches = 0;
		// For each element of the current position...
		for(iPos = 0; iPos < uiTotalColumns; iPos++)
		{
			// Start at the first database column and compare with the current
			// position's element that we're looking at. Of course, if the current
			// position's element that we're looking at has already been matched,
			// don't bother.
			bDone = false;
			for(iDbCol = 0; iDbCol < uiTotalColumns && bDone == false; iDbCol++)
			{
				if(*(pPos + iPos) == *(ppDataBase[iDbRow] + iDbCol) && pPlaceHolder[iDbCol] != 1)
				{
					pPlaceHolder[iDbCol] = 1;
					bDone = true;
					uiMatches++;
				}
			}
		}

		// We've been through every element of the current position and there
		// are iMatches. If it's number of columns minus one,
		// the current position is won and there's no point in carrying on.
		// (The outer for loop makes use of this flag.) Otherwise, re-zero the
		// place holders and try again.
		if(uiMatches == (uiTotalColumns - 1))
		{
			bPosition = WON;
		}
		else
		{
			for(i = 0; i < uiTotalColumns; i++)
			{
				pPlaceHolder[i] = 0;
			}

		}
	}

	// If it's still lost, add it to the database. Note that the
	// uiCurrentDbSize starts at one after the first go, so the next
	// Db entry will be at position uiCurrentDbSize. Also, print it.
	if(bPosition == LOST)
	{
		for(i = 0; i < uiTotalColumns; i++)
		{
			*(ppDataBase[uiCurrentDbSize] + i) = *(pPos + i);
		}
		PrintCurrentPosition(ppDataBase[uiCurrentDbSize]);
		uiCurrentDbSize++;
	}

	return (bPosition);
}






// Function main
// Return Values:
//	OK		 - All went well
//	NOMEMORY - not enough memory
//	BADINPUT - A combination was attempted with R > N
//
uint main(void)
{
	uint i = 999, j = 999;
	uint **ppDataBase; // Pointer to pointer to database of uints
	uint *pPos; // Pointer to the zeroth column of the working position
	uint uiMemNeeded = 0; // Amount of memory to store the database

	// Vain opening screen...
	cout << "That Game Thang\n";
	cout << "Mark Thurston (C) 1998\n\n";

	// Create a new position
	pPos = new uint[uiTotalColumns];

	// Zero all the columns
	for(i = 0; i < uiTotalColumns; i++)
	{
		*(pPos + i) = 0;
	}

	// Allocate enough memory for the database. 
	if((DbMemoryRequirement(&uiMemNeeded)) != OK)
	{
		cout << "Insufficient memory or bad arguments in module DbMemoryRequirement\n";
		return (GENERALERROR);
	}

	// OK. Lets reserve the memory now we know how much...
	if((ppDataBase = (uint **) new uint[uiMemNeeded]) == NULL)
		return (NOMEMORY);
	for(i = 0; i < uiMemNeeded; i++)
	{
		if((ppDataBase[i] = (uint *) new uint[uiTotalColumns]) == NULL)
			return (NOMEMORY);
	}

	// Zero the whole lot of database elements...
	for(i = 0; i < uiMemNeeded; i++)
	{
		for(j = 0; j < uiTotalColumns; j++)
		{
			*(*(ppDataBase + i) + j) = 0;
		}
	}

	// Let's do the work
	// Note that the first call to NextPosition will set it
	// to all zeros except the zeroth column, which will be set
	// to one
	uint uiLostCount = 0;
	uint uiTotalPositions = 0;
	bool bLastPosition= WON; // On the first go, bLastPosition
							 // must be initialised to WON

	while(NextPosition(pPos, bLastPosition))
	{
		uiTotalPositions++;
		bLastPosition = Compare(pPos, ppDataBase);
		if(bLastPosition == LOST)
		{
			uiLostCount++;
		}
	}

	// Finally, some other information for the user
	cout << "Number of possible positions: " << uiTotalPositions << "\n";
	cout << "Number of reserved positions: " << uiMemNeeded << "\n";
	cout << "Number of lost positions: " << uiLostCount << "\n";

	return (OK);
}
