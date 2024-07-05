CC = gcc
CFLAGS = -g
LIBS = -lao -lsndfile -lmpg123
SRCDIR = src
OBJDIR = objs

SOURCES = $(wildcard $(SRCDIR)/*.c) 

OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

all: musplay 

musplay: $(OBJECTS)
	$(CC) $(CFLAGS) -o musplay $(OBJECTS) $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	 rm -f $(OBJECTS) musplay



