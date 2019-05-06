CC = gcc
OBJS = eurovision.o libmtm.a eurovisionTests.o eurovisionTestsMain.o judge.o mapForEurovision.o state.o
EXEC = EurovisionContest
COMP_FLAG = -std=c99 -Wall -o EurovisionContest -pedantic-errors -Werror -DNDEBUG

$(EXEC) : $(OBJS)
	$(CC) $(COMP_FLAG) $(OBJS) -o $@
eurovision.o : eurovision.c eurovision.h list.h judge.h state.h mapForEurovision.h
	$(CC) -c $(COMP_FLAG) $*.c
eurovisionTests.o : eurovisionTests.c list.h eurovision.h judge.h state.h mapForEurovision.h eurovisionTests.h
	$(CC) -c $(COMP_FLAG) $*.c
eurovisionTestsMain.o : eurovisionTestsMain.c eurovisionTests.h
	$(CC) -c $(COMP_FLAG) $*.c
judge.o : judge.c judge.h list.h
	$(CC) -c $(COMP_FLAG) $*.c
mapForEurovision.o : mapForEurovision.c mapForEurovision.h
	$(CC) -c $(COMP_FLAG) $*.c
state.o : state.c state.h mapForEurovision.h
	$(CC) -c $(COMP_FLAG) $*.c