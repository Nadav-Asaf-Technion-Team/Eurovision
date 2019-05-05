#ifndef JUDGE_H_
#define JUDGE_H_

#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#define STATES_TO_SCORE 10

/**
* judge struct
*
* Implements a judge struct type for a eurovision contest.
*
* The following functions are available:
*   judgeCreate			- Creates a new empty judge
*   judgeDestroy		- Deletes an existing judge and frees all resources
*	getJudgeId			- returns the judge ID as an int
*	getJudgeName		- returns the states name as a char*
*	getJudgeResults		- returns the final results given from said judge as a
*						  size 10 int*
*	stateCopy			- copies an existing judge struct

*/

typedef struct Judge_t* Judge;

typedef enum judgeResult_t {
	JUDGE_NULL_ARGUMENT,
	JUDGE_SUCCESS
}JudgeResult;

/**
Creates a new judge element
@param judgeId - unique judge ID for the new judge
@param judgeName - string representing the judges name (not unique)
@param judgeResults - array of integers containing state IDs, represnting the
					  score the judge had given to those states
@return
	NULL - if malloc failed
	else returns the new judge
*/
Judge judgeCreate(int judgeId, const char* judgeName, int* judgeResults);

/**
Destroys given judge.
@param judge - judge to destroy
*/
void judgeDestroy(Judge judge);

/**
Returns judge ID
@param judge - judge element whose ID you want
@return -
	NULL if NULL was given as judge
	judgeId of judge otherwise
*/
int getJudgeId(Judge judge);

/**
Returns judge name
@param judge - judge element whose name you want
@return -
	NULL if NULL was given as judge
	judgeName of judge otherwise
*/
const char* getJudgeName(Judge judge);

/**
Returns judge results array
@param judge - judge element whose results you want
@return -
	NULL if NULL was given as judge
	judgeResults of judge otherwise
*/
int* getJudgeResults(Judge judge);

/**
copies judge and returns the new judge.
@param judge - judge to copy
@return - 
	NULL if NULL was given as judge or if copying failed.
	else - returns the new judge.
*/
Judge judgeCopy(Judge judge);

//TODO: write description for function
int getResultFromJudge(Judge judge, int stateId);

#endif /* JUDGE_H_ */