CFLAGS = -Wall -std=c99

SRCS = ids.c
OBJS = $(SRCS:.c=.o)
LIBS = -lcrypto
BIN = ids


all: $(BIN)

$(BIN): $(OBJS)
	$(CC) -o $(BIN) $(OBJS) $(LIBS)

clean:
	$(RM) $(BIN) $(OBJS)

