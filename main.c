#include "eurovision.h"
#include "state.h"
#include "judge.h"
#include <string.h>
#include <stdio.h>
//This is the main function for testing. make sure you're on 
//testing-branch before you work on it, and make sure you go
//back to master when you're done.
int main() {
	printf("====================TEST 1================\n");
	State state = stateCreate(1, "israel", "toy");
	printf("%d, %s, %s", getStateId(state), getStateName(state), getSongName(state));
	State state2 = stateCopy(state);
	printf("\nfuck\n");
	printf("%d, %s, %s\n", getStateId(state), getStateName(state), getSongName(state));
	State state3 = stateCreate(2, "germany", "ddd");
	printf("%d, %s, %s\n", getStateId(state3), getStateName(state3), getSongName(state3));
	printf("====================TEST 2================\n");
	addVoteFromState(state, 2);
	addVoteFromState(state, 2);
	addVoteFromState(state, 2);
	printf("the new vote is : %d\n", getVoteFromStateToState(state, getStateId(state3)));
	printf("====================TEST 3================\n");
	removeAllVotesFromStateToState(state, 2);
	printf("the new new vote is : %d\n", getVoteFromStateToState(state, getStateId(state3)));
	

	stateDestroy(state);
	stateDestroy(state2);
	stateDestroy(state3);

return 0;
}