#include "eurovision.h"
#include "state.h"
#include "judge.h"
#include <string.h>
#include <stdio.h>
//This is the main function for testing. make sure you're on 
//testing-branch before you work on it, and make sure you go
//back to master when you're done.
int main() {
	State state = stateCreate(1, "israel", "toy");
	printf("%d, %s, %s", getStateId(state), getStateName(state), getSongName(state));
	State state2 = stateCopy(state);
	printf("f");
	printf("%d, %s, %s\n", getStateId(state), getStateName(state), getSongName(state));
	addVoteFromState(state, 2);
	State state3 = stateCreate(2, "germany", "ddd");
	stateDestroy(state);
	stateDestroy(state2);

return 0;
}