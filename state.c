#include "state.h"
#include "map.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define NUMBER_OF_RESULTS_PER_STATE 10
#define NO_VOTES 0

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
	if (!state) return NULL;
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

void addVoteFromState(State stateGiver, int stateTakerId) {
	mapPut(stategiver->stateVotes, stateTakerId, ++mapGet(stategiver->stateVotes, stateTakerId));
}

void removeVoteFromState(State stateGiver, int stateTakerId) {
	mapPut(stategiver->stateVotes, stateTakerId, --mapGet(stategiver->stateVotes, stateTakerId));
}

void sumResultsFromState(State state) {
	map reversedMap = mapCreate(copyDataInt, copyKeyInt, freeDataInt, freeKeyInt, compareIntsReversed);
	/*creates a reverset map where every key element in the original map is inserted as data and vise versa
	therefor we'll get a map sorted from biggest number of votes to the smallest */
	MAP_FOREACH(int, stateId, state->stateVotes) {
		mapPut(reversedMap, mapGet(state->stateVotes, stateId), stateId);
	}
	MapKeyElement givenvotes = mapGetFirst(reversedMap);
	for (int i = 0; i < NUMBER_OF_RESULTS_PER_STATE, i++) {
		(state->stateResults)[i] = mapGet(reversedMap, givenVotes);//we need to check what happens if two states got the same votes 
		givenvotes = mapGetNext(reveredMap);
	}
	mapDestroy(reversedMap);
}

int* getAllResultsFromState(State state) {
	if (!state) return NULL;
	return state->stateResults;
}

int getResultFromStateToState(State stateGiver, int stateTakerId) {
	switch (stateTakerId)
	{
	case (stateGiver->stateResults)[0]: return 12;
	case (stateGiver->stateResults)[1]: return 10;
	case (stateGiver->stateResults)[2]: return 8;
	case (stateGiver->stateResults)[3]: return 7;
	case (stateGiver->stateResults)[4]: return 6;
	case (stateGiver->stateResults)[5]: return 5;
	case (stateGiver->stateResults)[6]: return 4;
	case (stateGiver->stateResults)[7]: return 3;
	case (stateGiver->stateResults)[8]: return 2;
	case (stateGiver->stateResults)[9]: return 1;
	default: return 0;
		break;
	}
}

int getVoteFromStateToState(State stateGiver, int stateTakerId) {
	int votes = mapGet(stateGiver->stateVotes, stateTakerId);
	if (votes == NULL) return NO_VOTES;
	return votes;
}

StateResult removeVoteFromState(state stateGiver, int stateTakerId) {
	MapResult result = mapRemove(stateGiver->stateVotes, stateTakerId);
	if (result == MAP_NULL_ARGUMENT) return STATE_NULL_ARGUMENT;
	else if (result == MAP_ITEM_DOES_NOT_EXIST) return STATE_NOT_EXIST;
	return STATE_SUCCESS;
}