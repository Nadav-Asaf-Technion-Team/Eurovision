#include "eurovision.h"
#include "state.h"
#include "judge.h"
#include "list.h"
#include <string.h>
#include <stdio.h>
//This is the main function for testing. make sure you're on 
//testing-branch before you work on it, and make sure you go
//back to master when you're done.
int main() {
	Eurovision eurovision = eurovisionCreate();
	printf("adding states..\n");
	eurovisionAddState(eurovision, 1, "a", "song1");
	eurovisionAddState(eurovision, 2, "b", "song2");
	eurovisionAddState(eurovision, 3, "c", "song3");
	eurovisionAddState(eurovision, 4, "d", "song4");
	printf("Added states\n");
	eurovisionAddVote(eurovision, 1, 4);
	eurovisionAddVote(eurovision, 1, 2);
	eurovisionAddVote(eurovision, 1, 2);
	eurovisionAddVote(eurovision, 1, 2);
	eurovisionAddVote(eurovision, 1, 3);
	eurovisionAddVote(eurovision, 1, 3);
	eurovisionAddVote(eurovision, 1, 2);
	return 0;
}