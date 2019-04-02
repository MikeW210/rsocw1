CFLAGS=-ggdb -Wall -pedantic -D_REENTRANT

%: %.c
	gcc $(CFLAGS) $< -o $@ -lpthread -lm

EXECS = fork1 fork2 fork3 fork4 fork5 fork6 \
		netclient server1 server2 eintrc eintrs \
		threadsafe1 threadsafe2 threadsafe3 threadsafe4 \
		threadsafe5 threadsafe6 threadsafe7 \
		signal_example mutex condvar

all: $(EXECS)

fork1: fork1.c

fork2: fork2.c

fork3: fork3.c

fork4: fork4.c

fork5: fork5.c

fork6: fork6.c

netclient: netclient.c

server1: server1.c

server2: server2.c

eintrc: eintrc.c

eintrs: eintrs.c

threadsafe1: threadsafe1.c

threadsafe2: threadsafe2.c

threadsafe3: threadsafe3.c

threadsafe4: threadsafe4.c

threadsafe5: threadsafe5.c

threadsafe6: threadsafe6.c

threadsafe7: threadsafe7.c

condvar: condvar.c

mutex: mutex.c

signal_example: signal_example.c

.PHONY: clean all

clean:
	rm -f $(EXECS)
