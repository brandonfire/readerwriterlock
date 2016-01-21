c = gcc
LDFLAGS = -pthread

TARGETS = rwlock rwlock_test 

all: bench
bench: $(TARGETS)

clean:
		rm -f $(TARGETS)

.PHONY: all bench
