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

void printAllResults(Eurovision eurovision) {
	int* results = NULL;
	LIST_FOREACH(State, current, eurovision->statesList) {
		printf("%s's results: \n", getStateName(current));
		results = getAllResultsFromState(current);
		for (int i = 0; i < STATES_TO_SCORE; i++) {
			printf("%d, ", results[i]);
		}
		printf("\n");
	}
}
//_______________end oftest functions___________
/*checks if an ID is valid under the given requirements*/
static bool isIdValid(int id) {
	if (id < 0) return false;
	return true;
}
/*checks if a name is valid under the given requirements*/
static bool isNameValid(const char* Name) {
	const char* charCheck = Name;
	int i = 0;
	while (*(charCheck + i)) {
		if ((*(charCheck + i) < 'a' || *(charCheck + i) > 'z') 
			&& (*(charCheck + i) != ' ')) {
			return false;
		}
		i++;
	}
	return true;
}
/*checks if a state exists in a given list. check by ID for its uniqueness*/
static bool stateExist(Eurovision eurovision, int stateIdUnderCheck) {
	LIST_FOREACH(State, currentState, eurovision->statesList) {
		if (getStateId(currentState) == stateIdUnderCheck) return true;
	}
	return false;
}
/*checks if a judge exists in a given list. check by ID for its uniqueness*/
static bool judgeExist(Eurovision eurovision, int judgeId) {
	LIST_FOREACH(Judge, currentJudge, eurovision->judgesList) {
		if (getJudgeId(currentJudge) == judgeId) return true;
	}
	return false;
}
/*calculetes the final score of a song by sum the audience and judges avarage
  score taking in account each givers precentage in the final score*/
static double calculateTotal(int audiencePercent, double audienceAvarage,
	   double judgesAvarage) {
	double audiencePart = (double)audiencePercent / 100;
	double judgesPart = 1 - audiencePart;
	double result = ((audienceAvarage * audiencePart)
					  + (judgesAvarage * judgesPart));
	return result;
}
/* Function to be used by the list for comparing state elements */
static int compareStatesByScore(State first, State second) {
	double diff = getTotalScore(second) - getTotalScore(first);
	if (diff == 0) {
		return ((getStateId(first) - getStateId(second)) > 0 ? 1 : -1);
	} else if (diff > 0) {
		return 1;
	} else return -1;
}
/*preformes the summing of results for every state in a given list*/
static void sumAllResults(Eurovision eurovision) {
	LIST_FOREACH(State, rankingState, eurovision->statesList) {
		sumResultsFromState(rankingState);
	}
}
/*given two states already known as friendly, creates a concatinated string of 
  their name sorted lexicographicly*/
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

/* Function to be used by the list for freeing string elements */
static void freeString(char* str) {
	free(str);
}

/* Function to be used by the list for coping string elements */
static char* copyString(char* str) {
	if (str == NULL) return NULL;
	char* copy = malloc(strlen(str) + 1);
	if (copy == NULL) return NULL;
	return copy ? strcpy(copy, str) : NULL;
}

/*checks if an array (in this case of judges results) contains to equal IDs*/
static bool checkIdDuplicates(int* array) {
	for (int i = 0; i < STATES_TO_SCORE; i++) {
		for (int j = i + 1; j < STATES_TO_SCORE; j++) {
			if (array[i] == array[j]) return true;
		}
	}
	return false;
}

Eurovision eurovisionCreate() {
	Eurovision eurovision = malloc(sizeof(*eurovision));
	if (eurovision == NULL) return NULL;
	eurovision->statesList = listCreate((CopyListElement)stateCopy,
										(FreeListElement)stateDestroy);
	eurovision->judgesList = listCreate((CopyListElement)judgeCopy, 
										(FreeListElement)judgeDestroy);
	return eurovision;
}

void eurovisionDestroy(Eurovision eurovision) {
	listDestroy(eurovision->statesList);
	listDestroy(eurovision->judgesList);
	free(eurovision);
}



