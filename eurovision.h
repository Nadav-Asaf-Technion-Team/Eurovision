#ifndef EUROVISION_H_
#define EUROVISION_H_

#include "list.h"

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


typedef struct eurovision_t* Eurovision;

Eurovision eurovisionCreate(); //Asaf

void eurovisionDestroy(Eurovision eurovision); //Asaf

EurovisionResult eurovisionAddState(Eurovision eurovision, int stateId,
	const char* stateName,
	const char* songName); // Nadav

EurovisionResult eurovisionRemoveState(Eurovision eurovision, int stateId); //Nadav

EurovisionResult eurovisionAddJudge(Eurovision eurovision, int judgeId,
	const char* judgeName,
	int* judgeResults); //Asaf

EurovisionResult eurovisionRemoveJudge(Eurovision eurovision, int judgeId); //Asaf

EurovisionResult eurovisionAddVote(Eurovision eurovision, int stateGiver,
	int stateTaker);

EurovisionResult eurovisionRemoveVote(Eurovision eurovision, int stateGiver,
	int stateTaker);

List eurovisionRunContest(Eurovision eurovision, int audiencePercent);

List eurovisionRunAudienceFavorite(Eurovision eurovision);

List eurovisionRunGetFriendlyStates(Eurovision eurovision);


#endif /* EUROVISION_H_ */