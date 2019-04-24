//#include "eurovision.h"
//#include "judge.h"
//#include "state.h"
//#include <stdbool.h>
//
//struct Eurovision_t {
//	List statesList;
//	List judgesList;
//};
//
//Eurovision eurovisionCreate() {
//	Eurovision eurovision = malloc(sizeof(*eurovision));
//	eurovision->statesList = listCreate();
//	eurovision->judgesList = listCreate();
//}
//
////we need to check if its best to implimant the list of states as a map maybe
////also we need to confirm that LIST_FOREACH works as expected
//
//// static function that runs throuhgt the list of states and checks if a spesific id exists 
//static bool stateExist(Eurovision eurovision, int stateIdUnderCheck) {
//	bool stateExist = false;
//	LIST_FOREACH(State, currentState, eurovision->statesList) {
//		if (currentState->stateId == stateIdUnderCheck) {
//			stateExist = true;
//		}
//		return stateExist;
//	}
//}
//
//static bool isNameValid(const char *Name) {
//	const char* charCheck = Name;
//	int i = 0;
//	while (charCheck) {
//		if ((*(charCheck + i) < 'a' || *(charCheck + i) > 'z') && (*(charCheck + i) != ' ')) {
//			return false;
//		}
//		i++;
//	}
//	return true;
//}
//static bool isIdValid(int stateId) {
//	if (stateId < 0) return false;
//	return true;
//}
//
//EurovisionResult eurovisionAddState(Eurovision eurovision, int stateId, const char *stateName, const char *songName) {
//	/*input check*/
//	if (!eurovision || !stateId || !stateName || !songName) return EUROVISION_NULL_ARGUMENT; //check if needed
//	if (!isIdValid(stateId)) return EUROVISION_INVALID_ID;
//	if (stateExist(eurovision, stateId)) return EUROVISION_STATE_ALREADY_EXIST;
//	if (!isNameValid(stateName) || !isNameValid(songName)) return EUROVISION_INVALID_NAME;
//	/*end of input check*/
//	State currentState = stateCreate(stateId, stateName, songName);
//	ListResult result = listInsertAfterCurrent(eurovision->statesList, currentState);
//	if (result = LIST_OUT_OF_MEMORY) { //check if needed
//		stateDestroy(currentState);
//		return EUROVISION_OUT_OF_MEMORY;
//	}
//	return EUROVISION_SUCCESS;
//}
//
//EurovisionResult eurovisionRemoveState(Eurovision eurovision, int stateId) {
//	/*input check*/
//	if (!eurovision || !stateId) return EUROVISION_NULL_ARGUMENT; //check if needed
//	if (stateExist(eurovision, stateId) == false) return EUROVISION_STATE_NOT_EXIST;
//	if (isIdValid(stateId)) return EUROVISION_INVALID_ID;
//	/*end of input check*/
//	LIST_FOREACH(State, currentState, eurovision->statesList) {
//		if (currentState->stateId == stateId) {
//			listRemoveCurrent(eurovision->statesList);
//			break;
//		}
//	}
//		//we need to check for a jugde that gave points to this state, and remove him, will be done 
//		//	provided the proper function
//		LIST_FOREACH(State, currentState, eurovision->statesList) {
//			if (getVoteFromStateToState(currentState, stateId)) {
//				removeAllVotesFromStateToState(currentState, stateId);
//			}
//			return EUROVISION_SUCCESS;
//
//			EurovisionResult eurovisionAddVote(Eurovision eurovision, int stateGiver,
//				int stateTaker);
//
//			EurovisionResult eurovisionRemoveVote(Eurovision eurovision, int stateGiver,
//				int stateTaker);
//
//		}
//	}