EurovisionResult eurovisionAddJudge(Eurovision eurovision, int judgeId,
	const char* judgeName, int* judgeResults) {
	if (eurovision == NULL || judgeName == NULL || judgeResults == NULL) 
		return EUROVISION_NULL_ARGUMENT;
	if (!isIdValid(judgeId)) return EUROVISION_INVALID_ID;
	if (checkIdDuplicates(judgeResults)) return EUROVISION_INVALID_ID;
	if (!isNameValid(judgeName)) return EUROVISION_INVALID_NAME;
	for (int i = 0; i < STATES_TO_SCORE; i++) {
		if (!isIdValid(judgeResults[i])) {
			return EUROVISION_INVALID_ID;
		} else if (!stateExist(eurovision, judgeResults[i])) {
			return EUROVISION_STATE_NOT_EXIST;
		}
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

EurovisionResult eurovisionAddState(Eurovision eurovision, int stateId,
									const char *stateName, 
									const char *songName) {
	/*input check*/
	if (!eurovision || !stateName || !songName) 
		return EUROVISION_NULL_ARGUMENT; 
	if (!isIdValid(stateId)) return EUROVISION_INVALID_ID;
	if (!isNameValid(stateName) || !isNameValid(songName))
		return EUROVISION_INVALID_NAME;
	if (stateExist(eurovision, stateId)) 
		return EUROVISION_STATE_ALREADY_EXIST;
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
	if (stateExist(eurovision, stateId) == false)
		return EUROVISION_STATE_NOT_EXIST;
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

//returns state element according to ID. state's list internal iterator is 
//undefined after using this function
static State getStateFromId(Eurovision eurovision, int stateId) {
	LIST_FOREACH(State, currentState, eurovision->statesList) {
		if (getStateId(currentState) == stateId) return currentState;
	}
	return NULL;
}
                                                                               
EurovisionResult eurovisionAddVote(Eurovision eurovision, int stateGiver,
								   int stateTaker) {
	/*input check*/
	if (!eurovision) return EUROVISION_NULL_ARGUMENT;
	if (!isIdValid(stateGiver) || !isIdValid(stateTaker))
		return EUROVISION_INVALID_ID;
	if (!stateExist(eurovision, stateGiver)
		|| !stateExist(eurovision, stateTaker)) {
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
	if (!eurovision) return EUROVISION_NULL_ARGUMENT;
	if (!isIdValid(stateGiver) || !isIdValid(stateTaker))
		return EUROVISION_INVALID_ID;
	if (!stateExist(eurovision, stateGiver)
		|| !stateExist(eurovision, stateTaker)) {
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
	if (eurovision == NULL || audiencePercent < 1 || audiencePercent > 100)
		return NULL;
	int audienceTotal = 0, judgesTotal = 0;
	double audienceAvarage = 0, judgesAvarage = 0, totalStateScore = 0;
	List rank = listCopy(eurovision->statesList);
	if (rank == NULL) {
		eurovisionDestroy(eurovision);
		return NULL;
	}
	if (listGetSize(rank) < 2) return rank;
	sumAllResults(eurovision);
	LIST_FOREACH(State, rankedState, rank) {
		audienceTotal = 0;
		judgesTotal = 0;
		LIST_FOREACH(State, rankingState, eurovision->statesList) {
			if (getResultFromStateToState(rankingState,
				getStateId(rankedState)) != -1) {
				audienceTotal += getResultFromStateToState(rankingState,
													  getStateId(rankedState));
			}
		}
		LIST_FOREACH(Judge, rankingJudge, eurovision->judgesList) {
			judgesTotal += getResultFromJudge(rankingJudge,
											  getStateId(rankedState));
		}
		audienceAvarage = (double)audienceTotal /
						  (double)(listGetSize(eurovision->statesList) - 1);
		if (listGetSize(eurovision->judgesList) != 0)
			judgesAvarage = (double)judgesTotal / 
							(double)listGetSize(eurovision->judgesList);
		else judgesAvarage = 0;
		totalStateScore = calculateTotal(audiencePercent, audienceAvarage, 
										judgesAvarage);
		setTotalScore(rankedState, totalStateScore);
	}
	if (listSort(rank,(CompareListElements)compareStatesByScore) == 
		LIST_OUT_OF_MEMORY) {
		listDestroy(rank);
		eurovisionDestroy(eurovision);
		return NULL;
	}
	List rankByName = listCreate((CopyListElement)copyString,
								(FreeListElement)freeString);
	if (rankByName == NULL) {
		listDestroy(rank);
		eurovisionDestroy(eurovision);
		return NULL;
	}
	LIST_FOREACH(State, currentState, rank) {
		if (listInsertLast(rankByName, (char*)getStateName(currentState)) == 
			LIST_OUT_OF_MEMORY) {
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
/* resets the iterator of a given list to a given value 
   to be used in cases of nested or conflicting lists*/
static void resetIteratorTo(List list, State iterator) {
	LIST_FOREACH(State, i, list) {
		if (i == iterator) break;
	}
}

List eurovisionRunGetFriendlyStates(Eurovision eurovision) {
	sumAllResults(eurovision);
	int firstStateId = -1, secondStateId = -1;
	char* str = NULL;
	State secondState = NULL;
	List friendlyStates = listCreate((CopyListElement)copyString,
									(FreeListElement)freeString);
	if (friendlyStates == NULL) {
		eurovisionDestroy(eurovision);
		return NULL;
	}

	LIST_FOREACH(State, iterator, eurovision->statesList) {
		firstStateId = getStateId(iterator);
		secondStateId = getAllResultsFromState(iterator)[0];
		if (secondStateId == -1) continue;
		State currentIterator = iterator;
		secondState = getStateFromId(eurovision, secondStateId);
		resetIteratorTo(eurovision->statesList, currentIterator);
		if (getResultFromStateToState(secondState, firstStateId) == MAX_RESULT 
			&& !isFriendlied(secondState) && !isFriendlied(iterator)) {
			if (strcmp(getStateName(iterator), getStateName(secondState)) < 0) {
				str = createFriendlyString(iterator, secondState);
			} else {
				str = createFriendlyString(secondState, iterator);
			}
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
	if (listSort(friendlyStates, (CompareListElements)strcmp) == 
		LIST_OUT_OF_MEMORY) {
		listDestroy(friendlyStates);
		eurovisionDestroy(eurovision);
		return NULL;
	}
	LIST_FOREACH(State, iterator, eurovision->statesList) {
		setFriendlied(iterator, false);
	}
	return friendlyStates;
}