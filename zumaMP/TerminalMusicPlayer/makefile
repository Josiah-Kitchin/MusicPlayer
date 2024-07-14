CC = gcc
CFLAGS = -g
LIBS = -lao -lsndfile -lmpg123
SRCDIR = src
OBJDIR = objs

SOURCES = $(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/players/*.c) 

OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

all: play

play: $(OBJECTS)
	$(CC) $(CFLAGS) -o play $(OBJECTS) $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	 rm -f $(OBJECTS) play



