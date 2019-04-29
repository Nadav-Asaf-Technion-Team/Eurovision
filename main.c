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
	eurovisionAddState(eurovision, 1, "a", "songa");
	eurovisionAddState(eurovision, 2, "b", "songb");
	EurovisionResult results = eurovisionAddState(eurovision, 3, "c", "songc");
	printf("Error code for state 3 is %d\n", results);
	eurovisionAddState(eurovision, 4, "d", "songd");
	printf("Added states, amount of states is %d\n", getAmountOfStates(eurovision));
	eurovisionAddVote(eurovision, 1, 4);
	eurovisionAddVote(eurovision, 1, 2);
	eurovisionAddVote(eurovision, 1, 2);
	eurovisionAddVote(eurovision, 1, 2);
	eurovisionAddVote(eurovision, 1, 3);
	printf("added 1 vote to 3\n");
	eurovisionAddVote(eurovision, 1, 3);
	printf("added votes for 3\n");
	eurovisionAddVote(eurovision, 1, 2);
	printf("checking sum result");
	checkSumResults(eurovision, 1);
	return 0;
}