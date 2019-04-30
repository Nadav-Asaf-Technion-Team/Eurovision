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
	double totalScore;
	const char* stateName;
	const char* songName;
	Map stateVotes;
	int* stateResults;
};

//=================================test function=================================
void checkSumResultsAux(State state) {
	printf("map head is: %d\n", *(int*)mapGetFirst(state->stateVotes));
	printf("map size is: %d\n", mapGetSize(state->stateVotes));
	printf("next is null: %d\n", mapGetNext(state->stateVotes)==NULL);
	MAP_FOREACH(int*, currentId, state->stateVotes) {
		printf("current id (beofre sumresults): %d\n", *currentId);
	};
	sumResultsFromState(state);
	MAP_FOREACH(int*, currentId, state->stateVotes) {
		printf("current id (after sum results): %d\n", *currentId);
	}
}
//===============================end of test function=========================

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
	state->totalScore = 0;
	state->stateId = stateId;
	state->stateName = stateName;
	state->songName = songName;
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

double getTotalScore(State state) {
	if (state == NULL) return -1;
	else return state->totalScore;
}

StateResult setTotalScore(State state, double totalScore) {
	if (state == NULL) return STATE_NULL_ARGUMENT;
	state->totalScore = totalScore;
	return MAP_SUCCESS;
}

int* getAllResultsFromState(State state) {
	if (!state) return NULL;
	return state->stateResults;
}

static StateResult copyStateResults(int* destination, int* source) {
	if (source == NULL || destination == NULL) return STATE_NULL_ARGUMENT;
	for (int i = 0; i < NUMBER_OF_RESULTS_PER_STATE; i++) {
		destination[i] = source[i];
	}
	return STATE_SUCCESS;
}
State stateCopy(State state) {
	if (state == NULL) return NULL;
	int newId = getStateId(state);
	const char* newSong = getSongName(state);
	const char* newStateName = getStateName(state);
	Map newVotes = mapCopy(state->stateVotes);
	int* newResults = malloc(sizeof(int) * NUMBER_OF_RESULTS_PER_STATE);
	if (copyStateResults(newResults, getAllResultsFromState(state)) != STATE_SUCCESS) return NULL;
	State newState = stateCreate(newId, newStateName, newSong);
	newState->stateVotes = newVotes;
	newState->stateResults = newResults;
	newState->totalScore = state->totalScore;
	return newState;
}

void addVoteFromState(State stateGiver, int stateTakerId) {
	int current = 0;
	if (mapGet(stateGiver->stateVotes, &stateTakerId) != NULL) {
		current = *(int*)mapGet(stateGiver->stateVotes, &stateTakerId);
		current++;
	}
	else current = 1;
	mapPut(stateGiver->stateVotes, &stateTakerId, &current);
}

void removeVoteFromState(State stateGiver, int stateTakerId) {
	int current = 0;
	if (mapGet(stateGiver->stateVotes, &stateTakerId) != NULL) {
		current = *(int*)mapGet(stateGiver->stateVotes, &stateTakerId);
		current--;
	}
	else current = 0;
	mapPut(stateGiver->stateVotes, &stateTakerId, &current);
}

//under check
void sumResultsFromState(State state) {
	mapSortByKey(state->stateVotes);
	printf("==============sorted by key===================\n");
	mapSortByDataForInt(state->stateVotes);
	printf("==============sorted by data===================\n");
	MapKeyElement iterator = mapGetFirst(state->stateVotes);
	for (int i = 0; i < NUMBER_OF_RESULTS_PER_STATE; i++) {
		(state->stateResults)[i] = *(int*)mapGet(state->stateVotes, iterator);
		iterator = mapGetNext(state->stateVotes);
		if (iterator == NULL) break;

	}
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
	int votes = mapGet(stateGiver->stateVotes, &stateTakerId);
	if (votes == NULL) return NO_VOTES;
	return *(int*)votes;
}

StateResult removeAllVotesFromStateToState(State stateGiver, int stateTakerId) {
	MapResult result = mapRemove(stateGiver->stateVotes, &stateTakerId);
	if (result == MAP_NULL_ARGUMENT) return STATE_NULL_ARGUMENT;
	else if (result == MAP_ITEM_DOES_NOT_EXIST) return STATE_NOT_EXIST;
	return STATE_SUCCESS;
}