#include "judge.h"
//This is the main function for testing. make sure you're on 
//testing-branch before you work on it, and make sure you go
//back to master when you're done.
int main() {
	int results[10] = { 101, 102, 103, 104, 105, 106, 107, 108, 109, 110 };
	Judge judge = judgeCreate(55, "Nadav", results);
	
	printf("print judge id is %d\n", getJudgeId(judge));
	Judge newJudge = judgeCopy(judge);
	int* newResults = getJudgeResults(newJudge);
	for (int i = 0; i < 10; i++) {
		printf("%d, ", newResults[i]);
	}
}