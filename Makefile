CC=gcc
CFLAGS=-g -O2 -Wall

PROGS=pipeline reqrep pair pubsub survey bus
PROG_DIR=bin
OBJS=src/*.o
LIBS=/usr/local/lib/libnanomsg.a -lpthread -lanl

all: $(PROGS)
	mkdir -p $(PROG_DIR)
	mv $^ $(PROG_DIR)

pipeline: src/pipeline.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)
reqrep: src/reqrep.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)
pair: src/pair.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)
pubsub: src/pubsub.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)
survey: src/survey.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)
bus: src/bus.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

clean:
	rm -rf $(PROG_DIR) $(OBJS)
