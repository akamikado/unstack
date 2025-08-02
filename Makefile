CC = gcc
CFLAGS = -Wall -g
UNSTACK_OBJS = build/lib/inet.o\
							 build/lib/ip.o\
							 build/lib/netdev.o\
							 build/lib/skb.o\
							 build/lib/sock.o\
							 build/lib/socket.o\
							 build/lib/tcp.o\
							 build/lib/tun.o\
							 build/lib/util.o\

build: 
	mkdir build -p

build/test: build
	mkdir build/test -p

build/test/connection: build/test
	mkdir build/test/connection -p

build/lib: build
	mkdir build/lib -p

test_connection: $(UNSTACK_OBJS) build/test/connection test/connection/client.c test/connection/server.c
	$(CC) $(CFLAGS) -o build/test/connection/client test/connection/client.c $(UNSTACK_OBJS)
	$(CC) $(CFLAGS) -o build/test/connection/server test/connection/server.c $(UNSTACK_OBJS)

build/lib/%.o:lib/%.c build/lib
	$(CC) $(CFLAGS) -c -o $@ $<

clean: build
	rm -r build/
