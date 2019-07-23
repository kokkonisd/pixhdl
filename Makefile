CFLAGS = -g -O2 -Wall -Wextra -Isrc -rdynamic -DNDEBUG $(OPTFLAGS)
LDLIBS = -ldl $(OPTLIBS)
PREFIX ?= /usr/local

SOURCES = $(wildcard src/**/*.c src/*.c)
OBJECTS = $(patsubst %.c, %.o, $(SOURCES))

TEST_SRC = $(wildcard tests/*_tests.c)
TESTS = $(patsubst %.c, %, $(TEST_SRC))

TARGET = build/libpixhdl.a
SO_TARGET = $(patsubst %.a, %.so, $(TARGET))
BIN_TARGET = $(patsubst build/%.a, bin/%, $(TARGET))

# The Target Build
all: $(TARGET) $(SO_TARGET) $(BIN_TARGET) tests

dev: CFLAGS = -g -Wall -Isrc -Wall -Wextra $(OPTFLAGS)
dev: all

$(TARGET): CFLAGS += -fPIC
$(TARGET): build $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	ranlib $@

$(SO_TARGET): $(TARGET) $(OBJECTS)
	$(CC) -shared -o $@ $(OBJECTS)

$(BIN_TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

build:
	@mkdir -p build
	@mkdir -p bin

# The Unit Tests
.PHONY: tests
# Remove -Wextra because it throws an annoying warning on minunit.h
tests: CFLAGS = -g -O2 -Wall -Isrc -rdynamic -DNDEBUG $(OPTFLAGS)
tests: $(TESTS)
	sh ./tests/runtests.sh

# Explicitly tell the compiler to add the .a file at the end
# because Linux doesn't like it if it's not in that order
$(TESTS): $(TEST_SRC)
	$(CC) $(CFLAGS) $(TEST_SRC) -o $(TESTS) $(TARGET)

# The Cleaner
clean:
	rm -rf build bin $(OBJECTS) $(TESTS)
	rm -f tests/tests.log
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`

# The Install
install: all
	install -d $(DESTDIR)/$(PREFIX)/lib/
	install $(TARGET) $(DESTDIR)/$(PREFIX)/lib/

# The Checker
check:
	@echo Files with potentially dangerous functions.
	@egrep '[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)\
				|stpn?cpy|a?sn?printf|byte_)' $(SOURCES) || true