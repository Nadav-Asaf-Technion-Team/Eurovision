#include "eurovision.h"


#define MAX_RESULT 12
#define FRIENDLY_SEPERATOR " - "

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

static double calculateTotal(int audiencePercent, double audienceAvarage, double judgesAvarage) {
	double audiencePart = (double)audiencePercent / 100;
	double judgesPart = 1 - audiencePart;
	double result = ((audienceAvarage * audiencePart) + (judgesAvarage * judgesPart));
	return result;
}

static int compareStatesByScore(State first, State second) {
	double diff = getTotalScore(second) - getTotalScore(first);
	if (diff == 0) {
		return ((getStateId(first) - getStateId(second)) > 0 ? 1 : -1);
	}
	else if (diff > 0) return 1;
	else return -1;
}

static void sumAllResults(Eurovision eurovision) {
	LIST_FOREACH(State, rankingState, eurovision->statesList) {
		sumResultsFromState(rankingState);
	}
}

static char* createFriendlyString(State firstState, State secondState) {
	const char* firstName = getStateName(firstState);
	const char* secondName = getStateName(secondState);
	int combinedLen = strlen(firstName) + strlen(secondName) + 4; //4 = 2 spaces + '-' + '\0'
	char* friendlyString = malloc(sizeof(char) * combinedLen);
	if (friendlyString == NULL) return NULL;
	strcpy(friendlyString, firstName);
	strcat(friendlyString, FRIENDLY_SEPERATOR);
	strcat(friendlyString, secondName);	
	return friendlyString;
}

static void freeString(char* str) {
	free(str);
}

char* copyString(char* str) {
	if (str == NULL) return NULL;
	char* copy = malloc(strlen(str) + 1);
	if (copy == NULL) return NULL;
	return copy ? strcpy(copy, str) : NULL;
}

Eurovision eurovisionCreate() {
	Eurovision eurovision = malloc(sizeof(*eurovision));
	if (eurovision == NULL) return NULL;
	eurovision->statesList = listCreate((CopyListElement)stateCopy, (FreeListElement)stateDestroy);
	eurovision->judgesList = listCreate((CopyListElement)judgeCopy, (FreeListElement)judgeDestroy);
	return eurovision;
}

void eurovisionDestroy(Eurovision eurovision) {
	listDestroy(eurovision->statesList);
	listDestroy(eurovision->judgesList);
	free(eurovision);
}



EurovisionResult eurovisionAddJudge(Eurovision eurovision, int judgeId,
	const char* judgeName, int* judgeResults) {
	if (eurovision == NULL || judgeName == NULL || judgeResults == NULL) return EUROVISION_NULL_ARGUMENT;
	if (!isIdValid(judgeId)) return EUROVISION_INVALID_ID;
	if (!isNameValid(judgeName)) return EUROVISION_INVALID_NAME;
	for (int i = 0; i < STATES_TO_SCORE; i++) {
		if (!isIdValid(judgeResults[i])) return EUROVISION_INVALID_ID;
		else if (!stateExist(eurovision, judgeResults[i])) return EUROVISION_STATE_NOT_EXIST;		
	}	
	if (judgeExist(eurovision, judgeId)) return EUROVISION_JUDGE_ALREADY_EXIST;
	int* newResults = malloc(sizeof(int) * STATES_TO_SCORE);
	if (newResults == NULL) {
		eurovisionDestroy(eurovision);
		return EUROVISION_OUT_OF_MEMORY;
	}
	for (int i = 0; i < STATES_TO_SCORE; i++) {
		newResults[i] = judgeResults[i];
	}
	Judge judge = judgeCreate(judgeId, judgeName, newResults);
	if (judge == NULL) {
		free(newResults);
		eurovisionDestroy(eurovision);
		return EUROVISION_OUT_OF_MEMORY;
	}
	if (listInsertLast(eurovision->judgesList, judge) == LIST_OUT_OF_MEMORY) {
		judgeDestroy(judge);
		eurovisionDestroy(eurovision);
		return EUROVISION_OUT_OF_MEMORY;
	}
	judgeDestroy(judge);
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
	if (currentState == NULL) {
		eurovisionDestroy(eurovision);
		return EUROVISION_OUT_OF_MEMORY;
	}
	ListResult result = listInsertLast(eurovision->statesList, currentState);
	if (result == LIST_OUT_OF_MEMORY) { 
		stateDestroy(currentState);
		eurovisionDestroy(eurovision);
		return EUROVISION_OUT_OF_MEMORY;
	}
	stateDestroy(currentState);
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
	List judgesListCopy = listCopy(eurovision->judgesList);
	if (judgesListCopy == NULL) {
		eurovisionDestroy(eurovision);
		return EUROVISION_OUT_OF_MEMORY;
	}
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
	return EUROVISION_SUCCESS; 
}

