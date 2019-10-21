CC=g++
CFLAGS=-I.
DEPS = p1.h
OBJ = subProject1.cc
OBJ2 = subProject2.cc


%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


sub_project1: $(OBJ)
		$(CC) -o $@ $^ $(CFLAGS)

sub_project2: $(OBJ2)
		$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm sub_project1
