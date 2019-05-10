TAG = RD
HEAD = RD.h
DEP1 = main
DEPS = $(DEP1).o

$(TAG): $(DEPS)
	gcc -g -Wall $^ -lm -o $@
	rm -f $(DEPS)

%.o: %.c
	gcc -g -Wall -c $^ -o $@

%.c: $(HEAD)

clean:
	rm -f $(DEPS)



