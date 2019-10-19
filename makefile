CC=g++
CFLAGS=-I.
DEPS = p1.h
OBJ = subProject1.cc


%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


sub_project1: $(OBJ)
		$(CC) -o $@ $^ $(CFLAGS)
