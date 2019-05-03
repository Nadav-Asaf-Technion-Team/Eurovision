CC = gcc
OBJS = eurovision.o libmtm.a eurovisionTests.o eurovisionTestsMain.o judge.o map.o state.o
EXEC = eurovisionContestMakefile
COMP_FLAG = -std=c99 -Wall -g -pedantic-errors -Werror -DNDEBUG

$(EXEC) : $(OBJS)
	$(CC) $(COMP_FLAG) $(OBJS) -o $@
eurovision.o : eurovision.c eurovision.h list.h judge.h state.h map.h
	$(CC) -c $(COMP_FLAG) $*.c
eurovisionTests.o : eurovisionTests.c list.h eurovision.h judge.h state.h map.h eurovisionTests.h
	$(CC) -c $(COMP_FLAG) $*.c
eurovisionTestsMain.o : eurovisionTestsMain.c eurovisionTests.h
	$(CC) -c $(COMP_FLAG) $*.c
judge.o : judge.c judge.h list.h
	$(CC) -c $(COMP_FLAG) $*.c
map.o : map.c map.h
	$(CC) -c $(COMP_FLAG) $*.c
state.o : state.c state.h map.h
	$(CC) -c $(COMP_FLAG) $*.c