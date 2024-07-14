CC = gcc
CFLAGS = -g $(shell pkg-config --cflags gtk+-3.0 )
LIBS = $(shell pkg-config --libs gtk+-3.0) -lao -lsndfile -lmpg123 
SRCDIR = src
OBJDIR = objs
SOURCES = app.c $(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/players/*.c) $(wildcard $(SRCDIR)/gui/*.c) 

OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))


all: play

play: $(OBJECTS)
	$(CC) $(CFLAGS) -o play $(OBJECTS) $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	 rm -f $(OBJECTS) play



