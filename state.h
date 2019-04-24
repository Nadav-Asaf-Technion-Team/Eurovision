#ifndef STATE_H_
#define STATE_H_

#include "map.h"
//Nadav
typedef struct State_t* State;

typedef enum stateResult_t {
	STATE_NULL_ARGUMENT,
	STATE_INVALID_NAME,
	STATE_NOT_EXIST,
	STATE_SUCCESS
} StateResult;

/**
Creates a new state element
@param stateId - unique state ID for the new state
@param judgeName - string representing the states name (not unique)
@param songName - string representing the states song name (not unique)
@return
	NULL - if malloc failed
	else returns the new state
*/
State stateCreate(int stateId, const char* stateName, const char* songName);

/**
Destroys given state.
@param state - state to destroy
@return
	STATE_NOT_EXIST - if given state is NULL
	STATE_SUCCESS - elsewise
*/
StateResult stateDestroy(State state);

/**
Returns state ID
@param state - state element whose ID you want
@return -
	-1 -  if NULL was given as state
	stateId of state otherwise
*/
int getStateId(State state);

/**
Returns state name
@param state - state element whose name you want
@return -
	NULL if NULL was given as judge
	stateName of state otherwise
*/
const char* getStateName(State state);


/**
Returns states song name
@param state - state element whose song name you want
@return -
	NULL if NULL was given as state
	songName of state otherwise
*/
const char* getSongName(State state);

/**
adds 1 to the vote count for stateTaker in the map of votes of stateGiver
@param stateGiver - state element from whom you want to give a vote
@param stateTakerId - id of the state to whom you want to give a vote
*/
void addVoteFromState(State stateGiver, int stateTakerId);

State stateCopy(State state);

/**
deducts 1 from the vote count for stateTaker in the map of votes of stateGiver
@param stateGiver - state element from whom you want to deduct a vote
@param stateTakerId - id of the state to whom you want to deduct a vote
*/
void removeVoteFromState(State stateGiver, int stateTakerId);

/**
sums all the raw votes of state into an arrrey of the first 10 hightest scored states
this array can be found in getAllResultsFromState.
@param state - state element for whom you want to sum votes into results
*/
void sumResultsFromState(State state);

/**
Returns state results array
@param state - state element whose results you want
@return -
	NULL if NULL was given as state
	stateResults of state otherwise
*/
int* getAllResultsFromState(State state);

/**
returns the spesific final result given from stateGiver to StateTaker
@param stateGiver - state element from whom the result was given
@param stateTakerId - id of the state to whom the result was given
@return -
	0 - if no result was given
	1-7,8,10,12 - the result given 
*/
int getResultFromStateToState(State stateGiver, int stateTakerId);

/**
returns the number of raw votes given from stateGiver to StateTaker
@param stateGiver - state element from whom the result was given
@param stateTakerId - id of the state to whom the result was given
@return -
	NO_VOTES - defined as 0, will be returned if no vote was given to stateTaker
	returns the number of votes otherwise
*/
int getVoteFromStateToState(State stateGiver, int stateTakerId);

/**
removes all votes from stateGiver to stateTakerId
@param stateGiver - state element  whose votes you want to remove 
@param stateTakerId - id of the state to whom you want to remove votes
@return -
	STATE_NULL_ARGUMENT - will be returned if NULL was given as stateGiver or stateTaker
	STATE_NOT_EXIST - will be returned if no votes were given in the first place
	returns STATE_SUCCESS otherwise
*/
StateResult removeAllVotesFromStateToState(State stateGiver, int stateTakerId);


#endif /* STATE_H_ */