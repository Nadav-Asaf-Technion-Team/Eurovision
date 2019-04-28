#include "state.h"
#include "map.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define NUMBER_OF_RESULTS_PER_STATE 10
#define NO_VOTES 0
#define NUMBER_OF_FIELDS_TO_MAP 2

struct State_t {
	int stateId;
	const char* stateName;
	const char* songName;
	Map stateVotes;
	int* stateResults;
};

/*===========================implimantation of functions for map======================================*/
static MapKeyElement copyKeyInt(MapKeyElement n) {
	if (!n) {
		return NULL;
	}
	int* copy = malloc(sizeof(*copy));
	if (!copy) {
		return NULL;
	}
	*copy = *(int*)n;
	return copy;
}

static MapDataElement copyDataInt(MapDataElement n) {
	if (!n) {
		return NULL;
	}
	int* copy = malloc(sizeof(*copy));
	if (!copy) {
		return NULL;
	}
	*copy = *(int*)n;
	return copy;
}

/** Function to be used by the map for freeing elements */
static void freeKeyInt(MapKeyElement n) {
	free(n);

}/** Function to be used by the map for freeing elements */
static void freeDataInt(MapDataElement n) {
	free(n);
}

/** Function to be used by the map for comparing elements */
static int compareInts(MapKeyElement n1, MapKeyElement n2) {
	return (*(int*)n1 - *(int*)n2);
}

static int compareIntsReversed(MapKeyElement n1, MapKeyElement n2) {
	return (*(int*)n2 - *(int*)n1);
}
/*===========================end of implimantation======================================*/
State stateCreate(int stateId, const char* stateName, const char* songName) {
	State state = malloc(sizeof(*state));
	if (state == NULL) return NULL;
	state->stateId = stateId;
	state->stateName = stateName;
	state->songName = stateName;
	state->stateVotes = mapCreate(copyDataInt, copyKeyInt, freeDataInt, freeKeyInt, compareInts);
	state->stateResults = malloc(sizeof(int) * NUMBER_OF_RESULTS_PER_STATE);
	return state;
}


StateResult stateDestroy(State state) {
	if (!state) return STATE_NOT_EXIST; 
	mapDestroy(state->stateVotes);
	free(state->stateResults);
	free(state);
	return STATE_SUCCESS;
}

int getStateId(State state) {
	if (!state) return -1;
	return state->stateId;
}

const char* getStateName(State state) {
	if (!state) return NULL;
	return state->stateName;
}

const char* getSongName(State state) {
	if (!state) return NULL;
	return state->songName;
}

State stateCopy(State state) {
	if (state == NULL) return NULL;
	int newId = getStateId(state);
	const char* newSong = getSongName(state);
	const char* newStateName = getStateName(state);
	Map newVotes = mapCopy(state->stateVotes);
	int* newResults = state->stateResults;
	State newState = stateCreate(newId, newStateName, newSong);
	newState->stateVotes = newVotes;
	return newState;
}

void addVoteFromState(State stateGiver, int stateTakerId) {
	int current = mapGet(stateGiver->stateVotes, stateTakerId);
	mapPut(stateGiver->stateVotes, stateTakerId, current + 1);
}

void removeVoteFromState(State stateGiver, int stateTakerId) {
	int current = mapGet(stateGiver->stateVotes, stateTakerId);
	mapPut(stateGiver->stateVotes, stateTakerId, current - 1);
}


//under check
void sumResultsFromState(State state) {
	//mapSortByKey(state->stateVotes);
	//mapSortByData(state->stateVotes);
	MapKeyElement iterator = mapGetFirst(state->stateVotes);
	for (int i = 0; i < NUMBER_OF_RESULTS_PER_STATE; i++) {
		(state->stateResults)[i] = mapGet(state->stateVotes, iterator);
		iterator = mapGetNext(state->stateVotes);
	}
}

int* getAllResultsFromState(State state) {
	if (!state) return NULL;
	return state->stateResults;
}

int getResultFromStateToState(State stateGiver, int stateTakerId) {
	if (stateGiver == NULL) return -1;
	int* results = stateGiver->stateResults;
	bool inArray = false;
	int index = 0;
	for (index = 0; index < NUMBER_OF_RESULTS_PER_STATE; index++) {
		if (results[index] = stateTakerId) {
			inArray = true;
			break;
		}
	}
	if (!inArray) return 0;
	else {
		switch (index)
		{
		case 0: return 12;
		case 1:	return 10;
		case 2: return 8;
		case 3: return 7;
		case 4: return 6;
		case 5: return 5;
		case 6: return 4;
		case 7: return 3;
		case 8: return 2;
		case 9: return 1;
		default: break;
		}
	}
	return 0;
}

int getVoteFromStateToState(State stateGiver, int stateTakerId) {
	int votes = mapGet(stateGiver->stateVotes, stateTakerId);
	if (votes == NULL) return NO_VOTES;
	return votes;
}

StateResult removeAllVotesFromStateToState(State stateGiver, int stateTakerId) {
	MapResult result = mapRemove(stateGiver->stateVotes, stateTakerId);
	if (result == MAP_NULL_ARGUMENT) return STATE_NULL_ARGUMENT;
	else if (result == MAP_ITEM_DOES_NOT_EXIST) return STATE_NOT_EXIST;
	return STATE_SUCCESS;
}