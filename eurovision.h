#ifndef EUROVISION_H_
#define EUROVISION_H_

#include "list.h"
#include "judge.h"
#include "state.h"

/**
* eurovision struct
*
* Implements a eurovision contest struct type.
*
* The following functions are available:
*   eurovisionCreate	- Creates a new empty eurovission struct
*   eurovisionDestroy	- Deletes an existing eurovision and frees all resources
*	eurovisionAddState	- adds a new state to the compatition
*	eurovisionRemoveState
*						- removes an existing state from the compatition
*	eurovisionAddJudge	- add a new judge to the compatition
*	eurovisionRemoveJudge
*						- removes an existing judge from the compatition
*	eurovisionAddVote	- add 1 vote from the giver state to the taker
*	eurovisionRemoveVote
*						- removes 1 vote from the giver state to the taker
*	eurovisionRunContest
*						- runs the entire contest and calculate the score with consideration
*						  for the persantage given to the audiance score and the judges score
*	eurovisionRunAudianceFavorite
*						-  runs the entire contest and calculate the score with consideration
*						  only for the audiance score
*	eurovisionRunGetFriendlyStates
*						- runs the entire contest and calculate the score. returns a list of every
						  pair of states the gave each other the highst score
*/

typedef enum eurovisionResult_t {
	EUROVISION_NULL_ARGUMENT,
	EUROVISION_OUT_OF_MEMORY,
	EUROVISION_INVALID_ID,
	EUROVISION_INVALID_NAME,
	EUROVISION_STATE_ALREADY_EXIST,
	EUROVISION_STATE_NOT_EXIST,
	EUROVISION_JUDGE_ALREADY_EXIST,
	EUROVISION_JUDGE_NOT_EXIST,
	EUROVISION_SAME_STATE,
	EUROVISION_SUCCESS
} EurovisionResult;


typedef struct Eurovision_t* Eurovision;

//_______________test functions_________________
int getAmountOfStates(Eurovision eurovision);

void checkSumResults(Eurovision eurovision, int stateId);

void printAllResults(Eurovision eurovision);

//_______________end of test functions_________

/**
Creates a new eurovision element

@return
	NULL - if malloc failed
	else returns the new eurovision
*/
Eurovision eurovisionCreate(); 

/**
destroy given state
@param eurovision - eurovision ro destroy
*/
void eurovisionDestroy(Eurovision eurovision); 

/**
adds a state to the eurovision contest
@param eurovision - eurovision element to whom you want to add a state
@param stateId    - ID of the state added
@param stateName  - name of the state added
@param stateSong  - name of the song of the state added
@return -
	EUROVISION_NULL_ARGUMENT - if one of the pointer arguments given was NULL
	EUROVISION_INVALID_ID	 - if the state ID given was <0 
	EUROVISION_INVALID_NAME  - if the name given containd char diffrent than a-z and ' '
	EUROVISION_STATE_ALREADY_EXIST - if the ID of the state given already exists
	EUROVISION_OUT_OF_MEMORY - if MALLOC failed
	EUROVISION_SUCCESS - otherwise

*/
EurovisionResult eurovisionAddState(Eurovision eurovision, int stateId,
	const char* stateName,
	const char* songName); 

/**
remove a state from the eurovision contest
@param eurovision - eurovision element from whom you want to remove a state
@param stateId    - ID of the state removed

@return -
	EUROVISION_NULL_ARGUMENT - if one of the pointer arguments given was NULL
	EUROVISION_INVALID_ID	 - if the state ID given was <0
	EUROVISION_STATE_NOT_EXIST - if the ID of the state given does not exists under eurovision
	EUROVISION_OUT_OF_MEMORY - if MALLOC failed
	EUROVISION_SUCCESS - otherwise

*/
EurovisionResult eurovisionRemoveState(Eurovision eurovision, int stateId); 

EurovisionResult eurovisionAddJudge(Eurovision eurovision, int judgeId,
	const char* judgeName,
	int* judgeResults); 

EurovisionResult eurovisionRemoveJudge(Eurovision eurovision, int judgeId); 

EurovisionResult eurovisionAddVote(Eurovision eurovision, int stateGiver,
	int stateTaker);

EurovisionResult eurovisionRemoveVote(Eurovision eurovision, int stateGiver,
	int stateTaker);

List eurovisionRunContest(Eurovision eurovision, int audiencePercent);

List eurovisionRunAudienceFavorite(Eurovision eurovision);

List eurovisionRunGetFriendlyStates(Eurovision eurovision);


#endif /* EUROVISION_H_ */