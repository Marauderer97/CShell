TARGETS = om

.PHONY: clean

all: $(TARGETS)

%o: %.c
	gcc -c -o $@ $<
om: execute.o line.o om.o
	gcc -o om execute.o line.o om.o
clean:
	rm -f $(TARGETS) *.o
