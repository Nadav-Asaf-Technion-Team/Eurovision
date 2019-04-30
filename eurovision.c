#include "eurovision.h"
#include "judge.h"
#include "state.h"

struct Eurovision_t {
	List statesList;
	List judgesList;
};

//_______________test functions_________________
int getAmountOfStates(Eurovision eurovision) {
	return listGetSize(eurovision->statesList);
}

void checkSumResults(Eurovision eurovision, int stateId) {
	LIST_FOREACH(State, current, eurovision->statesList) {
		if (getStateId(current) == stateId) {
			checkSumResultsAux(current);
		}
			
	}
}
//_______________end oftest functions___________

static bool isIdValid(int id) {
	if (id < 0) return false;
	return true;
}

static bool isNameValid(const char* Name) {
	const char* charCheck = Name;
	int i = 0;
	while (*(charCheck + i)) {
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

static int calculateTotal(int audiencePercent, int audienceAvarage, int judgesAvarage) {
	int audiencePart = audiencePercent / 100;
	int judgesPart = 1 - audiencePart;
	return ((audienceAvarage * audiencePart) + (judgesAvarage * judgesPart));
}

static int compareStatesByScore(State first, State second) {
	if (getTotalScore(first) == getTotalScore(second))
		return getStateId(first) - getStateId(second);
	else
		return -1 * (getTotalScore(first) - getTotalScore(second));
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
	if (eurovision == NULL || judgeName == NULL || judgeResults == NULL) return EUROVISION_NULL_ARGUMENT;
	if (!isIdValid(judgeId)) return EUROVISION_INVALID_ID;
	if (!isNameValid(judgeName)) return EUROVISION_INVALID_NAME;
	//state not exist should be here
	if (judgeExist(eurovision, judgeId)) return EUROVISION_JUDGE_ALREADY_EXIST;
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
	LIST_FOREACH(Judge, currentJudge, eurovision->judgesList) {
		if (getJudgeId(currentJudge) == judgeId) {
			listRemoveCurrent(eurovision->judgesList);
		}
	}
	return EUROVISION_SUCCESS;
}

//we need to check if its best to implimant the list of states as a map maybe
//also we need to confirm that LIST_FOREACH works as expected

EurovisionResult eurovisionAddState(Eurovision eurovision, int stateId, const char *stateName, const char *songName) {
	/*input check*/
	if (!eurovision || !stateName || !songName) return EUROVISION_NULL_ARGUMENT; 
	if (!isIdValid(stateId)) return EUROVISION_INVALID_ID;
	if (!isNameValid(stateName) || !isNameValid(songName)) return EUROVISION_INVALID_NAME;
	if (stateExist(eurovision, stateId)) return EUROVISION_STATE_ALREADY_EXIST;
	/*end of input check*/
	State currentState = stateCreate(stateId, stateName, songName);
	ListResult result = listInsertLast(eurovision->statesList, currentState);
	if (result == LIST_OUT_OF_MEMORY) { //check if needed
		stateDestroy(currentState);
		return EUROVISION_OUT_OF_MEMORY;
	}
	return EUROVISION_SUCCESS;
}

EurovisionResult eurovisionRemoveState(Eurovision eurovision, int stateId) {
	/*input check*/
	if (!eurovision) return EUROVISION_NULL_ARGUMENT;
	if (!isIdValid(stateId)) return EUROVISION_INVALID_ID;
	if (stateExist(eurovision, stateId) == false) return EUROVISION_STATE_NOT_EXIST;
	/*end of input check*/
	LIST_FOREACH(State, currentState, eurovision->statesList) {
		if (getStateId(currentState) == stateId) {
			listRemoveCurrent(eurovision->statesList);
			break;
		}
	}
		//we need to check for a jugde that gave points to this state, and remove him, will be done 
		//	provided the proper function
	List judgesListCopy = listCopy(eurovision->judgesList);
	LIST_FOREACH(Judge, currentJudge, judgesListCopy) {
		if (getResultFromJudge(currentJudge, stateId) != 0) {
			eurovisionRemoveJudge(eurovision, getJudgeId(currentJudge));
		}
	}
	listDestroy(judgesListCopy);

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

List eurovisionRunContest(Eurovision eurovision, int audiencePercent) {
	if (eurovision == NULL || audiencePercent < 1 || audiencePercent > 100) return NULL;
	int audienceTotal = 0, judgesTotal = 0, audienceAvarage = 0, judgesAvarage = 0;
	int totalStateScore = 0;
	List rank = listCopy(eurovision->statesList);
	if (listGetSize(rank) == 0) return rank;
	LIST_FOREACH(State, rankedState, rank) {
		audienceTotal = 0;
		judgesTotal = 0;
		LIST_FOREACH(State, rankingState, eurovision->statesList) {
			audienceTotal += getResultFromStateToState(rankingState, rankedState);
		}
		LIST_FOREACH(Judge, rankingJudge, eurovision->judgesList) {
			judgesTotal += getResultFromJudge(rankingJudge, getStateId(rankedState));
		}
		audienceAvarage = audienceTotal / listGetSize(eurovision->statesList);
		judgesAvarage = judgesTotal / listGetSize(eurovision->judgesList);
		totalStateScore = calculateTotal(audiencePercent, audienceAvarage, judgesAvarage);
		setTotalScore(rankedState, totalStateScore);
	}
	if (listSort(rank, compareStatesByScore) == NULL) return EUROVISION_OUT_OF_MEMORY;
	return rank;
}

List eurovisionRunAudienceFavorite(Eurovision eurovision) {
	return eurovisionRunContest(eurovision, 100);
}