static State getStateFromId(Eurovision eurovision, int stateId) {
	LIST_FOREACH(State, currentState, eurovision->statesList) {
		if (getStateId(currentState) == stateId) return currentState;
	}
	return NULL;
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
	if (addVoteFromState(stateElement, stateTaker) == STATE_OUT_OF_MEMORY) {
		eurovisionDestroy(eurovision);
		return EUROVISION_OUT_OF_MEMORY;
	}
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
	if (removeVoteFromState(stateElement, stateTaker) == STATE_OUT_OF_MEMORY) {
		eurovisionDestroy(eurovision);
		return EUROVISION_OUT_OF_MEMORY;
	}
	return EUROVISION_SUCCESS;
}

List eurovisionRunContest(Eurovision eurovision, int audiencePercent) {
	if (eurovision == NULL || audiencePercent < 1 || audiencePercent > 100) return NULL;
	int audienceTotal = 0, judgesTotal = 0;
	double audienceAvarage = 0, judgesAvarage = 0;
	double totalStateScore = 0;
	List rank = listCopy(eurovision->statesList);
	if (rank == NULL) {
		eurovisionDestroy(eurovision);
		return NULL;
	}
	if (listGetSize(rank) == 0) return rank;
	sumAllResults(eurovision);
	LIST_FOREACH(State, rankedState, rank) {
		audienceTotal = 0;
		judgesTotal = 0;
		LIST_FOREACH(State, rankingState, eurovision->statesList) {
			if (getResultFromStateToState(rankingState, getStateId(rankedState)) != -1) {
				audienceTotal += getResultFromStateToState(rankingState, getStateId(rankedState));
			}
		}
		//printf("====================Audience total: %d====================\n", audienceTotal);
		LIST_FOREACH(Judge, rankingJudge, eurovision->judgesList) {
			judgesTotal += getResultFromJudge(rankingJudge, getStateId(rankedState));
		}
		audienceAvarage = (double)audienceTotal / (double)(listGetSize(eurovision->statesList) - 1);
		judgesAvarage = (double)judgesTotal / (double)listGetSize(eurovision->judgesList);
		totalStateScore = calculateTotal(audiencePercent, audienceAvarage, judgesAvarage);
		setTotalScore(rankedState, totalStateScore);
	}
	if (listSort(rank, (CompareListElements)compareStatesByScore) == LIST_OUT_OF_MEMORY) {
		listDestroy(rank);
		eurovisionDestroy(eurovision);
		return NULL;
	}
	List rankByName = listCreate((CopyListElement)copyString, (FreeListElement)freeString);
	if (rankByName == NULL) {
		listDestroy(rank);
		eurovisionDestroy(eurovision);
		return NULL;
	}
	LIST_FOREACH(State, currentState, rank) {
		if (listInsertLast(rankByName, (char*)getStateName(currentState)) == LIST_OUT_OF_MEMORY) {
			listDestroy(rank);
			listDestroy(rankByName);
			eurovisionDestroy(eurovision);
			return NULL;
		}
	}
	listDestroy(rank);
	return rankByName;
}

List eurovisionRunAudienceFavorite(Eurovision eurovision) {
	return eurovisionRunContest(eurovision, 100);
}

List eurovisionRunGetFriendlyStates(Eurovision eurovision) {
	sumAllResults(eurovision);
	int firstStateId = -1, secondStateId = -1;
	char* str = NULL;
	State secondState = NULL;
	List friendlyStates = listCreate((CopyListElement)copyString, (FreeListElement)freeString);
	if (friendlyStates == NULL) {
		eurovisionDestroy(eurovision);
		return NULL;
	}
	LIST_FOREACH(State, iterator, eurovision->statesList) {
		firstStateId = getStateId(iterator);
		secondStateId = getAllResultsFromState(iterator)[0];
		if (secondStateId == -1) continue;
		secondState = getStateFromId(eurovision, secondStateId);
		if (getResultFromStateToState(secondState, firstStateId) == MAX_RESULT 
			&& !isFriendlied(secondState) && !isFriendlied(iterator)) {
			if (strcmp(getStateName(iterator),getStateName(secondState)) < 0)
				str = createFriendlyString(iterator, secondState);
			else 
				str = createFriendlyString(secondState, iterator);
			if (str == NULL) {
				listDestroy(friendlyStates);
				eurovisionDestroy(eurovision);
				return NULL;
			}
			if (listInsertLast(friendlyStates, str) == LIST_OUT_OF_MEMORY) {
				listDestroy(friendlyStates);
				eurovisionDestroy(eurovision);
				return NULL;
			}
			free(str);
			setFriendlied(iterator, true);
			setFriendlied(secondState, true);
		}
	}
	if (listSort(friendlyStates, (CompareListElements)strcmp) == LIST_OUT_OF_MEMORY) {
		listDestroy(friendlyStates);
		eurovisionDestroy(eurovision);
		return NULL;
	}
	LIST_FOREACH(State, iterator, eurovision->statesList) {
		setFriendlied(iterator, false);
	}
	return friendlyStates;
}