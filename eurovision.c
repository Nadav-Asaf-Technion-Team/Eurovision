#include "eurovision.h"
#include "judge.h"
#include "state.h"

struct Eurovision_t {
	List statesList;
	List judgesList;
};

//_______________test functions_________________
int getAmountOfJudges(Eurovision eurovision) {
	return listGetSize(eurovision->judgesList);
}

static bool isIdValid(int id) {
	if (id < 0) return false;
	return true;
}

static bool isNameValid(const char* Name) {
	const char* charCheck = Name;
	int i = 0;
	while (charCheck) {
		if ((*(charCheck + i) < 'a' || *(charCheck + i) > 'z') && (*(charCheck + i) != ' ')) {
			return false;
		}
		i++;
	}
	return true;
}

static bool stateExist(Eurovision eurovision, int stateIdUnderCheck) {
	LIST_FOREACH(State, currentState, eurovision->statesList) {
		if (getStateId(currentState) == stateIdUnderCheck) return true;
	}
	return false;
}

static bool judgeExist(Eurovision eurovision, int judgeId) {
	LIST_FOREACH(Judge, currentJudge, eurovision->judgesList) {
		if (getJudgeId(currentJudge) == judgeId) return true;
	}
	return false;
}

Eurovision eurovisionCreate() {
	Eurovision eurovision = malloc(sizeof(*eurovision));
	if (eurovision == NULL) return NULL;
	eurovision->statesList = listCreate(stateCopy, stateDestroy);
	eurovision->judgesList = listCreate(judgeCopy, judgeDestroy);
	return eurovision;
}

void eurovisionDestroy(Eurovision eurovision) {
	listDestroy(eurovision->statesList);
	listDestroy(eurovision->judgesList);
	free(eurovision);
}

EurovisionResult eurovisionAddJudge(Eurovision eurovision, int judgeId,
	const char* judgeName,
	int* judgeResults) {
	if (eurovision == NULL || judgeName == NULL) return EUROVISION_NULL_ARGUMENT;
	if (judgeExist(eurovision, judgeId)) return EUROVISION_JUDGE_ALREADY_EXIST;
	if (!isIdValid(judgeId)) return EUROVISION_INVALID_ID;
	if (!isNameValid(judgeName)) return EUROVISION_INVALID_NAME;
	int* newResults = malloc(sizeof(int) * STATES_TO_SCORE);
	if (newResults == NULL) return EUROVISION_OUT_OF_MEMORY;
	for (int i = 0; i < STATES_TO_SCORE; i++) {
		if (!isIdValid(judgeResults[i])) return EUROVISION_INVALID_ID;
		else if (!stateExist(eurovision, judgeResults[i])) return EUROVISION_STATE_NOT_EXIST;
		else {
			newResults[i] = judgeResults[i];
		}
	}
	Judge judge = judgeCreate(judgeId, judgeName, newResults);
	if (listInsertLast(eurovision->judgesList, judge) != LIST_SUCCESS) return EUROVISION_OUT_OF_MEMORY;
	return EUROVISION_SUCCESS;
}

EurovisionResult eurovisionRemoveJudge(Eurovision eurovision, int judgeId) {
	if (!isIdValid(judgeId)) return EUROVISION_INVALID_ID;
	if (!judgeExist(eurovision, judgeId)) return EUROVISION_JUDGE_NOT_EXIST;
	Judge toRemove = NULL;
	LIST_FOREACH(Judge, currentJudge, eurovision->judgesList) {
		if (getJudgeId(currentJudge) == judgeId) {
			toRemove = currentJudge;
			listRemoveCurrent(eurovision->judgesList);
			judgeDestroy(toRemove);
		}
	}
	return EUROVISION_SUCCESS;
}

//we need to check if its best to implimant the list of states as a map maybe
//also we need to confirm that LIST_FOREACH works as expected

EurovisionResult eurovisionAddState(Eurovision eurovision, int stateId, const char *stateName, const char *songName) {
	/*input check*/
	if (!eurovision || !stateId || !stateName || !songName) return EUROVISION_NULL_ARGUMENT; //check if needed
	if (!isIdValid(stateId)) return EUROVISION_INVALID_ID;
	if (stateExist(eurovision, stateId)) return EUROVISION_STATE_ALREADY_EXIST;
	if (!isNameValid(stateName) || !isNameValid(songName)) return EUROVISION_INVALID_NAME;
	/*end of input check*/
	State currentState = stateCreate(stateId, stateName, songName);
	ListResult result = listInsertAfterCurrent(eurovision->statesList, currentState);
	if (result = LIST_OUT_OF_MEMORY) { //check if needed
		stateDestroy(currentState);
		return EUROVISION_OUT_OF_MEMORY;
	}
	return EUROVISION_SUCCESS;
}

EurovisionResult eurovisionRemoveState(Eurovision eurovision, int stateId) {
	/*input check*/
	if (!eurovision || !stateId) return EUROVISION_NULL_ARGUMENT; //check if needed
	if (stateExist(eurovision, stateId) == false) return EUROVISION_STATE_NOT_EXIST;
	if (isIdValid(stateId)) return EUROVISION_INVALID_ID;
	/*end of input check*/
	LIST_FOREACH(State, currentState, eurovision->statesList) {
		if (getStateId(currentState) == stateId) {
			listRemoveCurrent(eurovision->statesList);
			break;
		}
	}
		//we need to check for a jugde that gave points to this state, and remove him, will be done 
		//	provided the proper function
		LIST_FOREACH(State, currentState, eurovision->statesList) {
			if (getVoteFromStateToState(currentState, stateId)) {
				removeAllVotesFromStateToState(currentState, stateId);
			}
			return EUROVISION_SUCCESS;
		}
}

static State getStateFromId(Eurovision eurovision, int stateId) {
	LIST_FOREACH(State, currentState, eurovision->statesList) {
		if (getStateId(currentState) == stateId) return currentState;
	}
}
EurovisionResult eurovisionAddVote(Eurovision eurovision, int stateGiver,int stateTaker) {
	/*input check*/
	if (!eurovision || !stateGiver || !stateTaker) return EUROVISION_NULL_ARGUMENT;
	if (!isIdValid(stateGiver) || !isIdValid(stateTaker)) return EUROVISION_INVALID_ID;
	if (!stateExist(eurovision, stateGiver) || !stateExist(eurovision, stateTaker)) {
		return EUROVISION_STATE_NOT_EXIST;
	}
	if (stateGiver == stateTaker) return EUROVISION_SAME_STATE;
	/*end of input check*/
	State stateElement = getStateFromId(eurovision, stateGiver); 
	addVoteFromState(stateElement, stateTaker);
	return EUROVISION_SUCCESS;
}

EurovisionResult eurovisionRemoveVote(Eurovision eurovision, int stateGiver,
	int stateTaker){
	/*input check*/
	if (!eurovision || !stateGiver || !stateTaker) return EUROVISION_NULL_ARGUMENT;
	if (!isIdValid(stateGiver) || !isIdValid(stateTaker)) return EUROVISION_INVALID_ID;
	if (!stateExist(eurovision, stateGiver) || !stateExist(eurovision, stateTaker)) {
		return EUROVISION_STATE_NOT_EXIST;
	}
	if (stateGiver == stateTaker) return EUROVISION_SAME_STATE;
	/*end of input check*/
	State stateElement = getStateFromId(eurovision, stateGiver); 
	removeVoteFromState(stateElement, stateTaker);
	return EUROVISION_SUCCESS;
}
