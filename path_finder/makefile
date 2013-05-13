CC = gcc
CFLAGS = -Wall
LDFLAGS = 
EXECUTABLE=run


default:$(EXECUTABLE)

run: ld.o dijkstra.o attach.o topology.o 
	$(CC) $(CFLAGS) -o run ld.o dijkstra.o attach.o topology.o -ljansson -lcurl

ld.o: ld.c
	$(CC) $(CFLAGS) -c ld.c

dijkstra.o: dijkstra.c
	$(CC) $(CFLAGS) -c dijkstra.c

attach.o: attach.c
	$(CC) $(CFLAGS) -c attach.c

topology.o: topology.c
	$(CC) $(CFLAGS) -c topology.c

clean:
	rm -rf *o run

