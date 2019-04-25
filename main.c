#include "eurovision.h"
#include <stdio.h>
//This is the main function for testing. make sure you're on 
//testing-branch before you work on it, and make sure you go
//back to master when you're done.
int main() {
	Eurovision eurovision = eurovisionCreate();
	int results[10] = { 101, 102, 103, 104, 105 };
	if (eurovisionAddJudge(eurovision, 55, "John", results) != EUROVISION_INVALID_NAME) {
		printf("an error has occured\n");
		return 1;
	}
	else printf("number of judges is %d", getAmountOfJudges(eurovision));
	return 0;
}